#include <stdio.h>

#include "put_string.h"

void put_string(char *s) {
   int i = 0;
   while (s[i] != '\0') {
      fputc(s[i], stdout);
      i++;
   }
   fputc('\n', stdout);
}
