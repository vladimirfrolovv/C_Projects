#include "s21_decimal.h"

void s21_set_degree(s21_decimal *val, int degree) {
  int sign = s21_get_sign(*val);
  val->bits[3] = (degree << 16) | (sign << 31);
}

int s21_get_degree(s21_decimal val) {
  return (val.bits[3] & DEGREE_MASK) >> 16;
}
