#include "s21_decimal.h"

void s21_div_to_10(s21_decimal *in) {
  unsigned long int p_rest = 0;
  int div = 10;
  unsigned rest = 0;
  for (int i = 2; i >= 0; i--) {
    p_rest = rest * 4294967296L + in->bits[i];
    in->bits[i] = p_rest / div;
    rest = p_rest - div * in->bits[i];
  }
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int status = OK;
  if (result != NULL) {
    s21_decimal_init(result);
    int sign = 0;
    int counter = 0;
    s21_decimal *target = result;
    s21_decimal mantissa = {0};
    s21_decimal zero = {0};

    if (s21_is_equal(zero, value_2)) {
      status = DIV_BY_ZERO;
    } else if (s21_get_degree(value_1) - s21_get_degree(value_2) < -28) {
      status = TOO_SMALL;
    } else if (s21_get_degree(value_1) - s21_get_degree(value_2) > 28) {
      status = TOO_LARGE;
    } else {
      int high_bit_value_1 = s21_find_hight_bit(value_1);
      int high_bit_value_2 = s21_find_hight_bit(value_2);
      int high_bit_difference = high_bit_value_1 - high_bit_value_2;
      int result_degree = s21_get_degree(value_1) - s21_get_degree(value_2);

      if ((!s21_get_sign(value_1) && s21_get_sign(value_2)) ||
          (s21_get_sign(value_1) && !s21_get_sign(value_2))) {
        sign = 1;
      }
      s21_set_sign(&value_1, 0);
      s21_set_sign(&value_2, 0);
      s21_set_degree(&value_1, 0);
      s21_set_degree(&value_2, 0);

      if (high_bit_value_1 > high_bit_value_2) {
        for (int i = 0; i < high_bit_difference; i++) {
          s21_shift_left_1(&value_2);
        }
      }

      for (int i = 0; i < NOI * 32; i++) {
        if (i == (high_bit_difference + 1)) {
          target = &mantissa;
          counter = 0;
        }
        if (i == (high_bit_difference + 31) || s21_is_equal(zero, value_1)) {
          if (high_bit_difference < 0) {
            s21_shift_left_1(target);
          } else {
            for (int j = 0; j < high_bit_difference + 1 - i; j++) {
              s21_shift_left_1(target);
            }
          }
          break;
        }

        if (s21_is_greater_or_equal(value_1, value_2)) {
          s21_sub(value_1, value_2, &value_1);
          s21_shift_left_1(target);
          s21_set_bit(target, 0);
        } else {
          s21_shift_left_1(target);
        }
        counter++;
        s21_shift_left_1(&value_1);
      }

      float k = 0;
      for (int i = counter; i >= 0; i--) {
        k += pow(2, i * (-1)) * (mantissa.bits[0] & 1);
        s21_shift_right_1(&mantissa);
      }

      k *= 10000000;
      if (result_degree < 0) {
        for (int g = 0; g < result_degree * -1; g++) {
          k *= 10;
        }
      }
      int h = (int)k;
      s21_from_int_to_decimal(h, &mantissa);

      if (s21_is_equal(zero, mantissa)) {
        s21_set_degree(result, result_degree);
      } else {
        if (result_degree > 0) {
          s21_set_degree(&mantissa, 7 + result_degree);
        } else {
          s21_set_degree(&mantissa, 7);
        }
      }

      if (!s21_is_equal(mantissa, zero)) {
        s21_merge_decimal(*result, mantissa, result);
      }
      s21_set_sign(result, sign);
    }
  }
  return status;
}

int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  return s21_div(value_1, value_2, result);
}