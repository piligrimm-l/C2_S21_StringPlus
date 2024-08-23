#include "../s21_string.h"
// oznakfli 03.03.24
char *s21_strtok(char *str, const char *delim) {
  static char *token;

  if (str) {
    token = str;
    while (*token && s21_strchr(delim, *token)) {
      *token++ = '\0';
    }
  }

  if (!*token) {
    return NULL;
  }

  str = token;

  while (*token && !s21_strchr(delim, *token)) ++token;
  while (*token && s21_strchr(delim, *token)) {
    *token++ = '\0';
  }

  return (char *)str;
}
