#include "s21_decimal.h"

int s21_integer_greater(s21_decimal target, s21_decimal cmp) {
  int res = 0;
  int sign_target = s21_get_sign(target);
  int cmp_target = s21_get_sign(cmp);
  if (sign_target < cmp_target) {
    res = 1;
  } else if (sign_target == cmp_target) {
    res = 2;
    for (int i = 2; i >= 0; i--) {
      if (target.bits[i] > cmp.bits[i]) {
        res = 1;
        break;
      } else if (target.bits[i] < cmp.bits[i]) {
        res = 0;
        break;
      }
    }
    if (sign_target == 1) {
      res = !res;
    } else if (res == 2) {
      res = 0;
    }
  }

  return res;
}

int s21_integer_less(s21_decimal target, s21_decimal cmp) {
  int res = 0;
  int sign_target = s21_get_sign(target);
  int cmp_target = s21_get_sign(cmp);
  if (sign_target > cmp_target) {
    res = 1;
  } else if (sign_target == cmp_target) {
    res = 2;
    for (int i = 2; i >= 0; i--) {
      if (target.bits[i] < cmp.bits[i]) {
        res = 1;
        break;
      } else if (target.bits[i] > cmp.bits[i]) {
        res = 0;
        break;
      }
    }
    if (sign_target == 1) {
      res = !res;
    } else if (res == 2) {
      res = 0;
    }
  }
  return res;
}

int s21_is_equal(s21_decimal first, s21_decimal second) {
  int result = TRUE;

  if (s21_compare_zero_decimals(first, second) == 0) {
    s21_decimal first_int;
    s21_decimal first_mantissa;
    s21_decimal second_int;
    s21_decimal second_mantissa;
    s21_split_decimal(first, &first_int, &first_mantissa);
    s21_split_decimal(second, &second_int, &second_mantissa);
    if (first_int.bits[0] != second_int.bits[0] ||
        first_int.bits[1] != second_int.bits[1] ||
        first_int.bits[2] != second_int.bits[2]) {
      result = FALSE;
    } else {
      int degree_first = s21_get_degree(first);
      int degree_second = s21_get_degree(second);
      if (degree_first != degree_second) {
        if (degree_first < degree_second) {
          first_mantissa =
              s21_mul_ten(first_mantissa, degree_second - degree_first);
          s21_set_degree(&first_mantissa, degree_second);
        } else {
          second_mantissa =
              s21_mul_ten(second_mantissa, degree_first - degree_second);
          s21_set_degree(&second_mantissa, degree_first);
        }
      }

      for (int k = 0; k < 4; k++) {
        if (first_mantissa.bits[k] != second_mantissa.bits[k]) {
          result = FALSE;
          break;
        }
      }
    }
  }
  return result;
}

int s21_compare_zero_decimals(s21_decimal first, s21_decimal second) {
  int result = 1;
  if (first.bits[0] != 0 || first.bits[1] != 0 || first.bits[2] != 0 ||
      second.bits[0] != 0 || second.bits[1] != 0 || second.bits[2] != 0) {
    result = 0;
  }
  return result;
}

int s21_is_not_equal(s21_decimal first, s21_decimal second) {
  return s21_is_equal(first, second) ? FALSE : TRUE;
}

int s21_is_greater(s21_decimal first, s21_decimal second) {
  int result = TRUE;

  if (s21_is_equal(first, second) == 1) {
    result = FALSE;
  } else {
    s21_decimal first_int;
    s21_decimal first_mantissa;
    s21_decimal second_int;
    s21_decimal second_mantissa;
    s21_split_decimal(first, &first_int, &first_mantissa);
    s21_split_decimal(second, &second_int, &second_mantissa);
    if (s21_integer_less(first_int, second_int) == 1) {
      result = FALSE;
    } else {
      if (s21_is_equal(first_int, second_int) == 1 &&
          s21_integer_less(first_mantissa, second_mantissa) == 1) {
        result = FALSE;
      }
    }
  }
  return result;
}

int s21_is_less(s21_decimal first, s21_decimal second) {
  int result = TRUE;
  if (s21_is_equal(first, second) == 1) {
    result = FALSE;
  } else {
    s21_decimal first_int;
    s21_decimal first_mantissa;
    s21_decimal second_int;
    s21_decimal second_mantissa;
    s21_split_decimal(first, &first_int, &first_mantissa);
    s21_split_decimal(second, &second_int, &second_mantissa);
    if (s21_integer_greater(first_int, second_int) == 1) {
      result = FALSE;
    } else {
      if (s21_is_equal(first_int, second_int) == 1 &&
          s21_integer_greater(first_mantissa, second_mantissa) == 1) {
        result = FALSE;
      }
    }
  }
  return result;
}

int s21_is_greater_or_equal(s21_decimal first, s21_decimal second) {
  int result = TRUE;
  if (s21_is_equal(first, second) != 1) {
    s21_decimal first_int;
    s21_decimal first_mantissa;
    s21_decimal second_int;
    s21_decimal second_mantissa;
    s21_split_decimal(first, &first_int, &first_mantissa);
    s21_split_decimal(second, &second_int, &second_mantissa);
    if (s21_integer_less(first_int, second_int) == 1) {
      result = FALSE;
    } else {
      if (s21_is_equal(first_int, second_int) == 1 &&
          s21_integer_less(first_mantissa, second_mantissa) == 1) {
        result = FALSE;
      }
    }
  }
  return result;
}

int s21_is_less_or_equal(s21_decimal first, s21_decimal second) {
  int result = TRUE;
  if (s21_is_equal(first, second) != 1) {
    s21_decimal first_int;
    s21_decimal first_mantissa;
    s21_decimal second_int;
    s21_decimal second_mantissa;
    s21_split_decimal(first, &first_int, &first_mantissa);
    s21_split_decimal(second, &second_int, &second_mantissa);
    if (s21_integer_greater(first_int, second_int) == 1) {
      result = FALSE;
    } else {
      if (s21_is_equal(first_int, second_int) == 1 &&
          s21_integer_greater(first_mantissa, second_mantissa) == 1) {
        result = FALSE;
      }
    }
  }
  return result;
}
