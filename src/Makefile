CC = gcc -std=c11
CFLAGS = -Wall -Werror -Wextra
GCOV_FLAGS = -fprofile-arcs -ftest-coverage
SOURCES = $(wildcard Functions/s21_*.c)
TEST_SOURCES = $(wildcard Tests/*.check)
OBJ = $(SOURCES:.c=.o)
TEST_LIBS = s21_main_test.c
LIB_NAME = s21_string.a
EXEC = test

LIBS = -lcheck
OPEN = open
OS := $(shell uname -s)
ifeq ($(OS),Linux)
LIBS += -lsubunit -lm -lrt -lpthread 
OPEN = xdg-open
endif

all: s21_string.a

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $< 

s21_string.a: $(OBJ)
	ar rcs $(LIB_NAME) $^
	ranlib $@
	rm -rf Functions/s21_*.o

test: 
	checkmk clean_mode=1 $(TEST_SOURCES) > s21_main_test.c
	$(CC) $(CFLAGS) $(TEST_LIBS) -L. $(LIB_NAME) $(LIBS) -o $(EXEC)
	./test

gcov_report: clean s21_string.a 
	checkmk clean_mode=1 $(TEST_SOURCES) > s21_main_test.c
	mkdir -p coverage
	$(CC) $(CFLAGS) $(GCOV_FLAGS) -o $(EXEC) $(TEST_LIBS) -L. $(LIB_NAME) $(LIBS)
	./$(EXEC)
	lcov --capture --directory . --output-file coverage.info
	genhtml coverage.info --output-directory coverage
	rm -rf *.gcda *.gcno *.gcov *.info 
	$(OPEN) coverage/index.html

rebuild: clean all

clean:
	rm -f $(OBJ) $(EXEC) $(LIB_NAME) $(TEST_LIBS) check *.gcda *.gcno *.gcov *.info s21_main_test.check
	rm -rf coverage

.PHONY: all clean test gcov_report
