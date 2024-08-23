// By admiralm
#include "../s21_string.h"

typedef struct {
  int assigned_arguments;
  const char *begin_string;

  bool does_not_assign;
  bool have_width;
  int width;
  bool have_length;
  char length;
  char name;

} Specifiers;

void checkStringAndFormat(const char *str, const char *format);

Specifiers initSpecifier();
void defaultSpecifier(Specifiers *spec);

void someWhiteSpaces(const char **str);
bool iAmSpecifier(const char **str, Specifiers *spec);
void processing(const char *str, const char *format, va_list parameters,
                Specifiers *spec);
bool assignment(const char **str, va_list param, Specifiers *spec);
bool processingSpecifier_c(const char **str, va_list param, Specifiers *spec);
bool processingSpecifier_d_i(const char **str, va_list param, Specifiers *spec,
                             int base);
bool processingSpecifier_f_eE_gG(const char **str, va_list param,
                                 Specifiers *spec);
bool processingSpecifier_o_u_xX(const char **str, va_list param,
                                Specifiers *spec, int base);
bool processingSpecifier_p(const char **str, va_list param, Specifiers *spec,
                           int base);
bool processingSpecifier_s(const char **str, va_list param, Specifiers *spec);
bool processingSpecifier_n(const char **str, va_list param, Specifiers *spec);
bool processingSpecifier_Percent(const char **str, Specifiers *spec);

int s21_sscanf(const char *str, const char *format, ...) {
  Specifiers specifier = initSpecifier();
  specifier.begin_string = str;

  checkStringAndFormat(str, format);

  va_list parameters;
  va_start(parameters, format);

  processing(str, format, parameters, &specifier);

  va_end(parameters);

  return specifier.assigned_arguments;
}

//=====================================================================================================

void checkStringAndFormat(const char *str, const char *format) {
  if (str == S21_NULL) {
    fprintf(stderr, "ERROR: Argument 1 is NULL\n");
    exit(EXIT_FAILURE);
  }

  if (format == S21_NULL) {
    fprintf(stderr, "ERROR: Argument 2 is NULL\n");
    exit(EXIT_FAILURE);
  }

  if (s21_strncmp(format, "", 5) == 0) {
    fprintf(stderr, "ERROR: zero-length gnu_scanf format string\n");
    exit(EXIT_FAILURE);
  }
}

Specifiers initSpecifier() {
  Specifiers spec;
  spec.assigned_arguments = 0;
  defaultSpecifier(&spec);

  return spec;
}

void defaultSpecifier(Specifiers *spec) {
  spec->does_not_assign = false;
  spec->have_width = false;
  spec->width = 0;
  spec->have_length = false;
  spec->length = '\0';
  spec->name = '\0';
}

void someWhiteSpaces(const char **str) {
  while (isspace(**str) && isspace(*(*str + 1))) {
    *str = *str + 1;
  }
}

bool iAmSpecifier(const char **str, Specifiers *spec) {
  bool result = false;
  bool error = false;
  int index = 0;
  defaultSpecifier(spec);

  *str = *str + 1;

  while (**str != '\0' && !result && !error) {
    if (index == 0) {
      if (**str == '*') {
        spec->does_not_assign = true;
        index++;
        *str = *str + 1;
      } else if (isalnum(**str) || **str == '%') {
        index++;
      } else {
        error = true;
        fprintf(stderr,
                "ERROR: Unknown conversion type character \'%c\' in format!\n",
                **str);
      }
    }

    if (index == 1) {
      if (**str >= '1' && **str <= '9') {
        char *temp;
        spec->have_width = true;
        spec->width = strtoul(*str, &temp, 10);
        *str = temp;
        index++;
      } else if (**str == '0') {
        error = true;
        fprintf(stderr, "ERROR: Zero width in gnu_scanf format\n");
      } else {
        index++;
      }
    }

    if (index == 2) {
      if (s21_strchr("hlL", **str)) {  // Fix me s21_
        index++;
        spec->have_length = true;
        spec->length = **str;
        *str = *str + 1;
      } else {
        index++;
      }
    }

    if (index == 3) {
      if (s21_strchr("cdieEfgGosuXxpn%", **str)) {  // Fix me s21_
        spec->name = **str;
        result = true;
        *str = *str + 1;
      } else {
        error = true;
        fprintf(stderr,
                "ERROR: Unknown conversion type character \'%c\' in format!\n",
                **str);
      }
    }
  }
  return result;
}

void processing(const char *str, const char *format, va_list parameters,
                Specifiers *spec) {
  bool flag_processing = true;

  if (s21_strchr(format, '%'))  // Fix me: s21_strchr()
  {
    while (flag_processing && *format != '\0') {
      someWhiteSpaces(&format);

      if (isspace(*format) && isspace(*str)) {
        str = str + 1;
      } else if (isspace(*format) && !isspace(*str)) {
        format = format + 1;
      } else if (*format == *str && *format != '%') {
        format = format + 1;
        str = str + 1;
      } else if (*format == '%') {
        flag_processing = iAmSpecifier(&format, spec);
        if (flag_processing) {
          flag_processing = assignment(&str, parameters, spec);
        }
      } else if (*format != *str && spec->assigned_arguments == 0) {
        flag_processing = false;
        spec->assigned_arguments = EOF;
      } else {
        flag_processing = false;
      }
    }
  }
}

bool assignment(const char **str, va_list param, Specifiers *spec) {
  bool result = true;

  switch (spec->name) {
    case 'c':
      result = processingSpecifier_c(str, param, spec);
      break;
    case 'd':
      result = processingSpecifier_d_i(str, param, spec, 10);
      break;
    case 'i':
      result = processingSpecifier_d_i(str, param, spec, 0);
      break;
    case 'f':
    case 'e':
    case 'E':
    case 'g':
    case 'G':
      result = processingSpecifier_f_eE_gG(str, param, spec);
      break;
    case 'o':
      result = processingSpecifier_o_u_xX(str, param, spec, 8);
      break;
    case 'u':
      result = processingSpecifier_o_u_xX(str, param, spec, 10);
      break;
    case 'x':
    case 'X':
      result = processingSpecifier_o_u_xX(str, param, spec, 16);
      break;
    case 'p':
      result = processingSpecifier_p(str, param, spec, 0);
      break;
    case 's':
      result = processingSpecifier_s(str, param, spec);
      break;
    case 'n':
      result = processingSpecifier_n(str, param, spec);
      break;
    case '%':
      result = processingSpecifier_Percent(str, spec);
      break;
  }

  return result;
}

bool processingSpecifier_c(const char **str, va_list param, Specifiers *spec) {
  bool result = true;

  if (**str != '\0') {
    if (!spec->have_width) {
      spec->width = 1;
    }

    if (spec->does_not_assign && !spec->have_length) {
      *str = *str + spec->width;
    } else if (!spec->does_not_assign) {
      if (!spec->have_length) {
        for (int i = 1; i <= spec->width && **str != '\0'; i++) {
          *(char *)va_arg(param, char *) = **str;
          *str = *str + 1;
        }
        spec->assigned_arguments++;

      } else if (spec->length == 'l') {
        for (int i = 1; i <= spec->width && **str != '\0'; i++) {
          *(wchar_t *)va_arg(param, wchar_t *) = *(wchar_t *)*str;
          *str = *str + 1;
        }
        spec->assigned_arguments++;
      } else {
        result = false;
        fprintf(
            stderr,
            "ERROR: Use of \'%c\' length modifier with \'%%c\' type character "
            "has either no effect or undefined behavior.\n",
            spec->length);
      }
    } else if (spec->does_not_assign && spec->have_length) {
      result = false;
      fprintf(
          stderr,
          "ERROR: Use of assignment suppression and length modifier together "
          "in gnu_scanf format\n");
    } else {
      result = false;
    }
  } else {
    result = false;
    spec->assigned_arguments = EOF;
  }

  return result;
}

bool processingSpecifier_d_i(const char **str, va_list param, Specifiers *spec,
                             int base) {
  bool result = true;
  int value;
  char *temp_ptr = S21_NULL;

  while (isspace(**str)) {
    *str = *str + 1;
  }

  if (isdigit(**str) || ((**str == '-' || **str == '+') &&
                         isdigit(*(*str + 1)) && spec->width != 1)) {
    if (spec->have_width) {
      bool flag_16 = false;
      char temp_str[spec->width + 1];
      int i = 0;

      for (int i = 0; i <= spec->width; i++) {
        temp_str[i] = '\0';
      }

      if ((**str == '-' || **str == '+') && i < spec->width) {
        temp_str[i++] = **str;
        *str = *str + 1;
      }

      if (**str == '0' && i < spec->width) {
        temp_str[i++] = **str;
        *str = *str + 1;
      }

      if ((**str == 'x' || **str == 'X') && i < spec->width) {
        temp_str[i++] = **str;
        *str = *str + 1;
        flag_16 = true;
      }

      while (i < spec->width &&
             (isdigit(**str) ||
              (flag_16 && s21_strchr("ABCDFabcdf", **str)))) {  // FIX me s21_
        temp_str[i++] = **str;
        *str = *str + 1;
      }
      temp_str[spec->width] = '\0';
      value = strtol(temp_str, &temp_ptr, base);

    } else {
      value = strtol(*str, &temp_ptr, base);
      *str = temp_ptr;
    }

    if (!spec->does_not_assign) {
      if (spec->have_length) {
        if (spec->length == 'h') {
          *(short int *)va_arg(param, short int *) = (short int)value;
          spec->assigned_arguments++;
        } else if (spec->length == 'l') {
          *(long int *)va_arg(param, long int *) = (long int)value;
          spec->assigned_arguments++;
        } else if (spec->length == 'L') {
          result = false;
          fprintf(stderr,
                  "ERROR: Use of \'L\' length modifier with \'d\' type "
                  "character has either no effect or undefined behavior.\n");
        }
      } else {
        *(int *)va_arg(param, int *) = (int)value;
        spec->assigned_arguments++;
      }
    } else if (spec->does_not_assign && spec->have_length) {
      result = false;
      fprintf(
          stderr,
          "ERROR: Use of assignment suppression and length modifier together "
          "in gnu_scanf format\n");
    }
  } else if (**str == '\0') {
    spec->assigned_arguments = EOF;
    result = false;
  } else {
    result = false;
  }

  return result;
}

bool processingSpecifier_f_eE_gG(const char **str, va_list param,
                                 Specifiers *spec) {
  bool result = true;
  long double value;
  char *temp_ptr = S21_NULL;

  while (isspace(**str)) {
    *str = *str + 1;
  }

  if (isdigit(**str) || **str == '+' || **str == '-' || **str == '.') {
    if (spec->have_width) {
    } else {
      value = strtod(*str, &temp_ptr);
      *str = temp_ptr;
    }

    if (!spec->does_not_assign) {
      if (spec->have_length) {
        if (spec->length == 'L') {
          *(long double *)va_arg(param, long double *) = (long double)value;
          spec->assigned_arguments++;
        } else if (spec->length == 'h' || spec->length == 'l') {
          result = false;
          fprintf(stderr,
                  "ERROR: Use of \'%c\' length modifier with \'%c\' type "
                  "character has either no effect or undefined behavior.\n",
                  spec->length, spec->name);
        }
      } else {
        *(float *)va_arg(param, float *) = (float)value;
        spec->assigned_arguments++;
      }
    } else if (spec->does_not_assign && spec->have_length) {
      result = false;
      fprintf(
          stderr,
          "ERROR: Use of assignment suppression and length modifier together "
          "in gnu_scanf format\n");
    }
  } else if (**str == '\0') {
    spec->assigned_arguments = EOF;
    result = false;
  } else {
    result = false;
  }

  return result;
}

bool processingSpecifier_o_u_xX(const char **str, va_list param,
                                Specifiers *spec, int base) {
  bool result = true;
  unsigned long value;
  char *temp_ptr = S21_NULL;

  while (isspace(**str)) {
    *str = *str + 1;
  }

  if (isdigit(**str)) {
    if (spec->have_width) {
      bool flag_16 = false;
      char temp_str[spec->width + 1];
      int i = 0;

      if (**str == '0' && i < spec->width) {
        temp_str[i++] = **str;
        *str = *str + 1;
      }

      if ((**str == 'x' || **str == 'X') && i < spec->width) {
        temp_str[i++] = **str;
        *str = *str + 1;
        flag_16 = true;
      }

      while (i < spec->width &&
             (isdigit(**str) ||
              (flag_16 && s21_strchr("ABCDFabcdf", **str)))) {  // FIX me s21_
        temp_str[i++] = **str;
        *str = *str + 1;
      }
      temp_str[spec->width] = '\0';
      value = strtoul(temp_str, &temp_ptr, base);

    } else {
      value = strtoul(*str, &temp_ptr, base);
      *str = temp_ptr;
    }

    if (!spec->does_not_assign) {
      if (spec->have_length) {
        if (spec->length == 'h') {
          *(unsigned short int *)va_arg(param, unsigned short int *) =
              (unsigned short int)value;
          spec->assigned_arguments++;
        } else if (spec->length == 'l') {
          *(unsigned long int *)va_arg(param, unsigned long int *) =
              (unsigned long int)value;
          spec->assigned_arguments++;
        } else if (spec->length == 'L') {
          result = false;
          fprintf(stderr,
                  "ERROR: Use of \'L\' length modifier with \'%c\' type "
                  "character has either no effect or undefined behavior.\n",
                  spec->name);
        }
      } else {
        *(unsigned int *)va_arg(param, unsigned int *) = (unsigned int)value;
        spec->assigned_arguments++;
      }
    } else if (spec->does_not_assign && spec->have_length) {
      result = false;
      fprintf(
          stderr,
          "ERROR: Use of assignment suppression and length modifier together "
          "in gnu_scanf format\n");
    }
  } else if (**str == '\0') {
    spec->assigned_arguments = EOF;
    result = false;
  } else {
    result = false;
  }

  return result;
}

bool processingSpecifier_p(const char **str, va_list param, Specifiers *spec,
                           int base) {
  bool result = true;
  unsigned long value;
  char *temp_ptr = S21_NULL;

  while (isspace(**str)) {
    *str = *str + 1;
  }

  if (isdigit(**str)) {
    if (spec->have_width) {
      bool flag_16 = false;
      char temp_str[spec->width + 1];
      int i = 0;

      if (**str == '0' && i < spec->width) {
        temp_str[i++] = **str;
        *str = *str + 1;
      }

      if ((**str == 'x' || **str == 'X') && i < spec->width) {
        temp_str[i++] = **str;
        *str = *str + 1;
        flag_16 = true;
      }

      while (i < spec->width &&
             (isdigit(**str) ||
              (flag_16 && s21_strchr("ABCDFabcdf", **str)))) {  // FIX me s21_
        temp_str[i++] = **str;
        *str = *str + 1;
      }
      temp_str[spec->width] = '\0';
      value = strtoul(temp_str, &temp_ptr, base);

    } else {
      value = strtoul(*str, &temp_ptr, base);
      *str = temp_ptr;
    }

    if (!spec->does_not_assign) {
      if (spec->have_length) {
        result = false;
        fprintf(stderr,
                "ERROR: Use of \'%c\' length modifier with \'%c\' type "
                "character has either no effect or undefined behavior.\n",
                spec->length, spec->name);

      } else {
        *(unsigned int *)va_arg(param, unsigned int *) = (unsigned int)value;
        spec->assigned_arguments++;
      }
    } else if (spec->does_not_assign && spec->have_length) {
      result = false;
      fprintf(
          stderr,
          "ERROR: Use of assignment suppression and length modifier together "
          "in gnu_scanf format\n");
    }
  } else if (**str == '\0') {
    spec->assigned_arguments = EOF;
    result = false;
  } else {
    result = false;
  }

  return result;
}

bool processingSpecifier_s(const char **str, va_list param, Specifiers *spec) {
  bool result = true;

  while (isspace(**str)) {
    *str = *str + 1;
  }

  if (**str != '\0') {
    if (spec->does_not_assign && !spec->have_length) {
      if (spec->have_width) {
        for (int i = 0; i < spec->width && !isspace(**str) && **str != '\0';
             i++) {
          *str = *str + 1;
        }
      } else {
        while (!isspace(**str) && **str != '\0') {
          *str = *str + 1;
        }
      }

    } else if (spec->does_not_assign && spec->have_length) {
      result = false;
      fprintf(
          stderr,
          "ERROR: Use of assignment suppression and length modifier together "
          "in gnu_scanf format\n");
    } else if (!spec->does_not_assign) {
      if (!spec->have_length) {
        char *value = (char *)va_arg(param, char *);
        if (spec->have_width) {
          for (int i = 0; i < spec->width && !isspace(**str) && **str != '\0';
               i++) {
            value[i] = **str;
            *str = *str + 1;
          }
          value[spec->width] = '\0';
          spec->assigned_arguments++;
        } else {
          int i = 0;
          for (; !isspace(**str) && **str != '\0'; i++) {
            value[i] = **str;
            *str = *str + 1;
          }
          value[i] = '\0';
          spec->assigned_arguments++;
        }
      } else {
        if (spec->length == 'l') {
          wchar_t *value = (wchar_t *)va_arg(param, wchar_t *);
          if (spec->have_width) {
            for (int i = 0; i < spec->width && !isspace(**str) && **str != '\0';
                 i++) {
              value[i] = **str;
              *str = *str + 1;
            }
            value[spec->width] = '\0';
            spec->assigned_arguments++;
          } else {
            int i = 0;
            for (; !isspace(**str) && **str != '\0'; i++) {
              value[i] = **str;
              *str = *str + 1;
            }
            value[i] = '\0';
            spec->assigned_arguments++;
          }
        } else {
          result = false;
          fprintf(stderr,
                  "ERROR: Use of \'%c\' length modifier with \'%%s\' type "
                  "character "
                  "has either no effect or undefined behavior.\n",
                  spec->length);
        }
      }
    }
  } else {
    spec->assigned_arguments = EOF;
    result = false;
  }
  return result;
}

bool processingSpecifier_n(const char **str, va_list param, Specifiers *spec) {
  bool result = true;

  if (spec->does_not_assign || spec->have_width || spec->have_length) {
    result = false;
    fprintf(stderr,
            "ERROR: Specifier \'%%n\' does not have control flags!!!\n");
  } else {
    int value = *str - spec->begin_string;
    *(int *)va_arg(param, int *) = value;
  }

  return result;
}

bool processingSpecifier_Percent(const char **str, Specifiers *spec) {
  bool result = true;

  while (isspace(**str)) {
    *str = *str + 1;
  }

  if (spec->does_not_assign || spec->have_width || spec->have_length) {
    result = false;
    fprintf(stderr,
            "ERROR: Specifier \'%%%%\' does not have control flags!!!\n");
  } else if (**str == '%') {
    *str = *str + 1;
  } else if (**str == '\0') {
    spec->assigned_arguments = EOF;
    result = false;
  } else {
    result = false;
  }

  return result;
}
