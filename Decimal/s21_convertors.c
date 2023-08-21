#include "s21_decimal.h"

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int error = OK;

  if (dst != NULL) {
    int degree = s21_get_degree(src);
    for (int k = 0; k < degree; k++) {
      s21_div_to_10(&src);
    }
    *dst = src.bits[0];
    if (src.bits[1] || src.bits[2]) {
      error = CONVERT_ERROR;
    }
    if (s21_get_sign(src) == 1) {
      *dst *= -1;
    }
  }
  return error;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  int status = OK;
  if (dst == NULL)
    status = CONVERT_ERROR;
  else {
    s21_decimal int_part = {0};
    s21_decimal mantissa_part = {0};
    s21_split_decimal(src, &int_part, &mantissa_part);
    if (int_part.bits[1] > 0 || int_part.bits[2] > 0) {
      status = CONVERT_ERROR;
    } else {
      double res = 0;
      for (int k = 0; k < 96; k++) {
        res += s21_get_bit(src, k) * pow(2, k);
      }
      for (int k = 0; k < s21_get_degree(src); k++) {
        res /= 10;
      }
      *dst = (float)res;
      if (s21_get_sign(src) == 1) {
        *dst *= -1;
      }
    }
  }
  return status;
}

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  if (dst != NULL) {
    s21_decimal_init(dst);
    if (src < 0) {
      dst->bits[3] = s21_modifyBit(dst->bits[3], 31, 1);
      src *= -1;
    }
    dst->bits[0] = src;
  }
  return dst == NULL ? CONVERT_ERROR : OK;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  int error = 0;
  double buf = (double)src;
  if (dst == NULL) {
    error = 1;
  } else if (src >= 7.922816251e+29 || src <= -7.922816251e+29) {
    error = 1;
  } else {
    s21_decimal_init(dst);
    if (src < 0) {
      dst->bits[3] = 1 << 31;
      src *= -1;
      buf *= -1;
    }

    unsigned scale = 0;
    while (scale < 28 && (int)buf / (int)powl(2, 21) == 0) {
      buf *= 10;
      scale++;
    }
    buf = round(buf);

    dst->bits[0] = (int)buf;
    s21_set_degree(dst, scale);
  }
  return error;
}