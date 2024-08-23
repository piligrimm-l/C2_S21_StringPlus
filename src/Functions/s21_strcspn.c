#include "../s21_string.h"
// oznakfli
s21_size_t s21_strcspn(const char *str1, const char *str2) {
  int flag = 0;
  s21_size_t i, j;

  for (i = 0; str1[i] != '\0'; i++) {
    for (j = 0; str2[j] != '\0'; j++) {
      if (str1[i] == str2[j]) {
        flag = 1;
        break;
      }
    }
    if (flag == 1) {
      break;
    }
  }
  return i;
}
