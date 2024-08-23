// #include <string.h>
// oznakfli
// barracol 02.03.24
#include <stdbool.h>

#include "../s21_string.h"

// By admiralm

void *s21_trim(const char *src, const char *trim_chars) {
  char *new_str = S21_NULL;

  if (src != S21_NULL) {
    if (src[0] != '\0') {
      const char *head = src;
      const char *tail = src + (s21_strlen(src) - 1);

      bool flag_head = false;
      bool flag_tail = false;

      while ((head <= tail) && (!flag_head || !flag_tail)) {
        if (trim_chars == S21_NULL || trim_chars[0] == '\0') {
          if (isspace(*head)) {
            head = head + 1;
          } else {
            flag_head = true;
          }

          if (isspace(*tail)) {
            tail = tail - 1;
          } else {
            flag_tail = true;
          }
        } else {
          if (s21_strchr(trim_chars, *head)) {
            head = head + 1;
          } else {
            flag_head = true;
          }

          if (s21_strchr(trim_chars, *tail)) {
            tail = tail - 1;
          } else {
            flag_tail = true;
          }
        }
      }

      if (head > tail) {
        new_str = (char *)malloc(sizeof(char));
        *new_str = '\0';
      } else {
        s21_size_t new_str_len = tail - head + 1;
        new_str = (char *)malloc(sizeof(char) * (new_str_len + 1));
        s21_strncpy(new_str, head, new_str_len);  // Fix me: s21_strncpy()
        new_str[new_str_len] = '\0';
      }

    } else {
      new_str = (char *)malloc(sizeof(char));
      *new_str = '\0';
    }
  }

  return new_str;
}

/*


void *s21_trim(const char *src, const char *trim_chars) {
  char *string = (char *)src;

  // обработка конечных символов (удаляя по одному символу, пока есть
  // совпадения)
  s21_size_t i;
  for (i = s21_strlen(string) - 1;
       s21_strchr(trim_chars, string[i]) != S21_NULL; i--) {
    string[i] = '\0';
  }

  // обработка начальных символов
  i = 0;
  s21_size_t j;
  if (string[i] != '\0') {
    while (s21_strchr(trim_chars, string[i]) != S21_NULL) {
      i++;
    }
  }

  if (i > 0) {
    for (j = 0; j < s21_strlen(string) - i; j++) {
      string[j] = string[j + i];
    }
    string[j] = '\0';
  }

  return (void *)string;
}
*/