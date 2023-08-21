#ifndef SRC_S21_DECIMAL_H_
#define SRC_S21_DECIMAL_H_

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define NOI 3                         // numbers of integers
#define NOB sizeof(unsigned int) * 8  // numbers of bits
#define BIT 1
#define SIGN_MASK 0x80000000  // starshiy bit
#define DEGREE_MASK 0xFF0000
#define DEGREE_FLOAT_MASK 0x7f800000
#define FLOAT_MASK 0x7fffff

typedef struct {
  unsigned int bits[4];
} s21_decimal;

typedef struct {
  unsigned int bits[8];
} s21_big_decimal;

enum error_code_arithmetic {
  OK = 0,
  TOO_LARGE = 1,
  TOO_SMALL = 2,
  DIV_BY_ZERO = 3
};

enum comparison_code { FALSE = 0, TRUE = 1 };

enum convertation_code_error { CONVERT_ERROR = 1 };

/***********************************
    === functions from s21_add ===
************************************/

// Менеджерит разпределение на сумму и вычитание
s21_decimal s21_sum_integer(s21_decimal first, s21_decimal second);

// Производит сумму decimal вне зависимости от знака и степени
s21_decimal s21_sum_processing(s21_decimal first, s21_decimal second,
                               s21_decimal result, int buff, unsigned res);

// Оригинальная функция склеивания
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

//проверяет для округления
int s21_fcheck(s21_decimal *integer, s21_decimal mantissa);

/**********************************
    === function from s21_sub ===
**********************************/

// Вычитает 2 decimal. Получает первый decimal ОБЯЗАТЕЛЬНО больше второго
s21_decimal s21_sub_processing(s21_decimal first, s21_decimal second,
                               s21_decimal result, int buff, unsigned res,
                               s21_decimal once);

// Менеждерит распределение на разность и сумму
s21_decimal s21_subtraction(s21_decimal first, s21_decimal second);

// Оригинальная сумма
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

/************************************
    === function from s21_split ===
************************************/

// Вспомогательная функция для разделения на integer и mantissa
int s21_minus_decimal(s21_decimal a, s21_decimal b, s21_decimal *result);

// Разбивает target на 2 части - целую и дробную
int s21_split_decimal(s21_decimal target, s21_decimal *integer,
                      s21_decimal *mantissa);

// Склеивает 2 части decimal
void s21_merge_decimal(s21_decimal integer, s21_decimal mantissa,
                       s21_decimal *target);

/**************************************
    === function from s21_compare ===
**************************************/

// TODO Вспомогательная функция определяющая больше ли target, чем cmp
int s21_integer_greater(s21_decimal target, s21_decimal cmp);

// TODO Вспомогательная функция определяющая меньше ли target, чем cmp
int s21_integer_less(s21_decimal target, s21_decimal cmp);

int s21_is_equal(s21_decimal first, s21_decimal second);

// Проверяет две структуры на пустоту в первых трех интах
int s21_compare_zero_decimals(s21_decimal first, s21_decimal second);

// Проверяет две структуры не равны ли они
int s21_is_not_equal(s21_decimal first, s21_decimal second);

int s21_is_greater_or_equal(s21_decimal first, s21_decimal second);

int s21_is_greater(s21_decimal first, s21_decimal second);

int s21_is_less(s21_decimal first, s21_decimal second);

int s21_is_less_or_equal(s21_decimal first, s21_decimal second);
// int s21_is_equal(s21_decimal val_one, s21_decimal val_two);

/***************************************
    === function from s21_degree ===
***************************************/

// устанавливает степень 10 в decimal
void s21_set_degree(s21_decimal *val, int degree);

// возвращает степень 10 в decimal
int s21_get_degree(s21_decimal val);

/***************************************
    === function from s21_help ===
***************************************/

int s21_assert(s21_decimal calculated, s21_decimal expected);

void s21_decimal_init(s21_decimal *target);

int s21_shift_left_1(s21_decimal *val);
int s21_shift_right_1(s21_decimal *val);

unsigned s21_get_bit(s21_decimal val, unsigned pos);

// находит старший бит в 96.  -1 если все три инта пустые
int s21_find_hight_bit(s21_decimal val);

void s21_set_bit(s21_decimal *val, unsigned pos);

// Возвращает 1, если знак отрицательный и 0 - если результат положительный
int s21_get_sign(s21_decimal target);

// Устанавливает в приходящий decimal значение знака sign
void s21_set_sign(s21_decimal *target, int sign);

int s21_get_number_of_ints(s21_decimal val);

int s21_check_zero(s21_decimal val);
// меняет в числе бит в позиции pos начиная с 0  на значение val
int s21_modifyBit(int num, int pos, int val);

int s21_tests(s21_decimal result, s21_decimal answer);
/***************************************
    === function from s21_convertors.c ===
***************************************/

int s21_from_int_to_decimal(int src, s21_decimal *dst);

int s21_from_float_to_decimal(float src, s21_decimal *dst);

int s21_from_decimal_to_int(s21_decimal src, int *dst);

int s21_from_decimal_to_float(s21_decimal src, float *dst);

/***************************************
    === function from s21_div ===
***************************************/
// Делит in на 10
void s21_div_to_10(s21_decimal *in);
// Деление двух decimal
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

/***************************************
    === function from s21_mul ===
***************************************/

s21_decimal s21_mul_ten(s21_decimal val, int degree);

s21_decimal s21_mul_ten_overflow(s21_decimal val, int *degree);

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

/***************************************
    === function from s21_mod ===
***************************************/

int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

/***************************************
    === function from s21_round ===
***************************************/

// округляет до целого
int s21_round(s21_decimal value, s21_decimal *result);

// округляет до целого, но в сторону отрицательной бесконечности
int s21_floor(s21_decimal value, s21_decimal *result);

// умножает на минус один
int s21_negate(s21_decimal value, s21_decimal *result);

// Отделяет целую часть decimal
int s21_truncate(s21_decimal value, s21_decimal *result);

/***************************************
    === function from s21_big_decimal ===
***************************************/
void s21_shift_big_decimal(s21_big_decimal *factor, s21_decimal value, int j);

int s21_truncate_big(s21_big_decimal value, s21_big_decimal *result,
                     int degree);

void s21_div_to_10_big(s21_big_decimal *in);

s21_big_decimal s21_add_big_decimal(s21_big_decimal first,
                                    s21_big_decimal second,
                                    s21_big_decimal result);

int s21_get_number_of_ints_big(s21_big_decimal val);

#endif  // SRC_S21_DECIMAL_H_
