#include <assert.h>
#include <math.h>
#include <stdarg.h>

#include "s21_string.h"

// Цель рефакторинга - чистота и соответcтвие алгоритму:
// Шаг 1) - парсинг флагов.
// Шаг 2) - выбор обработчика (главным образом зависит от возвращаемого типа)
// Шаг 2*) - в обработчике реконфигурация флагов. Задание флагов по умолчанию
// Шаг 3) - формирование буферов для вывода согласно точност.
// Шаг 4) - обработка префиксов
// Шаг 5) - вывод

int s21_sprintf(char *str, const char *format, ...) {
  // init
  va_list args;
  va_start(args, format);
  int format_ptr = 0, counter = 0;
  while (format[format_ptr]) {
    if (format[format_ptr] != '%') {
      *str = format[format_ptr];
      str += 1;
      format_ptr++, counter++;
    } else {
      format_ptr++;
      s21_p_prot prot = parse_print_prot(format, &format_ptr, &args);
      // Integer
      if (s21_strchr("idoxuX", prot.specf)) {  // numbers in different types
        if (prot.add_specf == 'h') {
          counter += s21_print_short(&str, prot, (short int)va_arg(args, int));
        } else if (prot.add_specf == 'l') {
          counter += s21_print_long(&str, prot, va_arg(args, long));
        } else {
          counter += s21_print_int(&str, prot, va_arg(args, int));
        }
        // Float
      } else if (s21_strchr("eEfgG", prot.specf)) {
        if (prot.add_specf == 'L') {
          counter +=
              s21_print_long_double(&str, prot, va_arg(args, long double));
        } else {
          counter += s21_print_double(&str, prot, va_arg(args, double));
        }

      } else if (prot.specf == 'c' || prot.specf == '%') {
        char to_print[2] = {(prot.specf == '%') ? '%' : (char)va_arg(args, int),
                            '\0'};
        prot.precision = -1;
        counter += s21_print_str(&str, prot, to_print);
      } else if (prot.specf == 's') {
        counter += s21_print_str(&str, prot, va_arg(args, char *));
      } else if (prot.specf == 'p') {
        unsigned long ptr = va_arg(args, unsigned long);
        prot.show_hidden = 1;
        prot.specf = 'x';
        counter += s21_print_long(&str, prot, ptr);
      } else if (prot.specf == 'n') {
        *va_arg(args, int *) = counter;
      }
    }
  }
  *str = '\0';
  va_end(args);
  return counter;
}

s21_p_prot parse_print_prot(const char *format, int *format_ptr,
                            va_list *args) {
  s21_p_prot result = {0, 0, 0, 0, 0, -1, -1, '/', '/'};
  // flags
  while (s21_strchr("-+ #0", format[*format_ptr]) && format[*format_ptr]) {
    if (format[*format_ptr] == '-') result.alignment = 1;
    if (format[*format_ptr] == '+') result.show_plus = 1;
    if (format[*format_ptr] == ' ') result.blank = 1;
    if (format[*format_ptr] == '#') result.show_hidden = 1;
    if (format[*format_ptr] == '0') result.fill_w_zeros = 1;
    *format_ptr += 1;
  }
  // width
  if (format[*format_ptr] == '*') {
    result.width = va_arg(*args, int);
    *format_ptr += 1;
  }
  while (format[*format_ptr] >= 48 && format[*format_ptr] < 58 &&
         format[*format_ptr]) {
    if (result.width < 0) result.width = 0;
    result.width = result.width * 10 + (format[*format_ptr] - 48);
    *format_ptr += 1;
  }
  // precision
  if (format[*format_ptr] == '.') {
    result.precision = 0;
    *format_ptr += 1;
    if (format[*format_ptr] == '*') {
      result.precision = va_arg(*args, int);
      *format_ptr += 1;
    }
    while (format[*format_ptr] >= 48 && format[*format_ptr] < 58 &&
           format[*format_ptr]) {
      result.precision = result.precision * 10 + (format[*format_ptr] - 48);
      *format_ptr += 1;
    }
  }
  // specification
  if (s21_strchr("hlL", format[*format_ptr]) && format[*format_ptr] &&
      format[*format_ptr]) {
    result.add_specf = format[*format_ptr];
    *format_ptr += 1;
  }
  assert(s21_strchr("cdieEfgGosuxXpn%", format[*format_ptr]) &&
         format[*format_ptr] && format[*format_ptr]);
  result.specf = format[*format_ptr];
  *format_ptr += 1;

  return result;
}

//------------------------ Integers --------------------------
#define PR_INT(name, type)                                                    \
  int s21_print_##type(char **dest, s21_p_prot prot, type num) {              \
    /* Working with prefix */                                                 \
    char prefix[NUMERIC_BUFFER] = "\0";                                       \
    unsigned type unum = 0;                                                   \
    if (s21_strchr("xXuo", prot.specf)) {                                     \
      unum = (unsigned type)num;                                              \
    } else {                                                                  \
      unum = (num < 0) ? -num : num;                                          \
    }                                                                         \
    s21_make_prefix(prefix, prot, (int)(num < 0));                            \
    /* Choosing base */                                                       \
    int base = 10;                                                            \
    if (prot.specf == 'o') {                                                  \
      base = 8;                                                               \
    } else if ((prot.specf == 'x' || prot.specf == 'X')) {                    \
      base = 16;                                                              \
    }                                                                         \
    /* filling buffer with number */                                          \
    char buffer[NUMERIC_BUFFER];                                              \
    int ptr = NUMERIC_BUFFER - 1;                                             \
    buffer[ptr] = '\0';                                                       \
                                                                              \
    while (unum > 0) {                                                        \
      ptr--;                                                                  \
      buffer[ptr] = unum % base;                                              \
      unum = unum / base;                                                     \
      if (buffer[ptr] >= 10) {                                                \
        buffer[ptr] = buffer[ptr] + 55 + (int)((prot.specf == 'x') ? 32 : 0); \
      } else {                                                                \
        buffer[ptr] = buffer[ptr] + 48;                                       \
      }                                                                       \
    }                                                                         \
                                                                              \
    /* precision */                                                           \
    if (prot.precision == -1) {                                               \
      prot.precision = 1;                                                     \
    } else {                                                                  \
      prot.fill_w_zeros = 0;                                                  \
    }                                                                         \
    if (NUMERIC_BUFFER - 1 == ptr && s21_strchr("xXo", prot.specf))           \
      s21_strncpy(prefix, "\0", NUMERIC_BUFFER);                              \
    while (NUMERIC_BUFFER - 1 - ptr < prot.precision) {                       \
      ptr--;                                                                  \
      buffer[ptr] = '0';                                                      \
      if (prot.specf == 'o') s21_strncpy(prefix, "\0", NUMERIC_BUFFER);       \
    }                                                                         \
                                                                              \
    return s21_output_num(dest, prot, &buffer[ptr], prefix);                  \
  }

PR_INT(int, int);
PR_INT(long, long);
PR_INT(short, short);
void s21_make_prefix(char *prefix, s21_p_prot prot, int minus) {
  if (prot.show_hidden && (prot.specf == 'x' || prot.specf == 'X')) {
    s21_strncpy(prefix, (prot.specf == 'x') ? "0x" : "0X", 8);
  } else if (prot.show_hidden && prot.specf == 'o') {
    s21_strncpy(prefix, "0", 8);
  } else if (s21_strchr("ideEgGf", prot.specf) && !minus) {
    if (prot.show_plus) {
      s21_strncpy(prefix, "+", 8);
    } else if (prot.blank) {
      s21_strncpy(prefix, " ", 8);
    }
  } else if (s21_strchr("ideEgGf", prot.specf) && minus) {
    s21_strncpy(prefix, "-", 8);
  }
}

int s21_output_num(char **dest, s21_p_prot prot, char *buffer, char *prefix) {
  int blanks_before = 0;
  int blanks_after = 0;
  int num_of_ch = (int)(s21_strlen(prefix) + s21_strlen(buffer));
  int zeros = 0;
  if (prot.width > -1) {
    if (prot.alignment) {
      blanks_after = prot.width - num_of_ch - zeros;
    } else if (prot.fill_w_zeros) {
      zeros = MAX(prot.width - num_of_ch, zeros);
    } else {
      blanks_before = prot.width - num_of_ch - zeros;
    }
  }
  // printf("%d %ld %d %ld %d\n", blanks_before, strlen(prefix), zeros,
  // strlen(buffer), blanks_after);
  // printf("%s\n", prefix);

  int counter = 0;
  for (; blanks_before > 0; blanks_before--, *dest += 1, counter++)
    **dest = ' ';
  for (int i = 0; prefix[i]; i++, *dest += 1, counter++) **dest = prefix[i];
  for (; zeros > 0; zeros--, *dest += 1, counter++) **dest = '0';
  for (int i = 0; buffer[i]; i++, *dest += 1, counter++) **dest = buffer[i];
  for (; blanks_after > 0; blanks_after--, *dest += 1, counter++) **dest = ' ';
  return counter;
}

//------------------------ Float numbers --------------------------

#define PR_FLT(name, type)                                                  \
  int count_offset_##name(type a) {                                         \
    a = ABS(a);                                                             \
    int result = 0;                                                         \
    if (a < 1 && a != 0) {                                                  \
      while (a * pow(10, result) < 1) result++;                             \
    } else if (a >= 10) {                                                   \
      while (a >= pow(10, 1 - result)) result--;                            \
    }                                                                       \
    return -result;                                                         \
  }                                                                         \
                                                                            \
  int s21_print_##name(char **dest, s21_p_prot prot, type num) {            \
    if (prot.precision == -1) prot.precision = 6;                           \
    /* Working with prefix */                                               \
    char prefix[NUMERIC_BUFFER] = "\0";                                     \
    s21_make_prefix(prefix, prot, (int)(num < 0));                          \
                                                                            \
    /* choosing type*/                                                      \
    int use_offset = 0;                                                     \
    int offset = count_offset_##name(num);                                  \
    if (s21_strchr("eE", prot.specf) ||                                     \
        (s21_strchr("gG", prot.specf) &&                                    \
         (offset >= prot.precision || offset < -4) && offset != 0)) {       \
      use_offset = 1;                                                       \
      num *= pow(10, -offset);                                              \
    }                                                                       \
                                                                            \
    long double u_wh = ((num < 0) ? -num : num);                            \
    int float_num = prot.precision;                                         \
    if (s21_strchr("gG", prot.specf)) {                                     \
      float_num -= (!use_offset) ? MAX(offset + 1, 1) : 1;                  \
    }                                                                       \
    float_num = MAX(0, float_num);                                          \
    u_wh = round(u_wh * pow(10, float_num)) * pow(10, -float_num);          \
    unsigned long u_int = floor(u_wh);                                      \
    long double u_float = u_wh - u_int;                                     \
                                                                            \
    char int_buffer[NUMERIC_BUFFER];                                        \
    char *int_ptr = int_buffer;                                             \
    s21_print_long(&int_ptr, (s21_p_prot){0, 0, 0, 0, 0, -1, -1, 'l', 'u'}, \
                   u_int);                                                  \
    *int_ptr = '\0';                                                        \
                                                                            \
    char float_buffer[NUMERIC_BUFFER];                                      \
    char *float_ptr = float_buffer;                                         \
    if (float_num > 0) {                                                    \
      s21_print_long(&float_ptr,                                            \
                     (s21_p_prot){0, 0, 0, 0, 0, -1, float_num, 'l', 'u'},  \
                     round(u_float *pow(10, float_num)));                   \
    }                                                                       \
    *float_ptr = '\0';                                                      \
    if (s21_strchr("gG", prot.specf) && !prot.show_hidden) {                \
      while (float_ptr - 1 >= float_buffer && *(float_ptr - 1) == '0') {    \
        float_ptr--;                                                        \
        *float_ptr = '\0';                                                  \
      }                                                                     \
    }                                                                       \
                                                                            \
    char final_buffer[NUMERIC_BUFFER];                                      \
    s21_strncpy(final_buffer, int_buffer, NUMERIC_BUFFER);                  \
    if (s21_strlen(float_buffer) || prot.show_hidden)                       \
      s21_strncat(final_buffer, ".", 1);                                    \
    s21_strncat(final_buffer, float_buffer, NUMERIC_BUFFER);                \
    if (use_offset) {                                                       \
      char exp_buffer[NUMERIC_BUFFER];                                      \
      char *exp_ptr = exp_buffer;                                           \
      *exp_ptr = (prot.specf > 'a') ? 'e' : 'E';                            \
      exp_ptr += 1;                                                         \
      s21_print_int(&exp_ptr, (s21_p_prot){0, 1, 0, 0, 0, -1, 2, '/', 'd'}, \
                    offset);                                                \
      *exp_ptr = '\0';                                                      \
      s21_strncat(final_buffer, exp_buffer, NUMERIC_BUFFER);                \
    }                                                                       \
    return s21_output_num(dest, prot, final_buffer, prefix);                \
  }

PR_FLT(double, double)
PR_FLT(long_double, long double)

//------------------------ Strings --------------------------------

// Зарефакторить со стринговыми методами
int s21_print_str(char **dest, s21_p_prot prot, char *str) {
  if (str == S21_NULL) {
    str = "(null)";
  }
  int blanks_bef = 0;
  int zeros = 0;
  int num_of_ch = (int)s21_strlen(str);
#if !defined(__APPLE__) && !defined(__MACH__)
  prot.fill_w_zeros = 0;
#endif
  if (prot.precision != -1) {
    num_of_ch = MIN(num_of_ch, prot.precision);
  }
  int blanks_after = 0;
  if (prot.width > -1) {
    if (prot.alignment) {
      blanks_after = prot.width - num_of_ch - zeros;
    } else if (prot.fill_w_zeros) {
      zeros = MAX(prot.width - num_of_ch, zeros);
    } else {
      blanks_bef = prot.width - num_of_ch - zeros;
    }
  }

  int counter = 0;
  for (; blanks_bef > 0; blanks_bef--, *dest += 1, counter++) **dest = ' ';
  for (; zeros > 0; zeros--, *dest += 1, counter++) **dest = '0';
  for (; num_of_ch > 0; num_of_ch--, *dest += 1, str += 1, counter++)
    **dest = *str;
  for (; blanks_after > 0; blanks_after--, *dest += 1, counter++) **dest = ' ';
  return counter;
}