#include "../s21_string.h"
// GylbertDo
// Копирует n символов из src в dest.
void *s21_memcpy(void *dest, const void *src, s21_size_t n) {
  char *temp_memcpy = (char *)dest;
  char *temp2_memcpy = (char *)src;
  for (s21_size_t i = 0; i < n; ++i) {
    temp_memcpy[i] = temp2_memcpy[i];
  }
  dest = temp_memcpy;
  return dest;
}