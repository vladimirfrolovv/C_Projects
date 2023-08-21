#include <math.h>

#include "s21_decimal.h"

s21_decimal s21_sum_processing(s21_decimal first, s21_decimal second,
                               s21_decimal result, int buff, unsigned res) {
  for (int i = 0; i < NOI; i++) {
    res = first.bits[i] + second.bits[i] + buff;
    buff = (res < first.bits[i]) ||
                   (res == first.bits[i] && second.bits[i] != 0) ||
                   (res == second.bits[i] && first.bits[i] != 0)
               ? 1
               : 0;
    result.bits[i] = res;
  }
  result.bits[3] |= buff;

  return result;
}

s21_decimal s21_sum_integer(s21_decimal first, s21_decimal second) {
  s21_decimal result = {0};
  int buff = 0;
  unsigned res = 0;

  if (s21_get_sign(first) == 0 && s21_get_sign(second) == 1) {
    second.bits[3] ^= SIGN_MASK;
    result = s21_subtraction(first, second);
  } else if (s21_get_sign(first) == 1 && s21_get_sign(second) == 0) {
    first.bits[3] ^= SIGN_MASK;
    result = s21_subtraction(second, first);
  } else if (s21_get_sign(first) == 1 && s21_get_sign(second) == 1) {
    result.bits[3] ^= SIGN_MASK;
    result = s21_sum_processing(first, second, result, buff, res);
  } else {
    result = s21_sum_processing(first, second, result, buff, res);
  }
  result.bits[3] |= (s21_get_degree(second) << 16);

  return result;
}

int s21_fcheck(s21_decimal *integer, s21_decimal mantissa) {
  s21_decimal cp = {{0, 0, 0, 0}};
  s21_decimal once = {{1, 0, 0, (s21_get_sign(mantissa) << 31)}};
  int status = 0;
  int size = s21_get_number_of_ints(*integer);

  if (size == 29) {
    if (integer->bits[0] & 1) {
      s21_merge_decimal(once, mantissa, &mantissa);
    }

    s21_round(mantissa, &cp);

    if (cp.bits[0] == 2) {
      *integer = s21_sum_integer(*integer, once);
    }
    status = 1;
  }

  return status;
}

static void s21_check_degree(int value_1_degree, int value_2_degree,
                             int *common_degree, s21_decimal *value_1_mantissa,
                             s21_decimal *value_2_mantissa) {
  if (value_1_degree > value_2_degree) {
    *value_2_mantissa =
        s21_mul_ten(*value_2_mantissa, value_1_degree - value_2_degree);
    s21_set_degree(value_2_mantissa, s21_get_degree(*value_2_mantissa) +
                                         value_1_degree - value_2_degree);
  } else if (value_1_degree < value_2_degree) {
    *common_degree = value_2_degree;
    *value_1_mantissa =
        s21_mul_ten(*value_1_mantissa, value_2_degree - value_1_degree);
    s21_set_degree(value_1_mantissa, s21_get_degree(*value_1_mantissa) +
                                         value_2_degree - value_1_degree);
  }
}

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int status = OK;
  if (result != NULL) {
    s21_decimal_init(result);

    *result = (s21_decimal){{0, 0, 0, 0}};
    s21_decimal value_1_integer = {0};
    s21_decimal value_1_mantissa = {0};
    s21_decimal value_2_integer = {0};
    s21_decimal value_2_mantissa = {0};
    s21_decimal result_integer = {0};
    s21_decimal result_mantissa = {0};
    s21_decimal once = {{1, 0, 0, 0}};
    s21_decimal copy = {0};
    int value_1_degree = 0;
    int value_2_degree = 0;
    int common_degree = 0;

    value_1_degree = s21_get_degree(value_1);
    value_2_degree = s21_get_degree(value_2);

    s21_split_decimal(value_1, &value_1_integer, &value_1_mantissa);
    s21_split_decimal(value_2, &value_2_integer, &value_2_mantissa);

    common_degree = value_1_degree;
    s21_check_degree(value_1_degree, value_2_degree, &common_degree,
                     &value_1_mantissa, &value_2_mantissa);

    result_integer = s21_sum_integer(value_1_integer, value_2_integer);
    result_mantissa = s21_sum_integer(value_1_mantissa, value_2_mantissa);

    copy = result_mantissa;
    copy.bits[3] = 0;
    for (int i = 0; i < common_degree; i++) {
      s21_div_to_10(&copy);
    }

    if (s21_assert(copy, once)) {
      result_integer = s21_sum_integer(result_integer, once);

      result_mantissa =
          s21_subtraction(result_mantissa, s21_mul_ten(once, common_degree));
      s21_set_degree(&result_mantissa, common_degree);
    }

    int is_out_of_mantiss = s21_fcheck(&result_integer, result_mantissa);

    *result = result_integer;

    if (is_out_of_mantiss == 0) {
      if (s21_check_zero(result_mantissa) || result_integer.bits[3] == 1) {
        *result = result_integer;
      } else {
        s21_merge_decimal(result_integer, result_mantissa, result);
      }
    }

    if ((result->bits[3] & 0x80000001) == 0x80000001) {
      s21_decimal_init(result);
      status = TOO_SMALL;
    } else if ((result->bits[3] & 1) == 1) {
      s21_decimal_init(result);
      status = TOO_LARGE;
    }
  }

  return status;
}
