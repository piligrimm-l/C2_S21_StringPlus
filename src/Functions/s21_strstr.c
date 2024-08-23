#include "../s21_string.h"
// oznakfli
// GylbertDo 27.02.24
char *s21_strstr(const char *haystack, const char *needle) {
  s21_size_t flag = 0, flag_len = 0;
  int i = 0, j = 0;
  const char *result = S21_NULL;
  if (*haystack != '\0' && *needle != '\0') {
    for (; haystack[i] != '\0';) {
      for (; needle[j] != '\0';) {
        if (haystack[i] == needle[j]) {
          if ((s21_strlen(haystack) - i) < s21_strlen(needle)) {
            flag = 1;
            break;
          } else {
            i++;
            j++;
            flag_len++;
          }
        } else if (haystack[i] != needle[j]) {
          i++;
          j = 0;
          flag_len = 0;
          break;
        }
        if ((needle[j] == '\0') && flag_len == s21_strlen(needle)) {
          result = &haystack[i - j];
          flag = 1;
          break;
        }
      }
      if (flag == 1) {
        break;
      }
    }
  } else if ((*haystack != '\0' && *needle == '\0') ||
             (*haystack == '\0' && *needle == '\0')) {
    result = haystack;
  }
  return (char *)result;
}