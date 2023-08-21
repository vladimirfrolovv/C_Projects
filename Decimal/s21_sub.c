#include <stdint.h>

#include "s21_decimal.h"

s21_decimal s21_sub_processing(s21_decimal first, s21_decimal second,
                               s21_decimal result, int buff, unsigned res,
                               s21_decimal once) {
  for (int i = 0; i < 3; i++) {
    res = first.bits[i] - second.bits[i] - buff;
    buff = (res > first.bits[i]) ||
                   (res == first.bits[i] && second.bits[i] == UINT32_MAX) ||
                   (res == second.bits[i] && first.bits[i] == UINT32_MAX)
               ? 1
               : 0;
    result.bits[i] = res;
  }
  if (buff == 1) {
    for (int i = 0; i < 3; i++) {
      result.bits[i] = ~result.bits[i];
    }
    result = s21_sum_integer(result, once);
    result.bits[3] ^= SIGN_MASK;
  }
  return result;
}

s21_decimal s21_subtraction(s21_decimal first, s21_decimal second) {
  s21_decimal result = {0};
  s21_decimal once = {.bits[0] = 1, .bits[1] = 0, .bits[2] = 0, .bits[3] = 0};
  int buff = 0;
  unsigned int res = 0;

  if (s21_get_sign(first) == 0 && s21_get_sign(second) == 0) {
    result = s21_sub_processing(first, second, result, buff, res, once);
  } else {
    second.bits[3] ^= SIGN_MASK;
    result = s21_sum_integer(first, second);
  }

  return result;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int status = OK;
  if (result != NULL) {
    value_2.bits[3] ^= SIGN_MASK;
    status = s21_add(value_1, value_2, result);
  }
  return status;
}
