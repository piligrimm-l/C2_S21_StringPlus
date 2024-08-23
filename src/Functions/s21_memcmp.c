#include "../s21_string.h"
// GylbertDo
// Сравнивает первые n байтов str1 и str2.
int s21_memcmp(const void* str1, const void* str2, s21_size_t n) {
  char* temp1_memcmp = (char*)str1;
  char* temp2_memcmp = (char*)str2;
  int num = 0;
  for (s21_size_t i = 0; i < n; i++) {
    if (temp1_memcmp[i] != temp2_memcmp[i]) {
      num = (int)temp1_memcmp[i] - (int)temp2_memcmp[i];
      break;
    }
  }
  return num;
}