#include "s21_string.h"
#include <string.h>

#define BUF_SIZE 256

int main(void) {

  char buffer[BUF_SIZE];
  sprintf(buffer, "%s %d %c", "hop-hey-la-la-ley", 111, '!');
  printf("buffer = \"%s\", strlen(buffer) = %d, s21_strlen(bufffer) = %lu \n", buffer, strlen(buffer), s21_strlen(buffer));
  printf("adress the character finded = %p, ch = %c",
          s21_memchr(buffer, 97, 11), *((char*)s21_memchr(buffer, 97,11)));
  return 0;
}
