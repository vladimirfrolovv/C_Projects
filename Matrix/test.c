#include <check.h>
#include <math.h>
#include <stdlib.h>

#include "s21_matrix.h"

START_TEST(create_1) {
  matrix_t my_matrix;
  int res = s21_create_matrix(2, 2, &my_matrix);
  ck_assert_int_eq(res, 0);
  s21_remove_matrix(&my_matrix);

  res = s21_create_matrix(-2, 3, &my_matrix);
  ck_assert_int_eq(res, 1);

  res = s21_create_matrix(-3, 2, &my_matrix);
  ck_assert_int_eq(res, 1);

  res = s21_create_matrix(0, 2, &my_matrix);
  ck_assert_int_eq(res, 1);

  res = s21_create_matrix(3, 0, &my_matrix);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(eq_1) {
  matrix_t matrix_1, matrix_2;
  s21_create_matrix(3, 3, &matrix_1);
  s21_create_matrix(3, 3, &matrix_2);
  int res = s21_eq_matrix(&matrix_1, &matrix_2);
  ck_assert_int_eq(res, 1);
  s21_remove_matrix(&matrix_1);
  s21_remove_matrix(&matrix_2);

  res = s21_eq_matrix(&matrix_1, &matrix_2);
  ck_assert_int_eq(res, 0);

  s21_create_matrix(3, 3, &matrix_1);
  s21_create_matrix(6, 6, &matrix_2);
  res = s21_eq_matrix(&matrix_1, &matrix_2);
  ck_assert_int_eq(res, 0);
  s21_remove_matrix(&matrix_1);
  s21_remove_matrix(&matrix_2);

  s21_create_matrix(3, 3, &matrix_1);
  s21_create_matrix(3, 3, &matrix_2);
  matrix_1.matrix[1][2] = 2;
  res = s21_eq_matrix(&matrix_1, &matrix_2);
  ck_assert_int_eq(res, 0);
  s21_remove_matrix(&matrix_1);
  s21_remove_matrix(&matrix_2);

  s21_create_matrix(3, 3, &matrix_1);
  s21_create_matrix(3, 3, &matrix_2);
  matrix_1.matrix[1][2] = 2;
  matrix_2.matrix[1][2] = 2;
  res = s21_eq_matrix(&matrix_1, &matrix_2);
  ck_assert_int_eq(res, 1);
  s21_remove_matrix(&matrix_1);
  s21_remove_matrix(&matrix_2);
}
END_TEST

START_TEST(sum_1) {
  matrix_t matrix_1, matrix_2, result, answer;
  s21_create_matrix(3, 3, &matrix_1);
  s21_create_matrix(3, 3, &matrix_2);
  s21_create_matrix(3, 3, &answer);
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      matrix_1.matrix[i][j] = i + j;
      answer.matrix[i][j] = i + j;
    }
  }
  int res = s21_sum_matrix(&matrix_1, &matrix_2, &result);
  ck_assert_int_eq(res, 0);
  res = s21_eq_matrix(&result, &answer);
  ck_assert_int_eq(res, 1);
  res = s21_sum_matrix(NULL, &matrix_2, &result);
  ck_assert_int_eq(res, 1);
  s21_remove_matrix(&matrix_1);
  s21_remove_matrix(&matrix_2);
  s21_remove_matrix(&result);
  s21_remove_matrix(&answer);
}
END_TEST

START_TEST(sub_1) {
  matrix_t matrix_1, matrix_2, result, answer;
  s21_create_matrix(4, 4, &matrix_1);
  s21_create_matrix(4, 4, &matrix_2);
  s21_create_matrix(4, 4, &answer);
  int res = s21_sub_matrix(&matrix_1, &matrix_2, &result);
  ck_assert_int_eq(res, 0);
  res = s21_eq_matrix(&result, &answer);
  ck_assert_int_eq(res, 1);
  s21_remove_matrix(&matrix_1);
  s21_remove_matrix(&matrix_2);
  s21_remove_matrix(&result);
  s21_remove_matrix(&answer);

  s21_create_matrix(4, 5, &matrix_1);
  s21_create_matrix(4, 4, &matrix_2);
  res = s21_sub_matrix(&matrix_1, &matrix_2, &result);
  ck_assert_int_eq(res, 2);
  s21_remove_matrix(&matrix_1);
  s21_remove_matrix(&matrix_2);

  s21_create_matrix(4, 4, &matrix_2);
  res = s21_sub_matrix(&matrix_1, &matrix_2, &result);
  ck_assert_int_eq(res, 1);
  s21_remove_matrix(&matrix_1);
  s21_remove_matrix(&matrix_2);

  s21_create_matrix(4, 4, &matrix_1);
  s21_create_matrix(4, 4, &matrix_2);
  s21_create_matrix(4, 4, &answer);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      matrix_1.matrix[i][j] = i + j;
      answer.matrix[i][j] = i + j;
    }
  }
  res = s21_sub_matrix(&matrix_1, &matrix_2, &result);
  ck_assert_int_eq(res, 0);
  res = s21_eq_matrix(&result, &answer);
  ck_assert_int_eq(res, 1);
  s21_remove_matrix(&matrix_1);
  s21_remove_matrix(&matrix_2);
  s21_remove_matrix(&result);
  s21_remove_matrix(&answer);

  s21_create_matrix(4, 4, &matrix_1);
  s21_create_matrix(4, 4, &matrix_2);
  s21_create_matrix(4, 4, &answer);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      matrix_1.matrix[i][j] = i + j;
      matrix_2.matrix[i][j] = i + j;
    }
  }
  res = s21_sub_matrix(&matrix_1, &matrix_2, &result);
  ck_assert_int_eq(res, 0);
  res = s21_eq_matrix(&result, &answer);
  ck_assert_int_eq(res, 1);
  s21_remove_matrix(&matrix_1);
  s21_remove_matrix(&matrix_2);
  s21_remove_matrix(&result);
  s21_remove_matrix(&answer);
}
END_TEST

START_TEST(mult_1) {
  matrix_t mat_1, mat_2, res_mat, answer;

  s21_create_matrix(2, 2, &mat_1);
  s21_create_matrix(2, 2, &mat_2);
  s21_create_matrix(2, 2, &answer);
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      mat_1.matrix[i][j] = i + j + 1;
      mat_2.matrix[i][j] = i + j + 1;
    }
  }
  answer.matrix[0][0] = 5;
  answer.matrix[0][1] = 8;
  answer.matrix[1][0] = 8;
  answer.matrix[1][1] = 13;
  int res = s21_mult_matrix(&mat_1, &mat_2, &res_mat);
  ck_assert_int_eq(res, 0);
  res = s21_eq_matrix(&res_mat, &answer);
  ck_assert_int_eq(res, 1);
  res = s21_mult_matrix(NULL, &mat_2, &res_mat);
  ck_assert_int_eq(res, INCORRECT_MATR);
  s21_remove_matrix(&mat_1);
  s21_remove_matrix(&mat_2);
  s21_remove_matrix(&res_mat);
  s21_remove_matrix(&answer);

  s21_create_matrix(3, 2, &mat_1);
  s21_create_matrix(2, 2, &mat_2);
  s21_create_matrix(3, 2, &answer);
  res = s21_mult_matrix(&mat_1, &mat_2, &res_mat);
  ck_assert_int_eq(res, 0);
  res = s21_eq_matrix(&res_mat, &answer);
  ck_assert_int_eq(res, 1);
  s21_remove_matrix(&mat_1);
  s21_remove_matrix(&mat_2);
  s21_remove_matrix(&res_mat);
  s21_remove_matrix(&answer);

  s21_create_matrix(2, 7, &mat_1);
  s21_create_matrix(2, 2, &mat_2);
  res = s21_mult_matrix(&mat_1, &mat_2, &res_mat);
  ck_assert_int_eq(res, 2);
  s21_remove_matrix(&mat_1);
  s21_remove_matrix(&mat_2);
}
END_TEST

START_TEST(mult_number_1) {
  matrix_t matrix_1, result, answer;
  s21_create_matrix(4, 4, &matrix_1);
  s21_create_matrix(4, 4, &answer);
  int res = s21_mult_number(&matrix_1, 5, &result);
  ck_assert_int_eq(res, 0);
  res = s21_eq_matrix(&result, &answer);
  ck_assert_int_eq(res, 1);
  s21_remove_matrix(&matrix_1);
  s21_remove_matrix(&result);
  s21_remove_matrix(&answer);

  s21_create_matrix(4, 4, &matrix_1);
  s21_create_matrix(4, 4, &answer);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      matrix_1.matrix[i][j] = i + j;
      answer.matrix[i][j] = (i + j) * (-3);
    }
  }
  res = s21_mult_number(&matrix_1, -3, &result);
  ck_assert_int_eq(res, 0);
  res = s21_eq_matrix(&result, &answer);
  ck_assert_int_eq(res, 1);
  res = s21_mult_number(NULL, -3, &result);
  ck_assert_int_eq(res, INCORRECT_MATR);
  s21_remove_matrix(&result);
  s21_remove_matrix(&answer);

  s21_create_matrix(4, 4, &answer);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      matrix_1.matrix[i][j] = i + j;
    }
  }
  res = s21_mult_number(&matrix_1, 0, &result);
  ck_assert_int_eq(res, 0);
  res = s21_eq_matrix(&result, &answer);
  ck_assert_int_eq(res, 1);
  s21_remove_matrix(&matrix_1);
  s21_remove_matrix(&result);
  s21_remove_matrix(&answer);
}
END_TEST

START_TEST(transpose_1) {
  matrix_t mat_1, res_mat, answer;

  s21_create_matrix(2, 2, &mat_1);
  s21_create_matrix(2, 2, &answer);
  mat_1.matrix[0][0] = 1;
  mat_1.matrix[0][1] = 2;
  mat_1.matrix[1][0] = 3;
  mat_1.matrix[1][1] = 4;
  answer.matrix[0][0] = 1;
  answer.matrix[0][1] = 3;
  answer.matrix[1][0] = 2;
  answer.matrix[1][1] = 4;
  int res = s21_transpose(&mat_1, &res_mat);
  ck_assert_int_eq(res, 0);
  res = s21_eq_matrix(&res_mat, &answer);
  ck_assert_int_eq(res, 1);
  res = s21_transpose(NULL, &res_mat);
  ck_assert_int_eq(res, INCORRECT_MATR);
  s21_remove_matrix(&mat_1);
  s21_remove_matrix(&res_mat);
  s21_remove_matrix(&answer);

  double matrix[2][2] = {
      {1, 2},
      {3, 4},
  };
  double r[2][2] = {{1, 3}, {2, 4}};
  s21_create_matrix(2, 2, &mat_1);
  s21_create_matrix(2, 2, &answer);
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      mat_1.matrix[i][j] = matrix[i][j];
    }
  }
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      answer.matrix[i][j] = r[i][j];
    }
  }
  res = s21_transpose(&mat_1, &res_mat);
  ck_assert_int_eq(res, 0);
  res = s21_eq_matrix(&res_mat, &answer);
  ck_assert_int_eq(res, 1);
  s21_remove_matrix(&mat_1);
  s21_remove_matrix(&res_mat);
  s21_remove_matrix(&answer);
}
END_TEST

START_TEST(determinant_1) {
  matrix_t matrix;
  s21_create_matrix(3, 3, &matrix);
  int count = 1;
  for (int i = 0; i < matrix.rows; i++) {
    for (int j = 0; j < matrix.columns; j++) {
      matrix.matrix[i][j] = count++;
    }
  }
  matrix.matrix[2][2] = 10;
  double res = 0;
  int r = s21_determinant(&matrix, &res);
  ck_assert_uint_eq(res, -3);
  ck_assert_int_eq(r, 0);
  r = s21_determinant(NULL, &res);
  ck_assert_uint_eq(r, INCORRECT_MATR);
  s21_remove_matrix(&matrix);
  matrix_t sad;
  s21_create_matrix(1, 1, &sad);
  res = 0;
  sad.matrix[0][0] = 2;
  r = s21_determinant(&sad, &res);
  ck_assert_uint_eq(res, 2);
  ck_assert_int_eq(r, 0);
  s21_remove_matrix(&sad);
}
END_TEST

START_TEST(calc_complements_1) {
  matrix_t first, result, expected;
  s21_create_matrix(3, 2, &first);
  s21_create_matrix(3, 3, &expected);
  int r = 0;

  r = s21_calc_complements(NULL, &result);
  ck_assert_int_eq(INCORRECT_MATR, r);

  r = s21_calc_complements(&first, NULL);
  ck_assert_int_eq(INCORRECT_MATR, r);

  r = s21_calc_complements(&first, &result);
  ck_assert_int_eq(ERROR_ARIF, r);

  s21_create_matrix(3, 3, &first);

  first.matrix[0][0] = 2;
  first.matrix[1][0] = 5;
  first.matrix[2][0] = 3;
  first.matrix[0][1] = 1;
  first.matrix[1][1] = -5;
  first.matrix[2][1] = 3.3;
  first.matrix[0][2] = -1;
  first.matrix[1][2] = 0;
  first.matrix[2][2] = 0.3;
  expected.matrix[0][0] = -1.5;
  expected.matrix[0][1] = -1.5;
  expected.matrix[0][2] = 31.5;
  expected.matrix[1][0] = -3.6;
  expected.matrix[1][1] = 3.6;
  expected.matrix[1][2] = -3.6;
  expected.matrix[2][0] = -5;
  expected.matrix[2][1] = -5;
  expected.matrix[2][2] = -15;

  r = s21_calc_complements(&first, &result);
  ck_assert_int_eq(OK, r);
  r = s21_eq_matrix(&result, &expected);
  ck_assert_int_eq(SUCCESS, r);

  expected.matrix[1][2] = 100000;

  r = s21_calc_complements(&first, &result);
  ck_assert_int_eq(OK, r);
  r = s21_eq_matrix(&result, &expected);
  ck_assert_int_eq(FAILURE, r);

  first.rows = -3;
  r = s21_calc_complements(&first, &result);
  ck_assert_int_eq(INCORRECT_MATR, r);

  s21_remove_matrix(&first);
  s21_remove_matrix(&result);
  s21_remove_matrix(&expected);
}
END_TEST

START_TEST(inverse_1) {
  double m[3][3] = {{2, 5, 7}, {6, 3, 4}, {5, -2, -3}};
  double r[3][3] = {{1, -1, 1}, {-38, 41, -34}, {27, -29, 24}};
  matrix_t matrix, answer, result;
  s21_create_matrix(3, 3, &matrix);
  s21_create_matrix(3, 3, &answer);
  for (int i = 0; i < matrix.rows; i++) {
    for (int j = 0; j < matrix.columns; j++) {
      matrix.matrix[i][j] = m[i][j];
    }
  }
  for (int i = 0; i < matrix.rows; i++) {
    for (int j = 0; j < matrix.columns; j++) {
      answer.matrix[i][j] = r[i][j];
    }
  }
  int res = s21_inverse_matrix(&matrix, &result);
  ck_assert_int_eq(res, 0);
  res = s21_eq_matrix(&answer, &result);
  ck_assert_int_eq(res, 1);
  res = s21_inverse_matrix(NULL, &result);
  ck_assert_int_eq(res, INCORRECT_MATR);
  s21_remove_matrix(&matrix);
  s21_remove_matrix(&result);
  s21_remove_matrix(&answer);
}
END_TEST

Suite *s21_matrix_tests_create() {
  {
    Suite *s21_matrix = suite_create("s21_matrix");
    TCase *s21_matrix_tests = tcase_create("S21_MATRIX");
    tcase_add_test(s21_matrix_tests, create_1);
    tcase_add_test(s21_matrix_tests, eq_1);
    tcase_add_test(s21_matrix_tests, sum_1);
    tcase_add_test(s21_matrix_tests, sub_1);
    tcase_add_test(s21_matrix_tests, mult_1);
    tcase_add_test(s21_matrix_tests, mult_number_1);
    tcase_add_test(s21_matrix_tests, transpose_1);
    tcase_add_test(s21_matrix_tests, determinant_1);
    tcase_add_test(s21_matrix_tests, calc_complements_1);
    tcase_add_test(s21_matrix_tests, inverse_1);
    suite_add_tcase(s21_matrix, s21_matrix_tests);
    return s21_matrix;
  }
}

int main() {
  Suite *s21_matrix = s21_matrix_tests_create();
  SRunner *s21_matrix_runner = srunner_create(s21_matrix);
  int number_failed;
  srunner_run_all(s21_matrix_runner, CK_NORMAL);
  number_failed = srunner_ntests_failed(s21_matrix_runner);
  srunner_free(s21_matrix_runner);

  return number_failed == 0 ? 0 : 1;
}
