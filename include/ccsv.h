#ifndef __CCSV_H__
#define __CCSV_H__ 

#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __GNUC__  // GCC, Clang, and compatible compilers
    #define INTERNAL __attribute__((visibility("hidden")))
#elif defined(_MSC_VER)  // Microsoft Visual Studio
    #define INTERNAL __declspec(selectany) static
#else
    #define INTERNAL static
#endif

typedef enum CCSV_ErrorNums CCSV_ErrorNums; 

typedef struct CCSV_DataFrame CCSV_DataFrame;
typedef struct CCSV_Column CCSV_Column;

extern CCSV_ErrorNums CCSV_errno;
extern const char* CCSV_errstr[];

#define ERR_LIST \
  ERR(NoErr, "Tf are you doing here?? There are no errors why aren't you happy?") \
  ERR(FileDescNull, "The provided File Descriptor is NULL")

enum CCSV_ErrorNums {
#define ERR(X,Y) CCSV_##X,
 ERR_LIST 
#undef ERR
}; 

struct CCSV_DataFrame {
  CCSV_Column* columns; 
  size_t length; 
};


struct CCSV_Column {
  const char* column_name;
};

//lexer 
typedef enum __internal_ccsv_tokkind __internal_ccsv_tokkind;
typedef struct __internal_ccsv_lexer __internal_ccsv_lexer;
#define TOK_LIST \
  X(Iden) \
  X(Comma) \
  X(Number)

enum __internal_ccsv_tokkind {
  #define X(x) __internal_ccsv_kind_##x,
  TOK_LIST
  #undef X
};

#define SHORTED(X) __internal_ccsv_##X

struct __internal_ccsv_lexer {
  const char* src;
  size_t len;
  size_t idx;
};

__internal_ccsv_tokkind lex(__internal_ccsv_lexer* state);
//parser

//main stuff
CCSV_DataFrame CCSV_parse(FILE* file);

// Helper
static inline const char* CCSV_what(CCSV_ErrorNums num) {
  return CCSV_errstr[num]; 
}

#ifdef CCSV_IMPLEMENTATION
#include <ctype.h>

CCSV_ErrorNums CCSV_errno = CCSV_NoErr;
const char* CCSV_errstr[] = {
#define ERR(X,Y) "CCSV_Error: " Y, 
  ERR_LIST 
#undef ERR
};

void lex_numbers(__internal_ccsv_lexer* state, unsigned char* c) {
  state->idx++;
  while(isdigit(*c) && *c != '\0') { 
    state->idx++;
    *c = state->src[state->idx];
  }
  if(*c == '.') {
   while(isdigit(*c) && *c != '\0') {
    state->idx++;
    *c = state->src[state->idx];
   }
  }
} 

__internal_ccsv_tokkind lex(__internal_ccsv_lexer* state){
  unsigned char c = state->src[state->idx];
  switch(c) {
    case 'a' ... 'z':
    case 'A' ... 'Z':
    case '_':
      state->idx++;
      while(isalnum(c) && c != '\0') {
        state->idx++;
        c = state->src[state->idx];
      }
    return SHORTED(kind_Iden);
    case '0' ... '9':
      lex_numbers(state, &c);
      return SHORTED(kind_Number);
    case '.':
      state->idx++;
      if(isdigit(c) && c != '\0') {
        lex_numbers(state, &c);
        return SHORTED(kind_Number);
      }
      return SHORTED(kind_Comma);
   default:
      return SHORTED(kind_Comma);
  }
};

CCSV_DataFrame CCSV_parse(FILE* file){
  if (file == NULL) {
    CCSV_DataFrame df = (CCSV_DataFrame) {
      .columns = NULL,
      .length = 0
    };

    CCSV_errno = CCSV_FileDescNull;
    return df;
  }

  size_t file_len = 0;
  fseek(file, 0, SEEK_END);
  file_len = ftell(file);
  fseek(file, 0, SEEK_SET);


  size_t col_length = 0;
  CCSV_Column* cols = NULL;

  CCSV_DataFrame df = {
    .columns = cols,
    .length = col_length
  };

  return df;
};  
#endif 

#endif 
