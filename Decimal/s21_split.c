#include <math.h>
#include <stdio.h>

#include "s21_decimal.h"

int s21_minus_decimal(s21_decimal a, s21_decimal b, s21_decimal *result) {
  int next_one = 0;
  int bit_one = 0;
  int bit_two = 0;

  int k = 0;
  unsigned err = 0;
  for (k = 0; k < 96; k++) {
    bit_one = s21_get_bit(a, k);
    bit_two = s21_get_bit(b, k);
    if (bit_one - next_one < 0) {
      bit_one = 1;
      next_one = 1;
    } else {
      bit_one = bit_one - next_one;
      next_one = 0;
    }
    if (bit_one - bit_two < 0) {
      next_one = 1;
      s21_set_bit(result, k);
    } else if (bit_one - bit_two == 1) {
      s21_set_bit(result, k);
    }
  }
  if (k == 95 && next_one == 1) {
    err = 1;
  }
  if (k == 95 && next_one == 1) {
    err = 1;
  }
  return err;
}

int s21_split_decimal(s21_decimal target, s21_decimal *integer,
                      s21_decimal *mantissa) {
  s21_decimal truncated = {0};
  s21_decimal_init(&truncated);
  s21_decimal_init(mantissa);
  s21_decimal_init(integer);

  int degree = s21_get_degree(target);
  int sign = s21_get_sign(target);

  s21_set_sign(integer, sign);
  s21_set_sign(mantissa, sign);
  s21_set_degree(mantissa, degree);

  s21_truncate(target, &truncated);
  *integer = truncated;
  truncated = s21_mul_ten(truncated, degree);
  s21_minus_decimal(target, truncated, mantissa);
  return 0;
}

void s21_merge_decimal(s21_decimal integer, s21_decimal mantissa,
                       s21_decimal *target) {
  *target = (s21_decimal){{0, 0, 0, 0}};
  int size_integer = s21_get_number_of_ints(integer);
  int size_mantissa =
      s21_get_degree(mantissa);  // s21_get_number_of_ints(mantissa);
  int empty_space = fmin(29 - size_integer, size_mantissa);
  if (empty_space == 0 || size_mantissa == 0) {
    mantissa = (s21_decimal){{0, 0, 0, 0}};
  } else if (size_mantissa <= empty_space) {
    integer = s21_mul_ten_overflow(integer, &empty_space);
    for (int i = 0; i < size_mantissa - empty_space; i++) {
      s21_div_to_10(&mantissa);
    }
  } else {
    integer = s21_mul_ten_overflow(integer, &empty_space);
    for (int i = 0; i < size_mantissa - empty_space; i++) {
      s21_div_to_10(&mantissa);
    }

    s21_set_degree(&mantissa, empty_space);
  }
  *target = s21_sum_integer(integer, mantissa);
  s21_set_degree(target, empty_space);
  target->bits[3] |= integer.bits[3];
}