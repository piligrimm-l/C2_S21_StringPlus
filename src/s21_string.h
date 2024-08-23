#ifndef S21_STRING_H
#define S21_STRING_H

#include <ctype.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#define STR_SIZE 4096
#define BUF_SIZE 256

#define S21_NULL ((void*)0)

typedef unsigned long s21_size_t;

typedef struct {
  int c;        // sprecifiers[0]
  int d;        // sprecifiers[1]
  int i;        // sprecifiers[2]
  int e;        // sprecifiers[3]
  int E;        // sprecifiers[4]
  int f;        // sprecifiers[5]
  int g;        // sprecifiers[6]
  int G;        // sprecifiers[7]
  int o;        // sprecifiers[8]
  int s;        // sprecifiers[9]
  int u;        // sprecifiers[10]
  int x;        // sprecifiers[11]
  int X;        // sprecifiers[12]
  int p;        // sprecifiers[13]
  int n;        // sprecifiers[14]
  int percent;  // sprecifiers[15]
} s_specifiers_t;

typedef struct {
  int dash;
  int plus;
  int space;
  int hash;
  int null;
  int width;
  int star_width;
  int precision;
  int star_precision;
  int h;
  int l;
  int L;
} s_flags_t;

typedef union {
  int i_num;
  unsigned u_num;
  long l_num;
  unsigned long ul_num;
} u_num_t;

void* s21_memchr(const void* str, int c, s21_size_t n);
// 1 Поиск символа в блоке памяти
int s21_memcmp(const void* str1, const void* str2, s21_size_t n);
// 2 Сравнение двух блоков памяти.
void* s21_memcpy(void* dest, const void* src, s21_size_t n);
// 3 Скопировать блок данных из памяти.
void* s21_memset(void* str, int c, s21_size_t n);
// 4 Заполнить n байтов блока памяти указанным символом.
char* s21_strncat(char* dest, const char* src, s21_size_t n);
// 5 Добавление n символов к строке.
char* s21_strchr(const char* str, int c);
// 6 Найти первое вхождение символа в строке.
int s21_strncmp(const char* str1, const char* str2, s21_size_t n);
// 7 Сравнивает не более первых n байтов str1 и str2.
char* s21_strncpy(char* dest, const char* src, s21_size_t n);
// 8 Скопировать n символов строки.
s21_size_t s21_strcspn(const char* str1, const char* str2);
// 9 Выполняет поиск первого вхождения в строку str1 любого из
char* s21_strerror(int errnum);
// 10 Интерпретация кодов ошибок в понятные сообщения об ошибках
s21_size_t s21_strlen(const char* str);
// 11 Вычисляет длину строки str, не включая завершающий нулевой символ.
char* s21_strpbrk(const char* str1, const char* str2);
// 12 Выполняет поиск первого вхождения в строку str1
char* s21_strrchr(const char* str, int c);
// 13 Поиск последнего вхождения указанного символа.
char* s21_strstr(const char* haystack, const char* needle);
// 14 Функция ищет первое вхождение подстроки str2 в строке str1.
char* s21_strtok(char* str, const char* delim);
// 15 Поиск лексем в строке, используя разделители.

int s21_sprintf(char* str, const char* format, ...);
// 16 считывает форматированный ввод из строки.
int s21_sscanf(const char* str, const char* format, ...);
// 17 отправляет форматированный вывод в строку, на которую указывает str.
void* s21_to_upper(const char* str);
// Возвращает копию строки (str), преобразованной в верхний регистр.
void* s21_to_lower(const char* str);
// Возвращает копию строки (str), преобразованной в нижний регистр.
void* s21_insert(const char* src, const char* str, s21_size_t start_index);
// Возвращает новую строку, в которой указанная строка (str) вставлена в
// указанную позицию (start_index) в данной строке (src).
void* s21_trim(const char* src, const char* trim_chars);
// Возвращает новую строку, в которой удаляются все начальные и конечные
// вхождения набора заданных символов (trim_chars) из данной строки (src).

//---------------------------------------------------------------
// add barracol
//---------------------------------------------------------------

void parsing_format(const char* format, s_specifiers_t* specifiers,
                    s_flags_t* flags, s21_size_t* index, va_list factor);
char* c_specifier(char* cp_str, const s_specifiers_t* specifiers,
                  const s_flags_t* flags, va_list factor);
char* d_i_specifier(char* cp_str, const s_specifiers_t* specifiers,
                    const s_flags_t* flags, va_list factor);
char* o_specifier(char* cp_str, const s_specifiers_t* specifiers,
                  const s_flags_t* flags, va_list factor);
char* xX_specifier(char* cp_str, const s_specifiers_t* specifiers,
                   const s_flags_t* flags, va_list factor);
char* f_specifier(char* cp_str, const s_specifiers_t* specifiers,
                  const s_flags_t* flags, va_list factor);
char* gG_specifier(char* cp_str, const s_specifiers_t* specifiers,
                   const s_flags_t* flags, va_list factor);
char* u_specifier(char* cp_str, const s_specifiers_t* specifiers,
                  const s_flags_t* flags, va_list factor);
char* s_specifier(char* cp_str, const s_specifiers_t* specifiers,
                  const s_flags_t* flags, va_list factor);
char* percent_specifier(char* cp_str, const s_specifiers_t* specifiers);
void* width_flag(const s_flags_t* flags, const s_specifiers_t* specifiers);
void* precision_flag(const s_flags_t* flags);
int size_precision(const s_flags_t* flags, const s_specifiers_t* specifiers);
void long_to_str(char* strnum, va_list factor, const s_flags_t* flags);
void int_to_str(char* strnum, va_list factor, const s_flags_t* flags);
void float_to_str(char* strnum, va_list factor, const s_flags_t* flags,
                  const s_specifiers_t* specifiers);
void unsigned_to_str(char* strnum, va_list factor, const s_flags_t* flags);
int str_to_num(const char* strnum);
void reverse_str(char* numstr);
char* read_strnum(const char* format, s21_size_t* index);
void* read_strarg(va_list factor, const s_flags_t* flags);
void zeroing_struct(const void* struct_name, s21_size_t size_struct);
void print_struct(const void* struct_name, s21_size_t size_struct,
                  const char* name_struct);

//---------------------------------------------------------------

void* realloc_pointer(void* data, s21_size_t* data_size, s21_size_t type_size);
void* clean_pointer(void* ptr);
void erralloc(const void* ptr);
void errbordertype(void);

#endif
