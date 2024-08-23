#include "../s21_string.h"
// oznakfli
char *s21_strpbrk(const char *str1, const char *str2) {
  int flag = 0;
  const char *result = S21_NULL;
  int i, j;

  for (i = 0; str1[i] != '\0'; i++) {
    for (j = 0; str2[j] != '\0'; j++) {
      if (str1[i] == str2[j]) {
        result = &str1[i];
        flag = 1;
        break;
      }
    }
    if (flag == 1) {
      break;
    }
  }
  return (char *)result;
}