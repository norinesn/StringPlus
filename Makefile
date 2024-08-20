CC = gcc
FLAGS = -std=c11 -Wall -Wextra -Werror
SOURCE = s21_string_special.c s21_string.c s21_sprintf.c
OBJECT = $(patsubst %.c,%.o, $(SOURCE))
TEST = s21_string_test.c

ifeq ($(shell uname),Darwin)
CHECKFLAGS = -lcheck
else
CHECKFLAGS = -lcheck -lm -lsubunit
endif

all: s21_string.a

s21_string.a:
	$(CC) -c $(FLAGS) $(SOURCE)
	ar rcs s21_string.a $(OBJECT)
	rm -rf *.o

test: clean
	$(CC) $(FLAGS) -o s21_string.test --coverage $(SOURCE) $(TEST) $(CHECKFLAGS)
	./s21_string.test

clean:
	rm -rf *.a *.gcno *.gcda coverage_html *.o *.test *.info

gcov_report: clean test
	lcov -d . -c -o s21_string.test.info
	genhtml -o coverage_html s21_string.test.info

clang:
	clang-format --style=google -n *.c
	clang-format --style=google -n *.h

full_test: clean gcov_report clang
	valgrind --tool=memcheck -q --leak-check=yes ./s21_string.test