#ifndef S21_STRING_H
#define S21_STRING_H

#include <stdarg.h>

#include "s21_error_code.h"
#define MIN(a, b) (((a) >= (b)) ? (b) : (a))
#define MAX(a, b) (((a) <= (b)) ? (b) : (a))
#define ABS(a) MAX(a, -a)
#define IS_NUM(a) (a >= '0' && a <= '9')

#define S21_NULL (void *)0
typedef long unsigned s21_size_t;

// 3 of 23 tasks are done

void *s21_memchr(const void *s, int c, s21_size_t n);
int s21_memcmp(const void *s1, const void *s2, s21_size_t n);
void *s21_memcpy(void *dest, const void *src, s21_size_t n);
void *s21_memset(void *str, int c, s21_size_t n);
char *s21_strncat(char *dest, const char *src, s21_size_t n);
char *s21_strchr(const char *str, int c);
int s21_strncmp(const char *str1, const char *str2, s21_size_t n);
char *s21_strncpy(char *dest, const char *src, s21_size_t n);
s21_size_t s21_strcspn(const char *str1, const char *str2);
char *s21_strerror(int errnum);
s21_size_t s21_strlen(const char *str);

char *s21_strpbrk(const char *str1, const char *str2);
char *s21_strrchr(const char *str, int c);
char *s21_strstr(const char *haystack, const char *needle);
char *s21_strtok(char *str, const char *delim);

void *s21_to_upper(const char *str);
void *s21_to_lower(const char *str);
void *s21_insert(const char *src, const char *str, s21_size_t start_index);
void *s21_trim(const char *src, const char *trim_chars);

#define NUMERIC_BUFFER 256

typedef struct s21_p_prot {
  // flags
  int alignment;
  int show_plus;
  int blank;
  int show_hidden;
  int fill_w_zeros;

  int width;
  int precision;
  char add_specf;
  char specf;
} s21_p_prot;

s21_p_prot parse_print_prot(const char *format, int *format_ptr, va_list *args);

int s21_print_short(char **dest, s21_p_prot prot, short num);
int s21_print_long(char **dest, s21_p_prot prot, long num);
int s21_print_int(char **dest, s21_p_prot prot, int num);
int s21_print_ptr(char **dest, unsigned long num);
void s21_make_prefix(char *prefix, s21_p_prot prot, int minus);
int s21_output_num(char **dest, s21_p_prot prot, char *buffer, char *prefix);

int s21_print_double(char **dest, s21_p_prot prot, double num);
int s21_print_long_double(char **dest, s21_p_prot prot, long double num);

int s21_print_str(char **dest, s21_p_prot prot, char *str);

/*---------------------------sprintf-------------------------------*/

int s21_sprintf(char *str, const char *format, ...);

#define NUMERIC_BUFFER 256

typedef struct s21_s_prot {
  int ignore;
  char add_specf;
  char specf;
} s21_s_prot;

#endif  // S21_STRING_H
