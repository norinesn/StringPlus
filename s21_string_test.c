#include "s21_string.h"

#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#define PRECISION 1E-06

START_TEST(s21_strlen_test) {
  // Test case 1: non-empty string
  const char *str1 = "hello school21!";
  s21_size_t expected_length1 = strlen(str1);
  s21_size_t actual_length1 = s21_strlen(str1);
  ck_assert_int_eq(expected_length1, actual_length1);

  // Test case 2: empty string
  const char *str2 = "";
  size_t expected_length2 = strlen(str2);
  size_t actual_length2 = s21_strlen(str2);
  ck_assert_int_eq(expected_length2, actual_length2);

  // Test case 3: string with null terminator.
  const char *str3 = "Hello\0ExtraCharacters";
  size_t expected_length3 = strlen(str3);
  size_t actual_length3 = s21_strlen(str3);
  ck_assert_int_eq(expected_length3, actual_length3);

  // Test case 4: very long string
  char very_long_str[1000];
  memset(very_long_str, 'a', sizeof(very_long_str) - 1);
  very_long_str[sizeof(very_long_str) - 1] = '\0';
  size_t expected_length4 = strlen(very_long_str);
  size_t actual_length4 = s21_strlen(very_long_str);
  ck_assert_int_eq(expected_length4, actual_length4);

  // Test case 5: unicode char
  const char *unicode_str = "Café";
  size_t expected_length5 = strlen(unicode_str);
  size_t actual_length5 = s21_strlen(unicode_str);
  ck_assert_int_eq(expected_length5, actual_length5);

  // Test case 6: leading/trailing whitespaces
  const char *whitespace_str = "   Trimmed   ";
  size_t expected_length6 = strlen(whitespace_str);
  size_t actual_length6 = s21_strlen(whitespace_str);
  ck_assert_int_eq(expected_length6, actual_length6);
}
END_TEST

START_TEST(s21_strerror_test) {
  for (int i = -5; i < 150; i++) {
    ck_assert_str_eq(s21_strerror(i), strerror(i));
  }
}
END_TEST

START_TEST(s21_strchr_test) {
  // Test case 1: present
  const char *str_present = "hello school21!";
  char ch_present = 'e';
  char *expected_result1 = strchr(str_present, ch_present);
  char *actual_result1 = s21_strchr(str_present, ch_present);
  ck_assert_ptr_eq(expected_result1, actual_result1);

  // Test case 2: not-present
  const char *str_not_present = "hello school21!";
  char ch_not_present = 'x';
  char *expected_result2 = strchr(str_not_present, ch_not_present);
  char *actual_result2 = s21_strchr(str_not_present, ch_not_present);
  ck_assert_ptr_eq(expected_result2, actual_result2);

  // Test case 3: \0 char
  const char *str_null = "hello school21!";
  char ch_null = '\0';
  char *expected_result3 = strchr(str_null, ch_null);
  char *actual_result3 = s21_strchr(str_null, ch_null);
  ck_assert_ptr_eq(expected_result3, actual_result3);

  // Test case 4: empty string
  const char *str_empty = "";
  char ch = 'a';
  char *expected_result4 = strchr(str_empty, ch);
  char *actual_result4 = s21_strchr(str_empty, ch);
  ck_assert_ptr_eq(expected_result4, actual_result4);

  // Test case 5: non-ascii string
  const char *str_non_ascii = "hello €";
  wchar_t ch_non_ascii = 0x20AC;
  char *expected_result5 = strchr(str_non_ascii, (char)ch_non_ascii);
  char *actual_result5 = s21_strchr(str_non_ascii, (char)ch_non_ascii);
  ck_assert_ptr_eq(expected_result5, actual_result5);
}
END_TEST

START_TEST(s21_strncpy_test) {
  char dest_custom[50];
  char dest_original[50];

  for (int i = 0; i < 50; i++) {
    dest_custom[i] = '\0';
    dest_original[i] = '\0';
  }

  // Test case 1: copy full string:
  const char *src1 = "Hello, school21!";
  s21_size_t n1 = 20;
  s21_strncpy(dest_custom, src1, n1);
  strncpy(dest_original, src1, n1);
  ck_assert_str_eq(dest_original, dest_custom);

  // Test case 2: string is bigger than need to copy
  const char *src2 = "Hello, school21! This is a test string.";
  s21_size_t n2 = 20;
  s21_strncpy(dest_custom, src2, n2);
  strncpy(dest_original, src2, n2);
  ck_assert_str_eq(dest_original, dest_custom);

  // Test case 3: copy an empty string
  const char *src3 = "";
  s21_size_t n3 = 10;
  s21_strncpy(dest_custom, src3, n3);
  strncpy(dest_original, src3, n3);
  ck_assert_str_eq(dest_original, dest_custom);

  // Test case 4: copy length of 0
  const char *src4 = "Hello school!";
  s21_size_t n4 = 0;
  s21_strncpy(dest_custom, src4, n4);
  strncpy(dest_original, src4, n4);
  ck_assert_str_eq(dest_original, dest_custom);

  // Test case 5: copy string with non-ascii char
  const char *src5 = "hello €school!";
  s21_size_t n5 = 10;
  s21_strncpy(dest_custom, src5, n5);
  strncpy(dest_original, src5, n5);
  ck_assert_str_eq(dest_original, dest_custom);
}
END_TEST

START_TEST(s21_memchr_test) {
  // Test case 1: Basic comparison
  char str1[] = "Hello, School21!";
  void *actual_result1 = s21_memchr(str1, 'S', strlen(str1));
  void *expected_result1 = memchr(str1, 'S', strlen(str1));
  ck_assert_ptr_eq(expected_result1, actual_result1);

  // Test case 2: Char not in a string
  char str2[] = "Hello, School21!";
  void *actual_result2 = s21_memchr(str2, 'X', strlen(str2));
  void *expected_result2 = memchr(str2, 'X', strlen(str2));
  ck_assert_ptr_eq(expected_result2, actual_result2);

  // Test case 3: long string
  char str3[200];
  memset(str3, 'A', sizeof(str3) - 1);
  str3[100] = 'B';
  void *actual_result3 = s21_memchr(str3, 'B', sizeof(str3) - 1);
  void *expected_result3 = memchr(str3, 'B', sizeof(str3) - 1);
  ck_assert_ptr_eq(expected_result3, actual_result3);

  // Test case 4: empty string
  char str4[] = "";
  void *actual_result4 = s21_memchr(str4, 'A', 0);
  void *expected_result4 = memchr(str4, 'A', 0);
  ck_assert_ptr_eq(expected_result4, actual_result4);

  // Test case 5:  null terminator in a non-empty string
  char str5[] = "Hello\0World";
  void *actual_result5 = s21_memchr(str5, '\0', strlen(str5));
  void *expected_result5 = memchr(str5, '\0', strlen(str5));
  ck_assert_ptr_eq(expected_result5, actual_result5);
}
END_TEST

START_TEST(s21_memcmp_test) {
  // Test case 1: equal strings
  char str1_equal[] = "Hello";
  char str2_equal[] = "Hello";
  int actual_result1 = s21_memcmp(str1_equal, str2_equal, 5);
  int expected_result1 = memcmp(str1_equal, str2_equal, 5);
  ck_assert_int_eq(expected_result1, actual_result1);

  // Test case 2: first less than second
  char str1_lessthan[] = "Hello";
  char str2_lessthan[] = "Hemlo";
  int actual_result2 = s21_memcmp(str1_lessthan, str2_lessthan, 5);
  int expected_result2 = memcmp(str1_lessthan, str2_lessthan, 5);
  ck_assert_int_eq(expected_result2, actual_result2);

  // Test case 3: first greater than second
  char str1_greaterthan[] = "Hello";
  char str2_greaterthan[] = "Hdllo";
  int actual_result3 = s21_memcmp(str1_greaterthan, str2_greaterthan, 5);
  int expected_result3 = memcmp(str1_greaterthan, str2_greaterthan, 5);
  ck_assert_int_eq(expected_result3, actual_result3);

  // Test case 4: different str len
  char str1_longer[] = "Hello School!";
  char str2_shorter[] = "Hello";
  int actual_result4 = s21_memcmp(str1_longer, str2_shorter, 5);
  int expected_result4 = memcmp(str1_longer, str2_shorter, 5);
  ck_assert_int_eq(expected_result4, actual_result4);

  // Test case 5: special character test
  char str1_special[] = "Hello ";  // ?
  char str2_normal[] = "Hello ";
  int actual_result5 = s21_memcmp(str1_special, str2_normal, 7);
  int expected_result5 = memcmp(str1_special, str2_normal, 7);
  ck_assert_int_eq(expected_result5, actual_result5);

  // Test case 6: compare empty strings
  char str1_empty[] = "";
  char str2_empty[] = "";
  int actual_result6 = s21_memcmp(str1_empty, str2_empty, 0);
  int expected_result6 = memcmp(str1_empty, str2_empty, 0);
  ck_assert_int_eq(expected_result6, actual_result6);

  // Test case 7: test with an \0 in a middle
  char str1_null_middle[] = "hello\0school";
  char str2_null_middle[] = "hello\0school";
  int actual_result7 = s21_memcmp(str1_null_middle, str2_null_middle, 12);
  int expected_result7 = memcmp(str1_null_middle, str2_null_middle, 12);
  ck_assert_int_eq(expected_result7, actual_result7);
}
END_TEST

START_TEST(s21_memcpy_test) {
  char dest_custom[50];
  char dest_original[50];

  // not necessary here
  for (int i = 0; i < 50; i++) {
    dest_custom[i] = '\0';
    dest_original[i] = '\0';
  }

  // Test case 1: copy string
  char str1[] = "Hello, school!";
  memcpy(dest_custom, str1, 15);
  s21_memcpy(dest_original, str1, 15);
  // ck_assert_str_eq(dest_original, dest_custom);
  ck_assert_mem_eq(dest_original, dest_custom, 15 * sizeof(char));

  // Test case 2: copy empty string
  char str2[] = "";
  memcpy(dest_custom, str2, 1);
  s21_memcpy(dest_original, str2, 1);
  // ck_assert_str_eq(dest_original, dest_custom);
  ck_assert_mem_eq(dest_original, dest_custom, 1 * sizeof(char));

  // Test case 3: string with \0 in a middle
  char str3[] = "Hello\0school!";
  memcpy(dest_custom, str3, 13);
  s21_memcpy(dest_original, str3, 13);
  ck_assert_mem_eq(dest_original, dest_custom, 13 * sizeof(char));

  // Test case 4: copy an array of ints.
  int src_arr_ints[] = {1, 2, 3, 4, 5};
  int custom_int_result[5];
  int original_int_result[5];
  memcpy(original_int_result, src_arr_ints, 5 * sizeof(int));
  s21_memcpy(custom_int_result, src_arr_ints, 5 * sizeof(int));
  ck_assert_mem_eq(original_int_result, custom_int_result, 5 * sizeof(int));

  // Test case 5: copy an array of floats.
  float src_array_floats[] = {5.1, 2.2, 3.3, 4.4, 5.5};
  float custom_float_result[5];
  float original_float_result[5];
  memcpy(original_float_result, src_array_floats, 5 * sizeof(float));
  s21_memcpy(custom_float_result, src_array_floats, 5 * sizeof(float));
  ck_assert_mem_eq(original_float_result, custom_float_result,
                   5 * sizeof(float));
}
END_TEST

START_TEST(s21_memset_test) {
  char char_buffer_custom[50];
  char char_buffer_original[50];

  // Test case 1: copy string
  memset(char_buffer_original, 's', 17);
  s21_memset(char_buffer_custom, 's', 17);
  ck_assert_mem_eq(char_buffer_original, char_buffer_custom, 17 * sizeof(char));

  // Test case 2: specific pattern
  memset(char_buffer_original, 0xAA, 20);
  s21_memset(char_buffer_custom, 0xAA, 20);
  ck_assert_mem_eq(char_buffer_original, char_buffer_custom, 20 * sizeof(char));

  // Test case 3: pattern with interval
  memset(char_buffer_original, 'A', 10);
  memset(char_buffer_original + 10, 'B', 15);
  s21_memset(char_buffer_custom, 'A', 10);
  s21_memset(char_buffer_custom + 10, 'B', 15);
  ck_assert_mem_eq(char_buffer_original, char_buffer_custom, 25 * sizeof(char));

  // Test case 4: memset array of ints
  int int_buffer_custom[10];
  int int_buffer_original[10];
  memset(int_buffer_original, 7, 10 * sizeof(int));
  s21_memset(int_buffer_custom, 7, 10 * sizeof(int));
  ck_assert_mem_eq(int_buffer_original, int_buffer_custom, 10 * sizeof(int));
}
END_TEST

START_TEST(s21_strncmp_test) {
  // Test case 1: equal strings
  char str1_equal[] = "Hello";
  char str2_equal[] = "Hello";
  int actual_result1 = s21_strncmp(str1_equal, str2_equal, 5);
  int expected_result1 = strncmp(str1_equal, str2_equal, 5);
  ck_assert_int_eq(expected_result1, actual_result1);

  // Test case 2: first less than second
  char str1_lessthan[] = "Hello";
  char str2_lessthan[] = "Hemlo";
  int actual_result2 = s21_strncmp(str1_lessthan, str2_lessthan, 5);
  int expected_result2 = strncmp(str1_lessthan, str2_lessthan, 5);
  ck_assert_int_eq(expected_result2, actual_result2);

  // Test case 3: first greater than second
  char str1_greaterthan[] = "Hello";
  char str2_greaterthan[] = "Hdllo";
  int actual_result3 = s21_strncmp(str1_greaterthan, str2_greaterthan, 5);
  int expected_result3 = strncmp(str1_greaterthan, str2_greaterthan, 5);
  ck_assert_int_eq(expected_result3, actual_result3);

  // Test case 4: different str len
  char str1_longer[] = "Hello School!";
  char str2_shorter[] = "Hello";
  int actual_result4 = s21_strncmp(str1_longer, str2_shorter, 5);
  int expected_result4 = strncmp(str1_longer, str2_shorter, 5);
  ck_assert_int_eq(expected_result4, actual_result4);

  // Test case 5: special character test
  char str1_special[] = "Hello  ";
  char str2_normal[] = "Hello  ";
  int actual_result5 = s21_strncmp(str1_special, str2_normal, 7);
  int expected_result5 = strncmp(str1_special, str2_normal, 7);
  ck_assert_int_eq(expected_result5, actual_result5);

  // Test case 6: compare empty strings
  char str1_empty[] = "";
  char str2_empty[] = "";
  int actual_result6 = s21_strncmp(str1_empty, str2_normal, 0);
  int expected_result6 = strncmp(str2_empty, str2_normal, 0);
  ck_assert_int_eq(expected_result6, actual_result6);

  // Test case 7: test with an \0 in a middle
  char str1_null_middle[] = "hello\0school";
  char str2_null_middle[] = "hello\0school";
  int actual_result7 = s21_strncmp(str1_null_middle, str2_null_middle, 12);
  int expected_result7 = strncmp(str1_null_middle, str2_null_middle, 12);
  ck_assert_int_eq(expected_result7, actual_result7);
}
END_TEST

START_TEST(s21_strncat_test) {
  // Test case 1: append entire src
  char dest_original1[20] = "Hello, ";
  char dest_custom1[20] = "Hello, ";
  const char *src1 = "School!";
  char *actual_result1 = s21_strncat(dest_custom1, src1, 20);
  char *expected_result1 = strncat(dest_original1, src1, 20);
  ck_assert_str_eq(expected_result1, actual_result1);

  // Test case 2: append part of a src
  char dest_original2[20] = "Hello, ";
  char dest_custom2[20] = "Hello, ";
  const char *src2 = "School!";
  char *actual_result2 = s21_strncat(dest_custom2, src2, 3);
  char *expected_result2 = strncat(dest_original2, src2, 3);
  ck_assert_str_eq(expected_result2, actual_result2);

  // Test case 3: append nothing
  char dest_original3[20] = "Hello, ";
  char dest_custom3[20] = "Hello, ";
  const char *src3 = "World!";
  char *actual_result3 = s21_strncat(dest_custom3, src3, 0);
  char *expected_result3 = strncat(dest_original3, src3, 0);
  ck_assert_str_eq(expected_result3, actual_result3);

  // Test case 4: append with empty src
  char dest_original4[20] = "Hello, ";
  char dest_custom4[20] = "Hello, ";
  const char *src4 = "";
  char *actual_result4 = s21_strncat(dest_custom4, src4, 5);
  char *expected_result4 = strncat(dest_original4, src4, 5);
  ck_assert_str_eq(expected_result4, actual_result4);

  // Test case 5: append to empty dest
  char dest_original5[20] = "";
  char dest_custom5[20] = "";
  const char *src5 = "Check!";
  char *actual_result5 = s21_strncat(dest_custom5, src5, 5);
  char *expected_result5 = strncat(dest_original5, src5, 5);
  ck_assert_str_eq(expected_result5, actual_result5);

  // Test case 6: append long string
  char dest_original6[21] = "The quick ";
  char dest_custom6[21] = "The quick ";
  const char *src6 = "brown fox jumps over the lazy dog!";
  char *actual_result6 = s21_strncat(dest_custom6, src6, 10);
  char *expected_result6 = strncat(dest_original6, src6, 10);
  ck_assert_str_eq(expected_result6, actual_result6);
}
END_TEST

START_TEST(s21_strcspn_test) {
  // Test case 1: no matching
  const char *str1 = "abcdef";
  const char *str2 = "xyz";
  s21_size_t actual_result1 = s21_strcspn(str1, str2);
  s21_size_t expected_result1 = strcspn(str1, str2);
  ck_assert_int_eq(expected_result1, actual_result1);

  // Test case 2: first string is empty
  const char *str3 = "";
  const char *str4 = "xyz";
  s21_size_t actual_result2 = s21_strcspn(str3, str4);
  s21_size_t expected_result2 = strcspn(str3, str4);
  ck_assert_int_eq(expected_result2, actual_result2);

  // Test case 3: second string is empty
  const char *str5 = "abcdef";
  const char *str6 = "";
  s21_size_t actual_result3 = s21_strcspn(str5, str6);
  s21_size_t expected_result3 = strcspn(str5, str6);
  ck_assert_int_eq(expected_result3, actual_result3);

  // Test case 4: middle
  const char *str7 = "school21cool";
  const char *str8 = "21";
  s21_size_t actual_result4 = s21_strcspn(str7, str8);
  s21_size_t expected_result4 = strcspn(str7, str8);
  ck_assert_int_eq(expected_result4, actual_result4);

  // Test case 5: identical
  const char *str9 = "abcdef";
  const char *str10 = "abcdef";
  s21_size_t actual_result5 = s21_strcspn(str9, str10);
  s21_size_t expected_result5 = strcspn(str9, str10);
  ck_assert_int_eq(expected_result5, actual_result5);
}
END_TEST

START_TEST(s21_strpbrk_test) {
  // Test case 1: found in a middle
  const char *str1 = "hello";
  const char *str2 = "world";
  char *actual_result1 = s21_strpbrk(str1, str2);
  char *expected_result1 = strpbrk(str1, str2);
  ck_assert_ptr_eq(expected_result1, actual_result1);

  // Test case 2: first string is emtpy
  const char *str3 = "";
  const char *str4 = "world";
  char *actual_result2 = s21_strpbrk(str3, str4);
  char *expected_result2 = strpbrk(str3, str4);
  ck_assert_ptr_eq(expected_result2, actual_result2);

  // Test case 3: second string empty
  const char *str5 = "hello";
  const char *str6 = "";
  char *actual_result3 = s21_strpbrk(str5, str6);
  char *expected_result3 = strpbrk(str5, str6);
  ck_assert_ptr_eq(expected_result3, actual_result3);

  // Test case 4: both strings empty
  const char *str7 = "";
  const char *str8 = "";
  char *actual_result4 = s21_strpbrk(str7, str8);
  char *expected_result4 = strpbrk(str7, str8);
  ck_assert_ptr_eq(expected_result4, actual_result4);

  // Test case 5: \0 in the middle, looking for \0
  const char *str9 = "hello\0school21";
  const char *str10 = "\0";
  char *actual_result5 = s21_strpbrk(str9, str10);
  char *expected_result5 = strpbrk(str9, str10);
  ck_assert_ptr_eq(expected_result5, actual_result5);
}
END_TEST

// FIXED
START_TEST(s21_strrchr_test) {
  // Test case 1: finding last dot
  const char *str1 = "https://www.school21.com";
  const char ch1 = '.';
  char *actual_result1 = s21_strrchr(str1, ch1);
  char *expected_result1 = strrchr(str1, ch1);
  ck_assert_ptr_eq(expected_result1, actual_result1);

  // Test case 2: find the char that not present in a string
  const char *str2 = "ABCDEF";
  const char ch2 = 'Z';
  char *actual_result2 = s21_strrchr(str2, ch2);
  char *expected_result2 = strrchr(str2, ch2);
  ck_assert_ptr_eq(expected_result2, actual_result2);

  // Test case 3: special characters
  const char *str3 = "!@#$%^&*()_+ ";
  const char ch3 = '$';
  char *actual_result3 = s21_strrchr(str3, ch3);
  char *expected_result3 = strrchr(str3, ch3);
  ck_assert_ptr_eq(expected_result3, actual_result3);

  // Test case 4: search in empty string
  const char *str4 = "";
  const char ch4 = 'A';
  char *actual_result4 = s21_strrchr(str4, ch4);
  char *expected_result4 = strrchr(str4, ch4);
  ck_assert_ptr_eq(expected_result4, actual_result4);

  // Test case 5: single char in a string
  const char *str5 = "x";
  const char ch5 = 'x';
  char *actual_result5 = s21_strrchr(str5, ch5);
  char *expected_result5 = strrchr(str5, ch5);
  ck_assert_ptr_eq(expected_result5, actual_result5);

  // Test case 6: NEW case \0 as second param
  const char *str6 = "hello school21!";
  const char ch6 = '\0';
  char *actual_result6 = s21_strrchr(str6, ch6);
  char *expected_result6 = strrchr(str6, ch6);
  ck_assert_ptr_eq(expected_result6, actual_result6);
}
END_TEST

// FIXED
START_TEST(s21_strstr_test) {
  // Test case 1: basic
  const char *haystack1 = "hello school21!";
  const char *needle1 = "school";
  char *actual_result1 = s21_strstr(haystack1, needle1);
  char *expected_result1 = strstr(haystack1, needle1);
  ck_assert_ptr_eq(expected_result1, actual_result1);

  // Test case2: repeated pattern of needle
  const char *haystack2 = "we open new campus in mississippi";
  const char *needle2 = "issi";
  char *actual_result2 = s21_strstr(haystack2, needle2);
  char *expected_result2 = strstr(haystack2, needle2);
  ck_assert_ptr_eq(expected_result2, actual_result2);

  // Test case 3: needle not in a haystack
  const char *haystack3 = "abcdefg";
  const char *needle3 = "xyz";
  char *actual_result3 = s21_strstr(haystack3, needle3);
  char *expected_result3 = strstr(haystack3, needle3);
  ck_assert_ptr_eq(expected_result3, actual_result3);

  // Test case 4: needle not in a haystack
  const char *haystack4 = "aaaaaaaaaab";
  const char *needle4 = "aaab";
  char *actual_result4 = s21_strstr(haystack4, needle4);
  char *expected_result4 = strstr(haystack4, needle4);
  ck_assert_ptr_eq(expected_result4, actual_result4);

  // Test case 5: repeating string case
  const char *haystack5 = "abcdabcdabcd";
  const char *needle5 = "abcd";
  char *actual_result5 = s21_strstr(haystack5, needle5);
  char *expected_result5 = strstr(haystack5, needle5);
  ck_assert_ptr_eq(expected_result5, actual_result5);

  // Test case 6: NEW case needle is longer than haystack
  const char *haystack6 = "ab";
  const char *needle6 = "abcd";
  char *actual_result6 = s21_strstr(haystack6, needle6);
  char *expected_result6 = strstr(haystack6, needle6);
  ck_assert_ptr_eq(expected_result6, actual_result6);
}
END_TEST

START_TEST(s21_to_lower_test) {
  // Test case 1: all uppercase source
  const char *src1 = "HELLO SCHOOL";
  char *actual_result1 = s21_to_lower(src1);
  ck_assert_str_eq("hello school", actual_result1);

  // Test case 2: uppercase / lowercase / symbols in source
  const char *src2 = "H@llo $cHoOl21!";
  char *actual_result2 = s21_to_lower(src2);
  ck_assert_str_eq("h@llo $chool21!", actual_result2);

  // Test case 3: empty source
  const char *src3 = "";
  char *actual_result3 = s21_to_lower(src3);
  ck_assert_str_eq("", actual_result3);

  // Test case 4: special characters only
  const char *src4 = "!@# $%^ &*()";
  char *actual_result4 = s21_to_lower(src4);
  ck_assert_str_eq("!@# $%^ &*()", actual_result4);

  free(actual_result1);
  free(actual_result2);
  free(actual_result3);
  free(actual_result4);
}
END_TEST;

START_TEST(s21_to_upper_test) {
  // Test case 1: all lowercase source
  const char *src1 = "i love strings";
  char *actual_result1 = s21_to_upper(src1);
  ck_assert_str_eq("I LOVE STRINGS", actual_result1);

  // Test case 2: uppercase / lowercase / symbols in source
  const char *src2 = "H@llo $cHoOl21!";
  char *actual_result2 = s21_to_upper(src2);
  ck_assert_str_eq("H@LLO $CHOOL21!", actual_result2);

  // Test case 3: empty source
  const char *src3 = "";
  char *actual_result3 = s21_to_upper(src3);
  ck_assert_str_eq("", actual_result3);

  // Test case 4: special characters only
  const char *src4 = "!@# $%^ &*()";
  char *actual_result4 = s21_to_upper(src4);
  ck_assert_str_eq("!@# $%^ &*()", actual_result4);

  free(actual_result1);
  free(actual_result2);
  free(actual_result3);
  free(actual_result4);
}
END_TEST;

// FIXED
START_TEST(s21_strtok_test) {
  char str[] = "School-21";
  char *tok1 = s21_strtok(str, "-");
  char *tok2 = strtok(str, "-");
  char *tok3 = s21_strtok(str, "");
  char *tok4 = strtok(str, "");
  char *tok5 = s21_strtok(str, "oo");
  char *tok6 = strtok(str, "oo");
  char *tok7 = s21_strtok(str, "Sc");
  char *tok8 = strtok(str, "Sc");
  char *tok9 = s21_strtok(str, "21");
  char *tok10 = strtok(str, "21");

  ck_assert_pstr_eq(tok1, tok2);
  ck_assert_pstr_eq(tok3, tok4);
  ck_assert_pstr_eq(tok5, tok6);
  ck_assert_pstr_eq(tok7, tok8);
  ck_assert_pstr_eq(tok9, tok10);

  char arr1[100] = "My name is Root. Hello world!", *parr1 = arr1,
       arr2[100] = "My name is Root. Hello world!", *parr2 = arr2;
  s21_strtok(parr1, " .");
  strtok(parr2, " .");
  ck_assert_str_eq(parr1, parr2);

  parr1 = s21_strtok(NULL, " .");
  parr2 = strtok(NULL, " .");
  ck_assert_str_eq(parr1, parr2);

  parr1 = s21_strtok(parr1, "x");
  parr2 = strtok(parr2, "x");
  ck_assert_str_eq(parr1, parr2);
}
END_TEST

// FIXED
START_TEST(s21_insert_test) {
  // Test case 1: insert in a middle of a string
  const char *src1 = "i love strings";
  const char *str1 = "hate";
  s21_size_t start_idx1 = 6;
  char *actual_result1 = s21_insert(src1, str1, start_idx1);
  ck_assert_str_eq("i lovehate strings", actual_result1);

  // // Test case 2: prepend
  const char *src2 = "i love strings";
  const char *str2 = "YES, ";
  s21_size_t start_idx2 = 0;
  char *actual_result2 = s21_insert(src2, str2, start_idx2);
  ck_assert_str_eq("YES, i love strings", actual_result2);

  // Test case 3: append
  const char *src3 = "i love strings";
  const char *str3 = ", and i mean it!";
  s21_size_t start_idx3 = 14;
  char *actual_result3 = s21_insert(src3, str3, start_idx3);
  ck_assert_str_eq("i love strings, and i mean it!", actual_result3);

  // Test case 4: overlapping insert
  const char *src4 = "abcdef";
  const char *str4 = "XYZ";
  s21_size_t start_idx4 = 3;
  char *actual_result4 = s21_insert(src4, str4, start_idx4);
  ck_assert_str_eq("abcXYZdef", actual_result4);

  // Test case 5: empty source string
  const char *src5 = "";
  const char *str5 = "hello";
  s21_size_t start_idx5 = 0;
  char *actual_result5 = s21_insert(src5, str5, start_idx5);
  ck_assert_str_eq("hello", actual_result5);

  // Test case 6: insert index beyond bounds
  const char *src6 = "hello";
  const char *str6 = "school21!";
  s21_size_t start_idx6 = 8;
  char *actual_result6 = s21_insert(src6, str6, start_idx6);
  ck_assert_ptr_null(actual_result6);

  // Test case 7: NEW case
  const char *src7 = NULL;
  const char *str7 = NULL;
  s21_size_t start_idx7 = 0;
  char *actual_result7 = s21_insert(src7, str7, start_idx7);
  ck_assert_ptr_null(actual_result7);

  free(actual_result1);
  free(actual_result2);
  free(actual_result3);
  free(actual_result4);
  free(actual_result5);
  // free(actual_result6);
  free(actual_result7);
}
END_TEST;

// FIXED
START_TEST(s21_trim_test) {
  // Test case 1: trim whitespaces
  const char *src1 = "    hello school21!    ";
  const char *trim_chars1 = " ";
  char *actual_result1 = s21_trim(src1, trim_chars1);
  ck_assert_str_eq("hello school21!", actual_result1);

  // Test case 2: trim from start/end + trimmed char in a middle
  const char *src2 = "@#^__hello_$chool21!^#@";
  const char *trim_chars2 = "_@#^";
  char *actual_result2 = s21_trim(src2, trim_chars2);
  ck_assert_str_eq("hello_$chool21!", actual_result2);

  // Test case 3: pass empty trim string
  const char *src3 = " _hello school21!_ ";
  const char *trim_chars3 = "";
  char *actual_result3 = s21_trim(src3, trim_chars3);
  ck_assert_str_eq(" _hello school21!_ ", actual_result3);

  // Test case 4: pass empty src
  const char *src4 = "";
  const char *trim_chars4 = "!@#$%";
  char *actual_result4 = s21_trim(src4, trim_chars4);
  ck_assert_str_eq("", actual_result4);

  // Test case 5: trim non-existent character
  const char *src5 = "hello!";
  const char *trim_chars5 = "#";
  char *actual_result5 = s21_trim(src5, trim_chars5);
  ck_assert_str_eq("hello!", actual_result5);

  // Test case 6: NEW CASE trim all junk
  const char *src6 = "!!!__!!!";
  const char *trim_chars6 = "!_";
  char *actual_result6 = s21_trim(src6, trim_chars6);
  ck_assert_str_eq("", actual_result6);

  free(actual_result1);
  free(actual_result2);
  free(actual_result3);
  free(actual_result4);
  free(actual_result5);
  free(actual_result6);
}
END_TEST;

START_TEST(s21_sprintf_test_integers) {
  char dest_custom[50];
  char dest_original[50];
  char format_buffer[50];
  for (int i = 0; i < 50; i++) {
    dest_custom[i] = '\0';
    dest_original[i] = '\0';
  }

  strcpy(format_buffer, "just a %d string");
  s21_sprintf(dest_custom, format_buffer, 555);
  sprintf(dest_original, format_buffer, 555);
  ck_assert_str_eq(dest_custom, dest_original);

  strcpy(format_buffer, "just a %.*d string");
  s21_sprintf(dest_custom, format_buffer, 5, 1);
  sprintf(dest_original, format_buffer, 5, 1);
  ck_assert_str_eq(dest_custom, dest_original);

  strcpy(format_buffer, "just a %*d string");
  s21_sprintf(dest_custom, format_buffer, 5, -5);
  sprintf(dest_original, format_buffer, 5, -5);
  ck_assert_str_eq(dest_custom, dest_original);

  strcpy(format_buffer, "just a %d %d string");
  s21_sprintf(dest_custom, format_buffer, -5, 10);
  sprintf(dest_original, format_buffer, -5, 10);
  ck_assert_str_eq(dest_custom, dest_original);
}
END_TEST

START_TEST(s21_sprintf_test_pointer) {
  int responce_custom, responce_original;
  char dest_custom[512];
  char dest_original[512];
  for (int i = 0; i < 512; i++) {
    dest_custom[i] = '\0';
    dest_original[i] = '\0';
  }
  responce_custom = s21_sprintf(dest_custom, "%p", &responce_custom);
  responce_original = sprintf(dest_original, "%p", &responce_custom);
  ck_assert_msg(strncmp(dest_custom, dest_original, 10) == 0,
                "s21: %s, cor: %s", dest_custom, dest_original);
  ck_assert_msg(responce_custom == responce_original, "s21: %d, cor: %d",
                responce_custom, responce_original);
}
END_TEST

START_TEST(s21_sprintf_test_n) {
  int responce_custom, responce_original;
  char dest_custom[1024];
  char dest_original[1024];
  int n1 = 0, n2 = 0;
  responce_custom = s21_sprintf(dest_custom, "abc %n", &n1);
  responce_original = sprintf(dest_original, "abc %n", &n2);
  ck_assert_int_eq(responce_custom, responce_original);
  ck_assert_int_eq(n1, n2);
}
END_TEST

START_TEST(s21_sprintf_autogen) {
  int responce_custom, responce_original;
  char dest_custom[1024];
  char dest_original[1024];
  char format_buffer[512];
  char exec_buffer[512];
  for (int i = 0; i < 512; i++) {
    dest_custom[i] = '\0';
    dest_original[i] = '\0';
  }

  char flags[5] = {'+', '-', ' ', '0', '#'};
  int flags_ptr = 0;
  char width[4][5] = {"0", "1", "5", "15"};
  int width_ptr = -1;
  char precision[4][5] = {"0", "1", "5", "15"};
  int precision_ptr = -1;

#define CASES 5
  int test_ints[CASES] = {0, -0, 1, 55, -55};
  int test_pos_ints[CASES] = {1, 1, 55, 555, 1234};
  double test_floats[CASES] = {123.1234, -123.1234, 1, 0, 3412123425};
  long double test_long_double[CASES] = {123.1234, -123.1234, 1, 0, 3412123425};
  char test_chars[CASES] = {'K', 'a', 'P', 'O', 'b'};
  char test_strings[CASES][64] = {
      "", "asd", "askjdadf", "ajdsnfjkafjkasdkfkadbfkabkhdfkhasbdfakdsfkad",
      "p"};
  int case_ptr = 0;

  int counter = 0;
  while (1) {
    counter++;
    format_buffer[0] = '%';
    int format_ptr = 1;
    for (int i = 0; i < 5; i++) {
      if ((flags_ptr >> i) & 1) {
        format_buffer[format_ptr] = flags[i];
        format_ptr++;
      }
    }
    format_buffer[format_ptr] = '\0';
    if (width_ptr != -1) {
      strcat(format_buffer, width[width_ptr]);
    }
    if (precision_ptr != -1) {
      strcat(format_buffer, ".");
      strcat(format_buffer, precision[precision_ptr]);
    }

    // Test start
    strcpy(exec_buffer, format_buffer);
    strcat(exec_buffer, "ld");
    responce_custom =
        s21_sprintf(dest_custom, exec_buffer, test_ints[case_ptr]);
    responce_original =
        sprintf(dest_original, exec_buffer, test_ints[case_ptr]);
    ck_assert_msg(strcmp(dest_custom, dest_original) == 0,
                  "format: %s, s21: %s, cor: %s, passed: %d", exec_buffer,
                  dest_custom, dest_original, counter);
    ck_assert_int_eq(responce_custom, responce_original);

    strcpy(exec_buffer, format_buffer);
    strcat(exec_buffer, "hd");
    responce_custom =
        s21_sprintf(dest_custom, exec_buffer, test_ints[case_ptr]);
    responce_original =
        sprintf(dest_original, exec_buffer, test_ints[case_ptr]);
    ck_assert_msg(strcmp(dest_custom, dest_original) == 0,
                  "format: %s, s21: %s, cor: %s, passed: %d", exec_buffer,
                  dest_custom, dest_original, counter);
    ck_assert_int_eq(responce_custom, responce_original);

    strcpy(exec_buffer, format_buffer);
    strcat(exec_buffer, "d");
    responce_custom =
        s21_sprintf(dest_custom, exec_buffer, test_ints[case_ptr]);
    responce_original =
        sprintf(dest_original, exec_buffer, test_ints[case_ptr]);
    ck_assert_msg(strcmp(dest_custom, dest_original) == 0,
                  "format: %s, s21: %s, cor: %s, passed: %d", exec_buffer,
                  dest_custom, dest_original, counter);
    ck_assert_int_eq(responce_custom, responce_original);

    strcpy(exec_buffer, format_buffer);
    strcat(exec_buffer, "i");
    responce_custom =
        s21_sprintf(dest_custom, exec_buffer, test_ints[case_ptr]);
    responce_original =
        sprintf(dest_original, exec_buffer, test_ints[case_ptr]);
    ck_assert_msg(strcmp(dest_custom, dest_original) == 0,
                  "format: %s, s21: %s, cor: %s, passed: %d", exec_buffer,
                  dest_custom, dest_original, counter);
    ck_assert_int_eq(responce_custom, responce_original);

    strcpy(exec_buffer, format_buffer);
    strcat(exec_buffer, "x");
    responce_custom =
        s21_sprintf(dest_custom, exec_buffer, test_pos_ints[case_ptr]);
    responce_original =
        sprintf(dest_original, exec_buffer, test_pos_ints[case_ptr]);
    ck_assert_msg(strcmp(dest_custom, dest_original) == 0,
                  "format: %s, s21: %s, cor: %s, passed: %d", exec_buffer,
                  dest_custom, dest_original, counter);
    ck_assert_int_eq(responce_custom, responce_original);

    strcpy(exec_buffer, format_buffer);
    strcat(exec_buffer, "X");
    responce_custom =
        s21_sprintf(dest_custom, exec_buffer, test_pos_ints[case_ptr]);
    responce_original =
        sprintf(dest_original, exec_buffer, test_pos_ints[case_ptr]);
    ck_assert_msg(strcmp(dest_custom, dest_original) == 0,
                  "format: %s, s21: %s, cor: %s, passed: %d", exec_buffer,
                  dest_custom, dest_original, counter);
    ck_assert_int_eq(responce_custom, responce_original);

    strcpy(exec_buffer, format_buffer);
    strcat(exec_buffer, "u");
    responce_custom =
        s21_sprintf(dest_custom, exec_buffer, test_pos_ints[case_ptr]);
    responce_original =
        sprintf(dest_original, exec_buffer, test_pos_ints[case_ptr]);
    ck_assert_msg(strcmp(dest_custom, dest_original) == 0,
                  "format: %s, s21: %s, cor: %s, passed: %d", exec_buffer,
                  dest_custom, dest_original, counter);
    ck_assert_int_eq(responce_custom, responce_original);

    strcpy(exec_buffer, format_buffer);
    strcat(exec_buffer, "o");
    responce_custom =
        s21_sprintf(dest_custom, exec_buffer, test_pos_ints[case_ptr]);
    responce_original =
        sprintf(dest_original, exec_buffer, test_pos_ints[case_ptr]);
    ck_assert_msg(strcmp(dest_custom, dest_original) == 0,
                  "format: %s, s21: %s, cor: %s, passed: %d", exec_buffer,
                  dest_custom, dest_original, counter);
    ck_assert_int_eq(responce_custom, responce_original);

    strcpy(exec_buffer, format_buffer);
    strcat(exec_buffer, "s");
    responce_custom =
        s21_sprintf(dest_custom, exec_buffer, test_strings[case_ptr]);
    responce_original =
        sprintf(dest_original, exec_buffer, test_strings[case_ptr]);
    ck_assert_msg(strcmp(dest_custom, dest_original) == 0,
                  "format: %s, s21: %s, cor: %s, passed: %d", exec_buffer,
                  dest_custom, dest_original, counter);
    ck_assert_int_eq(responce_custom, responce_original);

    strcpy(exec_buffer, format_buffer);
    strcat(exec_buffer, "c");
    responce_custom =
        s21_sprintf(dest_custom, exec_buffer, test_chars[case_ptr]);
    responce_original =
        sprintf(dest_original, exec_buffer, test_chars[case_ptr]);
    ck_assert_msg(strcmp(dest_custom, dest_original) == 0,
                  "format: %s, s21: %s, cor: %s, passed: %d", exec_buffer,
                  dest_custom, dest_original, counter);
    ck_assert_msg(responce_custom == responce_original,
                  "format: %s, s21: %d, cor: %d, passed tests: %d, value: %c",
                  exec_buffer, responce_custom, responce_original, counter,
                  test_chars[case_ptr]);

    strcpy(exec_buffer, format_buffer);
    strcat(exec_buffer, "lc");
    responce_custom =
        s21_sprintf(dest_custom, exec_buffer, test_chars[case_ptr]);
    responce_original =
        sprintf(dest_original, exec_buffer, test_chars[case_ptr]);
    ck_assert_msg(strcmp(dest_custom, dest_original) == 0,
                  "format: %s, s21: %s, cor: %s, passed: %d", exec_buffer,
                  dest_custom, dest_original, counter);
    ck_assert_msg(responce_custom == responce_original,
                  "format: %s, value: %c, s21: %d, cor: %d, passed tests: %d",
                  exec_buffer, test_chars[case_ptr], responce_custom,
                  responce_original, counter);

    strcpy(exec_buffer, format_buffer);
    strcat(exec_buffer, "f");
    responce_custom =
        s21_sprintf(dest_custom, exec_buffer, test_floats[case_ptr]);
    responce_original =
        sprintf(dest_original, exec_buffer, test_floats[case_ptr]);
    ck_assert_msg(strncmp(dest_custom, dest_original, 3 + 7) == 0,
                  "format: %s, s21: %s, cor: %s, passed: %d", exec_buffer,
                  dest_custom, dest_original, counter);
    ck_assert_int_eq(responce_custom, responce_original);

    strcpy(exec_buffer, format_buffer);
    strcat(exec_buffer, "Lf");
    responce_custom =
        s21_sprintf(dest_custom, exec_buffer, test_long_double[case_ptr]);
    responce_original =
        sprintf(dest_original, exec_buffer, test_long_double[case_ptr]);
    ck_assert_msg(strncmp(dest_custom, dest_original, 3 + 7) == 0,
                  "format: %s, s21: %s, cor: %s, passed: %d", exec_buffer,
                  dest_custom, dest_original, counter);
    ck_assert_int_eq(responce_custom, responce_original);

    strcpy(exec_buffer, format_buffer);
    strcat(exec_buffer, "g");
    responce_custom =
        s21_sprintf(dest_custom, exec_buffer, test_floats[case_ptr]);
    responce_original =
        sprintf(dest_original, exec_buffer, test_floats[case_ptr]);
    ck_assert_msg(strncmp(dest_custom, dest_original, 3 + 7) == 0,
                  "format: %s, s21: %s, cor: %s, passed: %d", exec_buffer,
                  dest_custom, dest_original, counter);
    ck_assert_int_eq(responce_custom, responce_original);

    strcpy(exec_buffer, format_buffer);
    strcat(exec_buffer, "G");
    responce_custom =
        s21_sprintf(dest_custom, exec_buffer, test_floats[case_ptr]);
    responce_original =
        sprintf(dest_original, exec_buffer, test_floats[case_ptr]);
    ck_assert_msg(strncmp(dest_custom, dest_original, 8) == 0,
                  "format: %s, s21: %s, cor: %s, passed: %d", exec_buffer,
                  dest_custom, dest_original, counter);
    ck_assert_int_eq(responce_custom, responce_original);

    strcpy(exec_buffer, format_buffer);
    strcat(exec_buffer, "e");
    responce_custom =
        s21_sprintf(dest_custom, exec_buffer, test_floats[case_ptr]);
    responce_original =
        sprintf(dest_original, exec_buffer, test_floats[case_ptr]);
    ck_assert_msg(strncmp(dest_custom, dest_original, 8) == 0,
                  "format: %s, s21: %s, cor: %s, passed: %d", exec_buffer,
                  dest_custom, dest_original, counter);
    ck_assert_int_eq(responce_custom, responce_original);

    strcpy(exec_buffer, format_buffer);
    strcat(exec_buffer, "E");
    responce_custom =
        s21_sprintf(dest_custom, exec_buffer, test_floats[case_ptr]);
    responce_original =
        sprintf(dest_original, exec_buffer, test_floats[case_ptr]);
    ck_assert_msg(strncmp(dest_custom, dest_original, 8) == 0,
                  "format: %s, s21: %s, cor: %s, passed: %d", exec_buffer,
                  dest_custom, dest_original, counter);
    ck_assert_int_eq(responce_custom, responce_original);

    strcpy(exec_buffer, format_buffer);
    strcat(exec_buffer, "f %% ");
    responce_custom =
        s21_sprintf(dest_custom, exec_buffer, test_floats[case_ptr]);
    responce_original =
        sprintf(dest_original, exec_buffer, test_floats[case_ptr]);
    ck_assert_msg(strncmp(dest_custom, dest_original, 10) == 0,
                  "format: %s, s21: %s, cor: %s, passed: %d", exec_buffer,
                  dest_custom, dest_original, counter);
    ck_assert_msg(responce_custom == responce_original,
                  "format: %s, s21: %d, cor: %d, passed: %d", exec_buffer,
                  responce_custom, responce_original, counter);

    if (flags_ptr < 31) {
      flags_ptr++;
    } else if (width_ptr < 3) {
      flags_ptr = 0;
      width_ptr++;
    } else if (precision_ptr < 3) {
      flags_ptr = 0;
      width_ptr = -1;
      precision_ptr++;
    } else if (case_ptr < CASES - 1) {
      flags_ptr = 0;
      width_ptr = -1;
      precision_ptr = -1;
      case_ptr++;
    } else {
      break;
    }
  }
  printf("Test done: %d\n", counter);
}
END_TEST

int main() {
  Suite *s;
  SRunner *sr;

  s = suite_create("main");
  TCase *hello_case = tcase_create("core");

  suite_add_tcase(s, hello_case);
  tcase_add_test(hello_case, s21_strerror_test);
  tcase_add_test(hello_case, s21_strlen_test);
  tcase_add_test(hello_case, s21_strchr_test);
  tcase_add_test(hello_case, s21_strncpy_test);
  tcase_add_test(hello_case, s21_memchr_test);
  tcase_add_test(hello_case, s21_memcmp_test);
  tcase_add_test(hello_case, s21_memcpy_test);
  tcase_add_test(hello_case, s21_memset_test);
  tcase_add_test(hello_case, s21_strncmp_test);
  tcase_add_test(hello_case, s21_strncat_test);
  tcase_add_test(hello_case, s21_strcspn_test);
  tcase_add_test(hello_case, s21_strpbrk_test);
  tcase_add_test(hello_case, s21_strrchr_test);
  tcase_add_test(hello_case, s21_strstr_test);
  tcase_add_test(hello_case, s21_to_lower_test);
  tcase_add_test(hello_case, s21_to_upper_test);
  tcase_add_test(hello_case, s21_insert_test);
  tcase_add_test(hello_case, s21_trim_test);
  tcase_add_test(hello_case, s21_strtok_test);

  tcase_add_test(hello_case, s21_sprintf_test_integers);
  tcase_add_test(hello_case, s21_sprintf_test_pointer);
  tcase_add_test(hello_case, s21_sprintf_test_n);
  tcase_add_test(hello_case, s21_sprintf_autogen);

  tcase_set_timeout(hello_case, 0);

  sr = srunner_create(s);
  srunner_run_all(sr, CK_NORMAL);
  int number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed) ? 1 : 0;
}