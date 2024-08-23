#include "../s21_string.h"
// oznakfli
char *s21_strncat(char *dest, const char *src, s21_size_t n) {
  s21_size_t i;
  s21_size_t dest_len = s21_strlen(dest);

  for (i = 0; i < n; i++) {
    dest[dest_len + i] = src[i];
  }

  return dest;
}