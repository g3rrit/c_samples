// clang main.c

#include <stdio.h>
#include "incf.h"

#define TOSTR(...) (#__VA_ARGS__)
#define hxp TOSTR(
const char a[] =
#include "flag"
);

int main() {

  printf("include: %s\n", a);

  printf("incbin: %s\n", INCBIN("./flag").data);

  return 0;
}
