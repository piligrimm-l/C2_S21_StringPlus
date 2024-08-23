#include "../s21_string.h"
// GylbertDo
// Сравнивает не более первых n байтов str1 и str2.
int s21_strncmp(const char* str1, const char* str2, s21_size_t n) {
  char* temp1_strncmp = (char*)str1;
  char* temp2_strncmp = (char*)str2;
  int num = 0;
  for (s21_size_t i = 0; i < n; i++) {
    if (temp1_strncmp[i] != temp2_strncmp[i]) {
      num = (int)temp1_strncmp[i] - (int)temp2_strncmp[i];
      break;
    }
  }
  return num;
}