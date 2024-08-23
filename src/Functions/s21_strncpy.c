#include "../s21_string.h"
// GylbertDo 27.02.24
char *s21_strncpy(char *dest, const char *src, size_t n) {
  s21_size_t i = 0;

  for (; i < n && i < s21_strlen(src); i++) {
    dest[i] = src[i];
  }

  for (; i < n; i++) {
    dest[i] = '\0';
  }

  return dest;
}
