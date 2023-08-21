#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "s21_decimal.h"

void s21_decimal_init(s21_decimal *target) {
  for (int i = 0; i < NOI + 1; i++) {
    target->bits[i] = 0;
  }
}

int s21_shift_left_1(s21_decimal *val) {
  int buff = 0;
  int bit = 0;
  for (int k = 0; k < 3; k++) {
    buff = (val->bits[k] & SIGN_MASK) == SIGN_MASK ? 1 : 0;
    val->bits[k] <<= 1;
    val->bits[k] |= bit;
    bit = buff;
  }
  return buff;
}

int s21_shift_right_1(s21_decimal *val) {
  int buff = 0;
  int bit = 0;
  for (int k = 0; k < 3; ++k) {
    buff = (val->bits[k] & SIGN_MASK) == SIGN_MASK ? 1 : 0;
    val->bits[k] >>= 1;
    val->bits[k] |= bit;
    bit = buff;
  }
  return buff;
}

unsigned s21_get_bit(s21_decimal val, unsigned pos) {
  unsigned res = 0;
  res = (val.bits[pos / 32] >> pos % 32) & 1U;
  return res == 0 ? 0 : 1;
}

int s21_find_hight_bit(s21_decimal val) {
  int result = -1;

  for (int k = 0; k < 96; k++) {
    if (val.bits[k / 32] == 0) {
      k += 31;
      continue;
    } else {
      if (s21_get_bit(val, k) == 1) {
        result = k;
      }
    }
  }
  return result + 1;
}

void s21_set_bit(s21_decimal *val, unsigned pos) {
  val->bits[pos / 32] |= (1UL << pos % 32);
}

int s21_get_sign(s21_decimal target) {
  return (target.bits[3] & SIGN_MASK) == SIGN_MASK ? 1 : 0;
}

void s21_set_sign(s21_decimal *target, int sign) {
  target->bits[3] = (sign << 31) | (s21_get_degree(*target) << 16);
}

int s21_get_number_of_ints(s21_decimal val) {
  int i = 0;
  for (i = 0; i < 29; i++) {
    if (s21_check_zero(val) == 1) {
      break;
    } else {
      s21_div_to_10(&val);
    }
  }
  return i;
}

int s21_check_zero(s21_decimal val) {
  int result = 0;
  if (val.bits[0] == 0 && val.bits[1] == 0 && val.bits[2] == 0) {
    result = 1;
  }
  return result;
}

int s21_tests(s21_decimal result, s21_decimal answer) {
  int res = 1;
  if (result.bits[0] == answer.bits[0] && result.bits[1] == answer.bits[1] &&
      result.bits[2] == answer.bits[2] && result.bits[3] == answer.bits[3]) {
    res = 0;
  }
  return res;
}

int s21_assert(s21_decimal calculated, s21_decimal expected) {
  int res = 1;
  for (int i = 0; i < 4; i++) {
    if (calculated.bits[i] != expected.bits[i]) {
      res = 0;
    }
  }
  return res;
}
int s21_modifyBit(int num, int pos, int val) {
  int mask = 1 << pos;
  return ((num & ~mask) | (val << pos));
}
