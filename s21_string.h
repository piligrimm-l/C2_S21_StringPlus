#ifndef S21_STRING_H
#define S21_STRING_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>

#define S21_NULL ((void*)0)

typedef unsigned long s21_size_t;

void*
s21_memchr(const void *str, int c, size_t n);

size_t
s21_strlen(const char *str);

#endif
