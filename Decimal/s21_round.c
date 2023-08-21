#include "s21_decimal.h"

int s21_round(s21_decimal value, s21_decimal *result) {
  int err = 0;

  if (result == NULL) {
    err = 1;
  } else {
    s21_decimal five = {{5, 0, 0, 65536}}, one = {{1, 0, 0, 0}}, mantisa = {0},
                integer = {0}, res = {0};
    int degree = s21_get_degree(value);
    if (value.bits[0] == 0 && value.bits[1] == 0 && value.bits[2] == 0) {
      s21_set_sign(&value, 0);
      s21_set_degree(&value, 0);
      *result = value;
    } else {
      if (degree != 0) {
        s21_split_decimal(value, &integer, &mantisa);
        int sign = s21_get_sign(integer);
        s21_set_sign(&integer, 0);
        s21_set_sign(&mantisa, 0);
        s21_set_degree(&mantisa, degree);
        s21_set_degree(&integer, degree);
        five = s21_mul_ten(five, degree - 1);
        s21_set_degree(&five, degree);
        if (s21_is_greater(mantisa, five)) {
          s21_set_degree(&one, degree);
          s21_add(integer, one, &res);
          s21_set_sign(&res, sign);
          s21_set_degree(&res, 0);
          *result = res;
        }
        if (s21_is_less(mantisa, five)) {
          s21_set_sign(&integer, sign);
          s21_set_degree(&integer, 0);
          *result = integer;
        }
        if (s21_is_equal(mantisa, five)) {
          if (s21_get_bit(integer, 0)) {
            s21_set_degree(&one, degree);
            s21_add(integer, one, &res);
            s21_set_sign(&res, sign);
            s21_set_degree(&res, 0);
            *result = res;
          } else {
            s21_set_sign(&integer, sign);
            s21_set_degree(&integer, 0);
            *result = integer;
          }
        }
      } else {
        s21_set_degree(&value, 0);
        *result = value;
      }
    }
  }
  return err;
}

int s21_floor(s21_decimal value, s21_decimal *result) {
  int err = 0;
  if (result == NULL) {
    err = 1;
  } else {
    if (value.bits[0] == 0 && value.bits[1] == 0 && value.bits[2] == 0) {
      s21_set_sign(&value, 0);
      s21_set_degree(&value, 0);
      *result = value;
    } else {
      if (s21_get_degree(value) == 0) {
        *result = value;
      } else {
        s21_decimal one = {{1, 0, 0, 0}};
        s21_decimal mantisa = {0};
        s21_decimal integer = {0};
        s21_decimal res = {0};
        s21_split_decimal(value, &integer, &mantisa);
        int sign = s21_get_sign(value);
        if (sign == 1) {
          s21_set_sign(&integer, 1);
          s21_sub(integer, one, &res);
          s21_set_degree(&res, 0);
          *result = res;
        } else {
          s21_set_degree(&integer, 0);
          *result = integer;
        }
      }
    }
  }
  return err;
}

int s21_negate(s21_decimal value, s21_decimal *result) {
  int err = 0;
  if (result == NULL) {
    err = 1;
  } else {
    if (value.bits[0] == 0 && value.bits[1] == 0 && value.bits[2] == 0) {
      s21_set_sign(&value, 0);
      s21_set_degree(&value, 0);
      *result = value;
    } else {
      s21_get_sign(value) ? s21_set_sign(&value, 0) : s21_set_sign(&value, 1);
      *result = value;
    }
  }
  return err;
}

int s21_truncate(s21_decimal value, s21_decimal *result) {
  int degree = 0;
  int err = 0;
  degree = s21_get_degree(value);
  if (degree > 28 || result == NULL) {
    err = 1;
  } else {
    s21_set_degree(&value, 0);
    for (int k = 0; k < degree; k++) {
      s21_div_to_10(&value);
    }
  }
  *result = value;
  return err;
}
