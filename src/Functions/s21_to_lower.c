#include "../s21_string.h"
// Gylbert Dori
// Возвращает копию строки (str), преобразованной в нижний регистр. В случае
// какой-либо ошибки следует вернуть значение NULL.
void* s21_to_lower(const char* str) {
  char* temp = (char*)str;
  char* ptr = S21_NULL;
  if (*temp != '\0') {
    for (s21_size_t i = 0; temp[i] != '\0'; ++i) {
      if (temp[i] > 64 && temp[i] < 91) {
        ptr = s21_strchr(temp, temp[i]);
        *ptr = (char)(temp[i] + 32);
      }
    }
  }
  return temp;
}