#define CCSV_IMPLEMENTATION
#include "ccsv.h"

int main() {
  FILE* none = NULL;
  CCSV_DataFrame df =  CCSV_parse(none);
  printf("%d\n", CCSV_errno);
  printf("%s\n", CCSV_what(CCSV_errno));
}
