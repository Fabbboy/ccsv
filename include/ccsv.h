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
typedef struct __internal_ccsv_token __internal_ccsv_token; 
#define TOK_LIST \
  X(Iden) \
  X(Comma) \
  X(Number)

enum __internal_ccsv_tokkind {
  #define X(x) __internal_ccsv_kind_##x,
  TOK_LIST
  #undef X
};

struct __internal_ccsv_token {
  size_t start_idx;
  size_t len;
  __internal_ccsv_tokkind kind;
};

#define SHORTED(X) __internal_ccsv_##X

struct __internal_ccsv_lexer {
  const char* src;
  size_t len;
  size_t idx;
};

__internal_ccsv_token lex(__internal_ccsv_lexer* state);
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

size_t lex_numbers(__internal_ccsv_lexer *state) {
  size_t start = state->idx;
  unsigned char c = state->src[state->idx];
  while (isdigit(c) && c != '\0') { 
    state->idx++;
    c = state->src[state->idx];
  }
  if (c == '.') {
    state->idx++;
    c = state->src[state->idx];
    while (isdigit(c) && c != '\0') {
      state->idx++;
      c = state->src[state->idx];
    }
  }
  return state->idx - start;
}


 __internal_ccsv_token lex(__internal_ccsv_lexer *state) {
    unsigned char c = state->src[state->idx];

    while (c == ' ' || c == '\t' || c == '\n') {
        state->idx++;
        c = state->src[state->idx];
    }

    switch (c) {
        case 'a' ... 'z': // Fallthrough
        case 'A' ... 'Z': // Fallthrough
        case '_': {
          size_t start = state->idx;
          do {
            state->idx++;
            c = state->src[state->idx];
          } while ((isalnum(c) || c == '_') && c != '\0');
          __internal_ccsv_token tok = (__internal_ccsv_token) {
            .start_idx = start, 
            .kind = __internal_ccsv_kind_Iden,
            .len = state->idx
          };
          return tok;
        } break;
        case '0' ... '9': {
          size_t start = state->idx;
          size_t len = lex_numbers(state);
          __internal_ccsv_token tok = (__internal_ccsv_token) {
            .start_idx = start,
            .kind = __internal_ccsv_kind_Number,
            .len = len
          };
          return tok;
        } break;
        case ',': {
          size_t idx = state->idx;
          state->idx++;
          __internal_ccsv_token tok = (__internal_ccsv_token) {
            .start_idx = idx,
            .len = 1,
            .kind = __internal_ccsv_kind_Comma
          };
          return tok;
        } break;
        default: {
           state->idx++;
          __internal_ccsv_token tok = (__internal_ccsv_token) {
            .start_idx = state->idx - 1,
            .kind = __internal_ccsv_kind_Comma,
            .len = state->idx
          };
          return tok;
        } break;
    }
}

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
