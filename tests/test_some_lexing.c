#define CCSV_IMPLEMENTATION
#include "ccsv.h"
#include <string.h>

int main() {
  const char* source = "hello,123"; 
  __internal_ccsv_lexer state = (__internal_ccsv_lexer) {
    .src = source,
    .len = strlen(source),
    .idx = 0,
  };

  __internal_ccsv_tokkind tok = lex(&state);
  printf("%d\n", tok);
  tok = lex(&state);
  printf("%d\n", tok);
  tok = lex(&state);
  printf("%d\n", tok);
}
