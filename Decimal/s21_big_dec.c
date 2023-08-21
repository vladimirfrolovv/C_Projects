#include "s21_decimal.h"

void s21_shift_big_decimal(s21_big_decimal *factor, s21_decimal value, int j) {
  int buff = 0;
  int bit = 0;

  for (int k = 0; k < 8; k++) {
    if (k < 3) {
      factor->bits[k] = value.bits[k];
    } else {
      factor->bits[k] = 0;
    }
  }
  for (int i = 0; i < j; i++) {
    for (int k = 0; k < 8; k++) {
      buff = (factor->bits[k] & SIGN_MASK) == SIGN_MASK ? 1 : 0;
      factor->bits[k] <<= 1;
      factor->bits[k] |= bit;
      bit = buff;
    }
  }
}

s21_big_decimal s21_add_big_decimal(s21_big_decimal first,
                                    s21_big_decimal second,
                                    s21_big_decimal result) {
  int buff = 0;
  unsigned res = 0;
  for (int i = 0; i < 6; i++) {
    res = first.bits[i] + second.bits[i] + buff;

    buff = (res < first.bits[i]) ||
                   (res == first.bits[i] && second.bits[i] != 0) ||
                   (res == second.bits[i] && first.bits[i] != 0)
               ? 1
               : 0;
    result.bits[i] = res;
  }
  result.bits[7] |= buff;

  return result;
}

void s21_div_to_10_big(s21_big_decimal *in) {
  unsigned long int p_rest = 0;
  int div = 10;
  int rest = 0;
  for (int i = 5; i >= 0; i--) {
    p_rest = rest * 4294967296L + in->bits[i];
    in->bits[i] = p_rest / div;
    rest = p_rest - div * in->bits[i];
  }
}

int s21_truncate_big(s21_big_decimal value, s21_big_decimal *result,
                     int degree) {
  int err = 0;
  if (result == NULL) {
    err = 1;
  } else {
    for (int k = 0; k < degree; k++) {
      s21_div_to_10_big(&value);
    }
  }
  *result = value;

  return err;
}