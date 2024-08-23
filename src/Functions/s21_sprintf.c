// Barracol
#include "../s21_string.h"

int s21_sprintf(char* str, const char* format, ...) {
  if (!format) {
    fprintf(stderr,
            "ОШИБКА: строка format ссылается на нулевой указатель (NULL)\n");
    exit(EXIT_FAILURE);
  }
  char* cp_str = str;
  s_specifiers_t specifiers = {0};
  s_flags_t flags = {0};
  va_list factor;
  va_start(factor, format);
  for (s21_size_t i = 0; i < s21_strlen(format); ++i) {
    if (format[i] != '%') {
      *cp_str++ = format[i];
    } else {
      parsing_format(format, &specifiers, &flags, &i, factor);
      if (specifiers.c &&
          (flags.plus || flags.space || flags.hash || flags.null ||
           flags.precision || flags.star_precision)) {
        fprintf(stderr,
                "ОШИБКА: использование флагов \' \', \'+\', \'#\', \'0\', "
                "precision, несовместимых со спецификатором \'%%c\'\n");
        exit(EXIT_FAILURE);
      }
      if (specifiers.d || specifiers.i || specifiers.u) {
        if (flags.null && flags.dash) {
          fprintf(stderr,
                  "ОШИБКА: одновременное использование флагов \'-\', \'0\' "
                  "со спецификаторами \'%%d\', \'%%i\' или \'%%u\'\n");
          exit(EXIT_FAILURE);
        }
        if (flags.hash) {
          fprintf(stderr,
                  "ОШИБКА: использование флага \'#\', несовместимого "
                  "со спецификатором \'%%d\', \'%%i\' или \'%%u\'\n");
          exit(EXIT_FAILURE);
        }
      }
      if (specifiers.u && (flags.plus || flags.space)) {
        fprintf(stderr,
                "ОШИБКА: использование флагов \'+\' или  \' \', несовместимых "
                "со спецификатором \'%%u\'\n");
        exit(EXIT_FAILURE);
      }
      cp_str = percent_specifier(cp_str, &specifiers);
      cp_str = c_specifier(cp_str, &specifiers, &flags, factor);
      cp_str = d_i_specifier(cp_str, &specifiers, &flags, factor);
      cp_str = u_specifier(cp_str, &specifiers, &flags, factor);
      cp_str = s_specifier(cp_str, &specifiers, &flags, factor);
      cp_str = f_specifier(cp_str, &specifiers, &flags, factor);
      zeroing_struct((void*)&specifiers, 16);
      zeroing_struct((void*)&flags, 12);
    }
  }
  va_end(factor);
  return s21_strlen(str);
}

void parsing_format(const char* format, s_specifiers_t* specifiers,
                    s_flags_t* flags, s21_size_t* index, va_list factor) {
  ++(*index);
  do {
    if (format[*index] == '-') {
      ++flags->dash;
      ++(*index);
    }
    if (format[*index] == '+') {
      ++flags->plus;
      ++(*index);
    } else if (format[*index] == ' ') {
      ++flags->space;
      ++(*index);
    }
    if (format[*index] == '#') {
      ++flags->hash;
      ++(*index);
    }
    if (format[(*index)] == '0') {
      ++flags->null;
      ++(*index);
    }
    if (format[(*index)] == '*') {
      flags->star_width = va_arg(factor, int);
      ++(*index);
    } else if (isdigit(format[*index])) {
      char* strnum = S21_NULL;
      strnum = read_strnum(format, index);
      flags->width = str_to_num(strnum);
      clean_pointer(strnum);
    }
    if (format[(*index)] == '.') {
      ++(*index);
      if (isdigit(format[*index])) {
        char* strnum = S21_NULL;
        strnum = read_strnum(format, index);
        flags->precision = str_to_num(strnum);
        clean_pointer(strnum);
      } else if (format[*index] == '*') {
        flags->star_precision = va_arg(factor, int);
        ++(*index);
      } else if (isalpha(format[*index])) {
        flags->precision = -1;
      }
    }
    if (format[(*index)] == 'h') {
      ++flags->h;
      ++(*index);
    }
    if (format[(*index)] == 'l') {
      ++flags->l;
      ++(*index);
    }
    if (format[(*index)] == 'L') {
      ++flags->L;
      ++(*index);
    }
    switch (format[*index]) {
      case 'c':
        ++specifiers->c;
        break;
      case 'd':
        ++specifiers->d;
        break;
      case 'i':
        ++specifiers->i;
        break;
      case 'e':
        ++specifiers->e;
        break;
      case 'E':
        ++specifiers->E;
        break;
      case 'f':
        ++specifiers->f;
        break;
      case 'g':
        ++specifiers->g;
        break;
      case 'G':
        ++specifiers->G;
        break;
      case 'o':
        ++specifiers->o;
        break;
      case 's':
        ++specifiers->s;
        break;
      case 'u':
        ++specifiers->u;
        break;
      case 'x':
        ++specifiers->x;
        break;
      case 'X':
        ++specifiers->X;
        break;
      case 'p':
        ++specifiers->p;
        break;
      case 'n':
        ++specifiers->n;
        break;
      case '%':
        ++specifiers->percent;
        break;
    }
    if (!s21_strchr("cdieEfghGlLosuxXpn% -+0123456789*.", format[*index])) {
      fprintf(stderr,
              "ОШИБКА: \'%c\' - не является флагом или спецификатором, "
              "неизвестный символ преорбразования\n",
              format[*index]);
      exit(EXIT_FAILURE);
    }
  } while (!s21_strchr("cdieEfgGosuxXpn%", format[*index]));
}

char* c_specifier(char* cp_str, const s_specifiers_t* specifiers,
                  const s_flags_t* flags, va_list factor) {
  void* c_str = S21_NULL;
  if (specifiers->c && !flags->l) {
    c_str = (char*)width_flag(flags, specifiers);
    if (c_str) {
      if (flags->dash) {
        *(char*)c_str = va_arg(factor, int);
      } else {
        ((char*)c_str)[s21_strlen(c_str) - 1] = va_arg(factor, int);
      }
    } else {
      c_str = (char*)calloc(2, sizeof(char));
      erralloc(c_str);
      *(char*)c_str = va_arg(factor, int);
    }
  } else if (specifiers->c && flags->l) {
    c_str = (wchar_t*)width_flag(flags, specifiers);
    if (c_str) {
      if (flags->dash) {
        *(wchar_t*)c_str = va_arg(factor, wchar_t);
      } else {
        ((wchar_t*)c_str)[wcslen((wchar_t*)c_str) - 1] =
            va_arg(factor, wchar_t);
      }
    } else {
      c_str = (wchar_t*)calloc(2, sizeof(wchar_t));
      erralloc(c_str);
      *(wchar_t*)c_str = va_arg(factor, wchar_t);
    }
  }
  if (c_str) {
    if (flags->l) {
      for (s21_size_t j = 0; j < wcslen(c_str); ++j) {
        *(wchar_t*)cp_str++ = ((wchar_t*)c_str)[j];
      }
    } else {
      for (s21_size_t j = 0; j < s21_strlen(c_str); ++j) {
        *cp_str++ = ((char*)c_str)[j];
      }
    }
    clean_pointer(c_str);
  }
  return cp_str;
}

char* d_i_specifier(char* cp_str, const s_specifiers_t* specifiers,
                    const s_flags_t* flags, va_list factor) {
  char* d_str = S21_NULL;
  char strnum[BUF_SIZE] = {'\0'};
  if (specifiers->d || specifiers->i) {
    if (flags->l) {
      long_to_str(strnum, factor, flags);
    } else {
      int_to_str(strnum, factor, flags);
    }
    d_str = width_flag(flags, specifiers);
    if (d_str && s21_strlen(strnum) < s21_strlen(d_str)) {
      if (flags->dash) {
        s21_size_t i = -1;
        while (strnum[++i]) {
          d_str[i] = strnum[i];
        }
      } else {
        int zero_label = -1;
        if (!isdigit(strnum[0]) && flags->null) {
          d_str[0] = strnum[0];
          ++zero_label;
        }
        for (int i = s21_strlen(d_str) - 1, j = s21_strlen(strnum) - 1;
             j > zero_label; --i, --j) {
          d_str[i] = strnum[j];
        }
      }
    } else {
      d_str = clean_pointer(d_str);
      d_str = strnum;
    }
  }
  if (d_str) {
    for (s21_size_t j = 0; j < s21_strlen(d_str); ++j) {
      *cp_str++ = d_str[j];
    }
    if (d_str != strnum) {
      d_str = clean_pointer(d_str);
    }
  }
  return cp_str;
}

char* f_specifier(char* cp_str, const s_specifiers_t* specifiers,
                  const s_flags_t* flags, va_list factor) {
  char* f_str = S21_NULL;
  char strnum[BUF_SIZE] = {'\0'};
  if (specifiers->f) {
    float_to_str(strnum, factor, flags, specifiers);
    f_str = width_flag(flags, specifiers);
    if (f_str && s21_strlen(strnum) < s21_strlen(f_str)) {
      if (flags->dash) {
        s21_size_t i = -1;
        while (strnum[++i]) {
          if (strnum[i] != '#') {
            f_str[i] = strnum[i];
          }
        }
      } else {
        int zero_label = -1;
        if (!isdigit(strnum[0]) && flags->null) {
          f_str[0] = strnum[0];
          ++zero_label;
        }
        for (int i = s21_strlen(f_str) - 1, j = s21_strlen(strnum) - 1;
             j > zero_label; --i, --j) {
          if (strnum[j] != '#') {
            f_str[i] = strnum[j];
          }
        }
      }
    } else {
      f_str = strnum;
    }
  }
  if (f_str) {
    for (s21_size_t j = 0; j < s21_strlen(f_str); ++j) {
      if (f_str[j] != '#') {
        *cp_str++ = f_str[j];
      }
    }
    if (f_str != strnum) {
      clean_pointer(f_str);
    }
  }
  return cp_str;
}

char* u_specifier(char* cp_str, const s_specifiers_t* specifiers,
                  const s_flags_t* flags, va_list factor) {
  char* u_str = S21_NULL;
  char strnum[BUF_SIZE] = {'\0'};
  if (specifiers->u) {
    unsigned_to_str(strnum, factor, flags);
    u_str = width_flag(flags, specifiers);
    if (u_str && s21_strlen(strnum) < s21_strlen(u_str)) {
      if (flags->dash) {
        s21_size_t i = -1;
        while (strnum[++i]) {
          u_str[i] = strnum[i];
        }
      } else {
        int zero_label = -1;
        if (!isdigit(strnum[0]) && flags->null) {
          u_str[0] = strnum[0];
          ++zero_label;
        }
        for (int i = s21_strlen(u_str) - 1, j = s21_strlen(strnum) - 1;
             j > zero_label; --i, --j) {
          u_str[i] = strnum[j];
        }
      }
    } else {
      u_str = strnum;
    }
  }
  if (u_str) {
    for (s21_size_t j = 0; j < s21_strlen(u_str); ++j) {
      *cp_str++ = u_str[j];
    }
    if (u_str != strnum) {
      clean_pointer(u_str);
    }
  }
  return cp_str;
}

char* s_specifier(char* cp_str, const s_specifiers_t* specifiers,
                  const s_flags_t* flags, va_list factor) {
  void* s_str = S21_NULL;
  void* strarg = S21_NULL;
  if (specifiers->s) {
    if (!flags->l) {
      s_str = (char*)width_flag(flags, specifiers);
      strarg = (char*)read_strarg(factor, flags);
      if (s_str && s21_strlen((char*)strarg) < s21_strlen((char*)s_str)) {
        if (flags->dash) {
          s21_size_t i = -1;
          while (((char*)strarg)[++i]) {
            ((char*)s_str)[i] = ((char*)strarg)[i];
          }
        } else {
          for (int i = (int)s21_strlen((char*)strarg) - 1,
                   j = s21_strlen((char*)s_str) - 1;
               i >= 0; --j, --i) {
            ((char*)s_str)[j] = ((char*)strarg)[i];
          }
        }
      }
    } else {
      s_str = (wchar_t*)width_flag(flags, specifiers);
      strarg = (wchar_t*)read_strarg(factor, flags);
      if (s_str && wcslen((wchar_t*)strarg) < wcslen((wchar_t*)s_str)) {
        if (flags->dash) {
          s21_size_t i = -1;
          while (((wchar_t*)strarg)[++i]) {
            ((wchar_t*)s_str)[i] = ((wchar_t*)strarg)[i];
          }
        } else {
          for (int i = (int)wcslen((wchar_t*)strarg) - 1,
                   j = wcslen((wchar_t*)s_str) - 1;
               i >= 0; --j, --i) {
            ((wchar_t*)s_str)[j] = ((wchar_t*)strarg)[i];
          }
        }
      }
    }
    if (!s_str) {
      s_str = strarg;
    }
  }
  if (s_str) {
    if (!flags->l) {
      for (s21_size_t j = 0; j < s21_strlen((char*)s_str); ++j) {
        *cp_str++ = ((char*)s_str)[j];
      }
    } else {
      for (s21_size_t j = 0; j < wcslen((wchar_t*)s_str); ++j) {
        *(wchar_t*)cp_str++ = ((wchar_t*)s_str)[j];
      }
    }
  }
  if (s_str != strarg) {
    clean_pointer(s_str);
  }
  clean_pointer(strarg);
  return cp_str;
}

char* percent_specifier(char* cp_str, const s_specifiers_t* specifiers) {
  if (specifiers->percent) {
    *cp_str++ = '%';
  }
  return cp_str;
}

void int_to_str(char* strnum, va_list factor, const s_flags_t* flags) {
  char* cp_strnum = strnum;
  char* str_precision = S21_NULL;
  int num = va_arg(factor, int);
  int abs_num = num < 0 ? -1 * num : num;
  do {
    *cp_strnum++ = abs_num % 10 + 48;
  } while (abs_num /= 10);
  if (flags->precision || flags->star_precision) {
    str_precision = precision_flag(flags);
    s21_size_t border = s21_strlen(str_precision) - s21_strlen(strnum);
    if (border > 0) {
      for (s21_size_t i = 0; i != border; ++i) {
        *cp_strnum++ = '0';
      }
    }
  }
  str_precision = clean_pointer(str_precision);
  if (num < 0) {
    *cp_strnum++ = '-';
  } else if (flags->plus) {
    *cp_strnum++ = '+';
  } else if (flags->space) {
    *cp_strnum++ = ' ';
  }
  reverse_str(strnum);
}

void long_to_str(char* strnum, va_list factor, const s_flags_t* flags) {
  char* cp_strnum = strnum;
  char* str_precision = S21_NULL;
  long num = va_arg(factor, long);
  long abs_num = num < 0 ? -1 * num : num;
  do {
    *cp_strnum++ = abs_num % 10 + 48;
  } while (abs_num /= 10);
  if (flags->precision || flags->star_precision) {
    str_precision = precision_flag(flags);
    s21_size_t border = s21_strlen(str_precision) - s21_strlen(strnum);
    if (border > 0) {
      for (s21_size_t i = 0; i != border; ++i) {
        *cp_strnum++ = '0';
      }
    }
  }
  str_precision = clean_pointer(str_precision);
  if (num < 0) {
    *cp_strnum++ = '-';
  } else if (flags->plus) {
    *cp_strnum++ = '+';
  } else if (flags->space) {
    *cp_strnum++ = ' ';
  }
  reverse_str(strnum);
}

void errbordertype(void) {
  fprintf(stderr, "ОШИБКА: аргумент не соответствует спецификатору\n");
  exit(EXIT_FAILURE);
}

void float_to_str(char* strnum, va_list factor, const s_flags_t* flags,
                  const s_specifiers_t* specifiers) {
  char* cp_strnum = strnum;
  double num = va_arg(factor, double);
  int precision = size_precision(flags, specifiers);
  double abs_num = num < 0 ? -1 * num : num;
  long long int_part = floor(abs_num);
  double fract_part = abs_num - int_part;
  int digit = floor(fract_part * 10);
  fract_part = 10 * fract_part - digit;
  if (precision != -1) {
    do {
      *cp_strnum++ = digit + 48;
      digit = floor(fract_part * 10);
      fract_part = 10 * fract_part - digit;
    } while (--precision > 0);
    reverse_str(strnum);
    *cp_strnum++ = '.';
  }
  do {
    *cp_strnum++ = int_part % 10 + 48;
  } while (int_part /= 10);
  *cp_strnum++ = '#';
  if (num < 0) {
    *cp_strnum++ = '-';
  } else if (flags->plus) {
    *cp_strnum++ = '+';
  } else if (flags->space) {
    *cp_strnum++ = ' ';
  }
  reverse_str(strnum);
  if (digit > 4) {
    for (int i = s21_strlen(strnum) - 1; i > -1; --i) {
      if (strnum[i] == '9') {
        strnum[i] = '0';
        if (strnum[i - 1] == '#') {
          strnum[i - 1] = '1';
          break;
        }
      } else if (strnum[i] != '.') {
        ++strnum[i];
        break;
      }
    }
  }
}

void unsigned_to_str(char* strnum, va_list factor, const s_flags_t* flags) {
  char* cp_strnum = strnum;
  char* str_precision = S21_NULL;
  unsigned long num = va_arg(factor, unsigned long);
  if (flags->h) {
    if (num > USHRT_MAX) {
      errbordertype();
    } else {
      num = (unsigned short)num;
    }
  } else if (!flags->l) {
    if (num > UINT_MAX) {
      errbordertype();
    } else {
      num = (unsigned)num;
    }
  }
  do {
    *cp_strnum++ = num % 10 + 48;
  } while (num /= 10);
  if (flags->precision || flags->star_precision) {
    str_precision = precision_flag(flags);
    s21_size_t border = s21_strlen(str_precision) - s21_strlen(strnum);
    if (border > 0) {
      for (s21_size_t i = 0; i != border; ++i) {
        *cp_strnum++ = '0';
      }
    }
  }
  str_precision = clean_pointer(str_precision);
  reverse_str(strnum);
}

int str_to_num(const char* strnum) {
  int num = 0;
  for (s21_size_t i = 0, j = 1; i < s21_strlen(strnum); j *= 10, ++i) {
    num += (strnum[i] - 48) * j;
  }
  return num;
}

void reverse_str(char* strnum) {
  for (s21_size_t i = 0; i != s21_strlen(strnum) / 2; ++i) {
    char tmp = strnum[i];
    strnum[i] = strnum[s21_strlen(strnum) - 1 - i];
    strnum[s21_strlen(strnum) - 1 - i] = tmp;
  }
}

void* read_strarg(va_list factor, const s_flags_t* flags) {
  void* strarg = S21_NULL;
  void* cp_strarg = S21_NULL;
  if (!flags->l) {
    strarg = va_arg(factor, char*);
    cp_strarg = (char*)calloc(s21_strlen((char*)strarg) + 1, sizeof(char));
    erralloc(cp_strarg);
  } else {
    strarg = va_arg(factor, wchar_t*);
    cp_strarg = (wchar_t*)calloc(wcslen((wchar_t*)strarg) + 1, sizeof(wchar_t));
    erralloc(cp_strarg);
  }
  int precision = 0;
  if (flags->precision || flags->star_precision) {
    precision = (flags->precision > flags->star_precision)
                    ? flags->precision
                    : flags->star_precision;
  } else {
    if (!flags->l) {
      precision = s21_strlen((char*)strarg);
    } else {
      precision = wcslen((wchar_t*)strarg);
    }
  }
  if (!flags->l) {
    for (int i = 0; i < precision; ++i) {
      ((char*)cp_strarg)[i] = ((char*)strarg)[i];
    }
  } else {
    for (int i = 0; i < precision; ++i) {
      ((wchar_t*)cp_strarg)[i] = ((wchar_t*)strarg)[i];
    }
  }
  return cp_strarg;
}

char* read_strnum(const char* format, s21_size_t* index) {
  s21_size_t size = BUF_SIZE;
  char* strnum = (char*)calloc(size, sizeof(char));
  erralloc(strnum);
  s21_size_t i = 0;
  while (isdigit(format[*index])) {
    strnum[i++] = format[(*index)++];
    if (i > size - 2) {
      strnum = realloc_pointer(strnum, &size, sizeof(char));
      erralloc(strnum);
    }
  }
  reverse_str(strnum);
  return strnum;
}

void* width_flag(const s_flags_t* flags, const s_specifiers_t* specifiers) {
  int size = 0;
  if (flags->width) {
    size = flags->width;
  }
  if (flags->star_width) {
    size = flags->star_width;
  }
  char* str_width = S21_NULL;
  wchar_t* wstr_width = S21_NULL;
  void* outstr = S21_NULL;
  if (size) {
    if ((specifiers->c || specifiers->s) && flags->l) {
      wstr_width = (wchar_t*)calloc(size + 1, sizeof(wchar_t));
      erralloc(wstr_width);
      for (int i = 0; i < size; ++i) {
        if (flags->null && !flags->dash) {
          wstr_width[i] = '0';
        } else {
          wstr_width[i] = ' ';
        }
      }
      outstr = wstr_width;
    } else {
      str_width = (char*)calloc(size + 1, sizeof(char));
      erralloc(str_width);
      for (int i = 0; i < size; ++i) {
        if (flags->null && !flags->dash) {
          str_width[i] = '0';
        } else {
          str_width[i] = ' ';
        }
      }
      outstr = str_width;
    }
  }
  return outstr;
}

int size_precision(const s_flags_t* flags, const s_specifiers_t* specifiers) {
  int size = 0;
  if (flags->precision) {
    size = flags->precision;
  } else if (flags->star_precision) {
    size = flags->star_precision;
  } else if (!flags->l && (specifiers->f || specifiers->e || specifiers->E ||
                           specifiers->g || specifiers->G)) {
    size = FLT_DIG;
  } else if (flags->l && (specifiers->f || specifiers->e || specifiers->E ||
                          specifiers->g || specifiers->G)) {
    size = DBL_DIG;
  } else if (flags->L && (specifiers->f || specifiers->e || specifiers->E ||
                          specifiers->g || specifiers->G)) {
    size = LDBL_DIG;
  }
  return size;
}

void* precision_flag(const s_flags_t* flags) {
  int size = 0;
  if (flags->precision) {
    size = flags->precision;
  }
  if (flags->star_precision) {
    size = flags->star_precision;
  }
  void* str_precision = S21_NULL;
  if (size) {
    if (!flags->l) {
      str_precision = (char*)calloc(size + 1, sizeof(char));
      erralloc(str_precision);
    } else {
      str_precision = (wchar_t*)calloc(size + 1, sizeof(wchar_t));
      erralloc(str_precision);
    }
    for (int i = 0; i < size; ++i) {
      if (!flags->l) {
        ((char*)str_precision)[i] = '0';
      } else {
        ((wchar_t*)str_precision)[i] = '0';
      }
    }
  }
  return str_precision;
}

void* realloc_pointer(void* data, s21_size_t* data_size, s21_size_t type_size) {
  s21_size_t newsize = *data_size * 2;
  void* new_data = realloc(data, newsize * type_size);
  if (!new_data) {
    free(data);
    data = S21_NULL;
    *data_size = 0;
  } else {
    data = new_data;
    *data_size = newsize;
  }
  return data;
}

void* clean_pointer(void* ptr) {
  if (ptr) {
    free(ptr);
    ptr = S21_NULL;
  }
  return ptr;
}

void erralloc(const void* ptr) {
  if (!ptr) {
    fprintf(stderr, "ОШИБКА: не удалось выделить динамическую память");
    exit(EXIT_FAILURE);
  }
}

void zeroing_struct(const void* struct_name, s21_size_t size_struct) {
  for (int* i = (int*)struct_name; i < (int*)struct_name + size_struct; ++i) {
    *i = 0;
  }
}

void print_struct(const void* struct_name, s21_size_t size_struct,
                  const char* name_struct) {
  int j = 0;
  for (int* i = (int*)struct_name; i < (int*)struct_name + size_struct; ++i) {
    printf("%s[%d] = %d\n", name_struct, j++, *i);
  }
}
