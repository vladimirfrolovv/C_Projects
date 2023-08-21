#include "s21_matrix.h"

int s21_create_matrix(int rows, int columns, matrix_t *result) {
  int status = OK;
  if (result != NULL && rows > 0 && columns > 0) {
    result->rows = rows;
    result->columns = columns;
    result->matrix = (double **)malloc(rows * sizeof(double *));
    for (int i = 0; i < rows; i++) {
      result->matrix[i] = (double *)malloc(columns * sizeof(double));
    }
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < columns; j++) {
        result->matrix[i][j] = 0;
      }
    }

  } else {
    status = INCORRECT_MATR;
  }
  return status;
}

void s21_remove_matrix(matrix_t *A) {
  if (s21_check_matrix(A)) {
    for (int i = 0; i < A->rows; i++) {
      free(A->matrix[i]);
    }
    free(A->matrix);
    A->matrix = NULL;
    A->rows = 0;
    A->columns = 0;
  }
}

int s21_eq_matrix(matrix_t *A, matrix_t *B) {
  int status = SUCCESS;
  if (s21_check_matrix(A) && s21_check_matrix(B) && A->rows == B->rows &&
      A->columns == B->columns) {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        if (fabs(A->matrix[i][j] - B->matrix[i][j]) > 1e-07) {
          status = FAILURE;
        }
      }
    }
  } else {
    status = FAILURE;
  }

  return status;
}

int s21_check_matrix(matrix_t *M) {
  int status = SUCCESS;
  if (M == NULL || M->rows <= 0 || M->columns <= 0) {
    status = FAILURE;
  }
  return status;
}

int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int status = OK;
  if (s21_check_matrix(A) && s21_check_matrix(B) && result != NULL) {
    if (A->rows == B->rows && A->columns == B->columns) {
      status = s21_create_matrix(A->rows, A->columns, result);
      if (status == 0) {
        for (int i = 0; i < A->rows; i++) {
          for (int j = 0; j < A->columns; j++) {
            result->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
          }
        }
      }
    } else {
      status = ERROR_ARIF;
    }
  } else {
    status = INCORRECT_MATR;
  }
  return status;
}

int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int status = OK;
  if (s21_check_matrix(A) && s21_check_matrix(B) && result != NULL) {
    if (A->rows == B->rows && A->columns == B->columns) {
      status = s21_create_matrix(A->rows, A->columns, result);
      if (status == 0) {
        for (int i = 0; i < A->rows; i++) {
          for (int j = 0; j < A->columns; j++) {
            result->matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
          }
        }
      }
    } else {
      status = ERROR_ARIF;
    }
  } else {
    status = INCORRECT_MATR;
  }
  return status;
}

int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int status = OK;
  if (s21_check_matrix(A) && s21_check_matrix(B) && result != NULL) {
    if (A->columns == B->rows) {
      status = s21_create_matrix(A->rows, B->columns, result);
      if (status == 0) {
        for (int i = 0; i < result->rows; i++) {
          for (int j = 0; j < result->columns; j++) {
            for (int k = 0; k < A->columns; k++) {
              result->matrix[i][j] += A->matrix[i][k] * B->matrix[k][j];
            }
          }
        }
      }
    } else {
      status = ERROR_ARIF;
    }
  } else {
    status = INCORRECT_MATR;
  }
  return status;
}

int s21_mult_number(matrix_t *A, double number, matrix_t *result) {
  int status = OK;
  if (s21_check_matrix(A) && result != NULL) {
    status = s21_create_matrix(A->rows, A->columns, result);
    if (status == 0) {
      for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < A->columns; j++) {
          result->matrix[i][j] = A->matrix[i][j] * number;
        }
      }
    }

  } else {
    status = INCORRECT_MATR;
  }
  return status;
}

int s21_transpose(matrix_t *A, matrix_t *result) {
  int status = OK;
  if (s21_check_matrix(A) && result != NULL) {
    status = s21_create_matrix(A->columns, A->rows, result);
    if (status == 0) {
      for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < A->columns; j++) {
          result->matrix[j][i] = A->matrix[i][j];
        }
      }
    }

  } else {
    status = INCORRECT_MATR;
  }
  return status;
}

void Minor(int a, int b, matrix_t *A, matrix_t *minor_mat) {
  s21_create_matrix(A->rows - 1, A->columns - 1, minor_mat);
  int i_m = 0;
  for (int i = 0; i < A->rows; i++) {
    int j_m = 0;
    if (i == a) continue;
    for (int j = 0; j < A->columns; j++) {
      if (j != b) {
        minor_mat->matrix[i_m][j_m] = A->matrix[i][j];
        j_m++;
      }
    }
    i_m++;
  }
}

int s21_determinant(matrix_t *A, double *result) {
  int status = OK;
  if (s21_check_matrix(A) && result != NULL && A != NULL) {
    if (A->rows == A->columns) {
      *result = s21_determinant_rec(A);
    } else {
      status = ERROR_ARIF;
    }
  } else {
    status = INCORRECT_MATR;
  }
  return status;
}

double s21_determinant_rec(matrix_t *A) {
  double result = 0;
  matrix_t minor_mat = {0};
  if (A->rows == 1) {
    result = A->matrix[0][0];
  } else if (A->rows == 2) {
    result =
        A->matrix[0][0] * A->matrix[1][1] - A->matrix[0][1] * A->matrix[1][0];
  } else {
    for (int i = 0; i < A->rows; i++) {
      Minor(0, i + 1 - 1, A, &minor_mat);
      result +=
          pow((-1), i) * A->matrix[0][i] * s21_determinant_rec(&minor_mat);
      s21_remove_matrix(&minor_mat);
    }
  }
  return result;
}

int s21_calc_complements(matrix_t *A, matrix_t *result) {
  int status = OK;
  matrix_t minor_mat = {0};
  if (s21_check_matrix(A) && result != NULL) {
    status = s21_create_matrix(A->rows, A->columns, result);
    if (status == 0) {
      if (A->rows == A->columns) {
        double det = 0;
        for (int i = 0; i < A->rows; i++) {
          for (int j = 0; j < A->columns; j++) {
            Minor(i, j, A, &minor_mat);
            s21_determinant(&minor_mat, &det);
            result->matrix[i][j] = pow((-1), i + j) * det;
            s21_remove_matrix(&minor_mat);
          }
        }

      } else {
        status = ERROR_ARIF;
      }
    }
  } else {
    status = INCORRECT_MATR;
  }
  return status;
}

int s21_inverse_matrix(matrix_t *A, matrix_t *result) {
  int status = 0;
  if (s21_check_matrix(A) && result != NULL) {
    if (A->rows == A->columns) {
      double det = 0;
      s21_determinant(A, &det);
      if (fabs(det - 0) > 1e-7) {
        matrix_t tmp_calc = {0};
        s21_calc_complements(A, &tmp_calc);
        matrix_t tmp_trans = {0};
        s21_transpose(&tmp_calc, &tmp_trans);
        s21_mult_number(&tmp_trans, 1 / det, result);
        s21_remove_matrix(&tmp_trans);
        s21_remove_matrix(&tmp_calc);
      } else {
        status = ERROR_ARIF;
      }
    } else {
      status = ERROR_ARIF;
    }
  } else {
    status = INCORRECT_MATR;
  }
  return status;
}