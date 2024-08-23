#include "../s21_string.h"
// GylbertDo
// Копирует символ c (беззнаковый тип) в первые n символов строки, на которую
// указывает аргумент str.
void *s21_memset(void *str, int c, s21_size_t n) {
  char *temp_memset = str;
  for (s21_size_t i = 0; i < n; ++i) {
    temp_memset[i] = (char)c;
  }
  return temp_memset;
}