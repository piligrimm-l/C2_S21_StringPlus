#include "../s21_string.h"
// oznakfli
//  GylbertDo 27.02.24
char *s21_strchr(const char *str, int c) {
  const char *result = S21_NULL;

  for (s21_size_t i = 0; i <= s21_strlen(str); i++) {
    if (str[i] == c) {
      result = &str[i];
      break;
    }
  }

  return (char *)result;
}