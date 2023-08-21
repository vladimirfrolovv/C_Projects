#include "s21_decimal.h"

s21_decimal s21_mul_ten(s21_decimal val, int degree) {
  s21_decimal result = val;
  s21_decimal one_shift = val;
  s21_decimal three_shift = val;

  for (int i = 0; i < degree; i++) {
    one_shift = result, three_shift = result;
    s21_shift_left_1(&one_shift);
    s21_shift_left_1(&three_shift);
    s21_shift_left_1(&three_shift);
    s21_shift_left_1(&three_shift);
    result = s21_sum_integer(one_shift, three_shift);
  }

  return result;
}

s21_decimal s21_mul_ten_overflow(s21_decimal val, int *degree) {
  s21_decimal result = val;
  s21_decimal buff = val;

  for (int i = 0; i < *degree; i++) {
    result = s21_mul_ten(result, 1);
    if (result.bits[3] & 1) {
      *degree = i;
      break;
    }
    buff = result;
  }

  return buff;
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int status = OK;
  if (result != NULL) {
    s21_decimal_init(result);

    s21_big_decimal result_big = {0};
    s21_big_decimal factor = {0};
    s21_big_decimal buf_res_big = {0};
    int s = s21_get_sign(value_1);
    int s2 = s21_get_sign(value_2);

    if (s && s2) s = 0;
    if (s || s2) s = 1;

    int degree_1 = s21_get_degree(value_1);
    int degree_2 = s21_get_degree(value_2);
    for (int k = 0; k < s21_find_hight_bit(value_2); k++) {
      int j = 0;
      while (j < k) {
        j++;
      }
      s21_shift_big_decimal(&factor, value_1, j);
      if (s21_get_bit(value_2, k) == 1) {
        result_big = s21_add_big_decimal(result_big, factor, result_big);
      }
    }

    s21_truncate_big(result_big, &buf_res_big, degree_1 + degree_2);

    int empty_space = 29 - s21_get_number_of_ints_big(buf_res_big);
    if (empty_space < 0) {
      status = TOO_LARGE;
    } else if (empty_space > 0) {
      if (empty_space < degree_1 + degree_2) {
        for (int k = 0; k < (degree_1 + degree_2) - empty_space; k++) {
          s21_div_to_10_big(&result_big);
        }
      }
    }

    result->bits[0] = result_big.bits[0];
    result->bits[1] = result_big.bits[1];
    result->bits[2] = result_big.bits[2];

    if (degree_1 + degree_2 > empty_space) {
      s21_set_degree(result, empty_space);
    } else {
      s21_set_degree(result, degree_1 + degree_2);
    }
    if (s21_get_sign(value_1)) {
      if (s21_get_sign(value_2) == 0) {
        s21_set_sign(result, 1);
      }
    } else if (s21_get_sign(value_2)) {
      s21_set_sign(result, 1);
    }
    if ((status && s == 1) ||
        (s21_check_zero(*result) &&
         (!s21_check_zero(value_1) && !s21_check_zero(value_2)))) {
      status = TOO_SMALL;
    }
  }
  return status;
}

int s21_get_number_of_ints_big(s21_big_decimal val) {
  int i = 0;

  for (i = 0; i < 40; i++) {
    if (val.bits[0] == 0 && val.bits[1] == 0 && val.bits[2] == 0 &&
        val.bits[3] == 0 && val.bits[4] == 0 && val.bits[5] == 0) {
      break;
    } else {
      s21_div_to_10_big(&val);
    }
  }
  return i;
}
