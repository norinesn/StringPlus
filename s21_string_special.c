#include <stdlib.h>

#include "s21_string.h"

void *s21_to_lower(const char *str) {
  char *out = S21_NULL;
  if (str != S21_NULL) {  //строка есть
    s21_size_t lenght = s21_strlen(str);
    out = (char *)malloc((lenght + 1) * sizeof(char));
    if (out != S21_NULL) {  //память выделена
      for (s21_size_t i = 0; i <= lenght; i++) {
        if (str[i] >= 'A' && str[i] <= 'Z') {
          out[i] = (str[i] - 'A') + 'a';
        } else {
          out[i] = str[i];
        }
      }
      out[lenght] = '\0';
    }
  }
  return out;
}

void *s21_to_upper(const char *str) {
  char *out = S21_NULL;
  if (str != S21_NULL) {  //строка есть
    s21_size_t lenght = s21_strlen(str);
    out = (char *)malloc((lenght + 1) * sizeof(char));
    if (out != S21_NULL) {  //память выделена
      for (s21_size_t i = 0; i <= lenght; i++) {
        if (str[i] >= 'a' && str[i] <= 'z') {
          out[i] = (str[i] - 'a') + 'A';
        } else {
          out[i] = str[i];
        }
      }
      out[lenght] = '\0';
    }
  }
  return out;
}

void *s21_insert(const char *src, const char *str, s21_size_t start_index) {
  if (!src || !str) {
    return S21_NULL;
  }

  s21_size_t src_len = s21_strlen(src);
  s21_size_t str_len = s21_strlen(str);
  s21_size_t whole_len = src_len + str_len;

  if (start_index > src_len) {
    return S21_NULL;
  }

  char *result_str = (char *)malloc((whole_len + 1) * sizeof(char));

  if (!result_str) {
    return S21_NULL;  // error message?
  }

  s21_size_t i;
  s21_size_t j;
  s21_size_t pos = 0;

  for (i = 0; i < src_len; i++) {
    if (i == start_index) {
      for (j = 0; j < str_len; j++) {
        result_str[pos++] = str[j];
      }
    }

    result_str[pos++] = src[i];
  }

  if (start_index == src_len) {
    for (j = 0; j < str_len; j++) {
      result_str[pos++] = str[j];
    }
  }

  result_str[whole_len] = '\0';

  return (void *)result_str;
}

void *s21_trim(const char *src, const char *trim_chars) {
  if (!src || !trim_chars) {
    return S21_NULL;
  }

  s21_size_t src_len = s21_strlen(src);
  s21_size_t trim_len = s21_strlen(trim_chars);

  // #1 count junk on the end:
  s21_size_t junk_end_count = 0;
  int found_junk = 0;
  for (s21_size_t i = src_len - 1; i > 0; i--) {
    for (size_t j = 0; j < trim_len; j++) {
      if (src[i] == trim_chars[j]) {
        found_junk = 1;
        junk_end_count++;
        break;
      } else {
        found_junk = 0;
      }
    }
    if (!found_junk) break;
  }

  // #2 count junk on the start:
  s21_size_t junk_start_count = 0;
  found_junk = 0;
  for (s21_size_t i = 0; i < src_len; i++) {
    for (size_t j = 0; j < trim_len; j++) {
      if (src[i] == trim_chars[j]) {
        found_junk = 1;
        junk_start_count++;
        break;
      } else {
        found_junk = 0;
      }
    }
    if (!found_junk) break;
  }

  // printf("   source length : %ld\n", src_len);
  // printf("junk on the start: %ld\n", junk_start_count);
  // printf("  junk on the end: %ld\n", junk_end_count);

  int all_junk = 0;
  if (src_len == junk_start_count || src_len == junk_end_count) {
    all_junk = 1;
  }

  s21_size_t result_len;

  if (!all_junk) {
    result_len = src_len - (junk_start_count + junk_end_count);
  }

  char *result_str;

  if (all_junk) {
    result_str = (char *)malloc(1 * sizeof(char));
  } else {
    result_str = (char *)malloc((result_len + 1) * sizeof(char));
  }

  if (!result_str) {
    return S21_NULL;
  }

  if (!all_junk) {
    s21_size_t pos = 0;
    for (s21_size_t i = junk_start_count; i < src_len - junk_end_count; i++) {
      result_str[pos++] = src[i];
    }

    result_str[pos] = '\0';
  } else {
    result_str[0] = '\0';
  }

  return (void *)result_str;
}