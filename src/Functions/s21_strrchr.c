#include "../s21_string.h"
// oznakfli
char *s21_strrchr(const char *str, int c) {
  const char *result = S21_NULL;
  s21_size_t len = s21_strlen(str);

  for (int i = len; i >= 0; i--) {
    if (str[i] == c) {
      result = &str[i];
      break;
    }
  }
  return (char *)result;
}
