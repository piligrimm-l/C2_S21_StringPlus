#include "../s21_string.h"
// GylbertDo
//  Выполняет поиск первого вхождения символа c (беззнаковый тип) в первых n
//  байтах строки, на которую указывает аргумент str.
void* s21_memchr(const void* str, int c, s21_size_t count) {
  char* out_memchr = S21_NULL;
  char* temp_memchr = (char*)str;
  for (s21_size_t i = 0; i < count; i++) {
    if (c == temp_memchr[i]) {
      out_memchr = &temp_memchr[i];
      break;
    }
  }
  return out_memchr;
}