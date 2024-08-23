#include "../s21_string.h"
// Gylbert Dori
// Возвращает копию строки (str), преобразованной в верхний регистр. В случае
// какой-либо ошибки следует вернуть значение NULL.
void* s21_to_upper(const char* str) {
  char* temp = (char*)str;
  char* ptr = S21_NULL;
  if (*temp != '\0') {
    for (s21_size_t i = 0; temp[i] != '\0'; ++i) {
      if (temp[i] > 96 && temp[i] < 123) {
        ptr = s21_strchr(temp, temp[i]);
        *ptr = (char)(temp[i] - 32);
      }
    }
  }
  return temp;
}