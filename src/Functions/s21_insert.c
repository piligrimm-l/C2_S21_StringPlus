// Нужно заменить функции на s21_
// admiralm 02.03.24
// barracol 02.03.24
// #include <stdlib.h>
// #include <string.h>

#include "../s21_string.h"

void *s21_insert(const char *src, const char *str, size_t start_index) {
  char *new_str = S21_NULL;

  s21_size_t src_len = s21_strlen(src);
  s21_size_t str_len = s21_strlen(str);

  if (src != S21_NULL && str != S21_NULL && start_index <= src_len) {
    new_str = (char *)calloc(src_len + str_len + 1, sizeof(char));
    s21_strncpy(new_str, src, start_index);
    s21_strncat(new_str, str, str_len);
    s21_strncat(new_str, (src + start_index), s21_strlen(src) - start_index);
  }
  return (void *)new_str;
}