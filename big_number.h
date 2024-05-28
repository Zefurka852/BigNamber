#ifndef BIG_NUMBER_H
#define BIG_NUMBER_H

#include <stdbool.h>

typedef unsigned char digit;

// Тип большого числа (>10 знаков)
typedef struct BigNumber {
    unsigned int size;      // Размер числа
    digit* digits;          // Массив из цифр числа
    bool is_negative;       // Знак числа
} BigNumber;

/*
 * @brief Создание большого числа
 * @param number_ : Строка с большим числом
 * @return Указатель на выделенную память с большим числом
 */
BigNumber* CreateBN(const char* number_);

/*
 * @brief Удаляет выбранное большое число
 * @param bn_ : Указатель на большое число
 */
void DeleteBN(BigNumber** bn_);

/*
 * @brief Печать большого числа
 * @param bn_ : Указатель на большое число
 */
void PrintBN(const BigNumber* bn_);

/*
 * @brief Сумма двух больших чисел
 * @param bn1_, bn2_ : Большие числа
 * @return Большое число из суммы двух входных
 */
BigNumber* SumBN(const BigNumber* bn1_, const BigNumber* bn2_);

/*
 * @brief Разность двух больших чисел
 * @param bn1_, bn2_ : Большие числа
 * @return Большое число из разности двух входных
 */
BigNumber* SubtractBN(const BigNumber* bn1_, const BigNumber* bn2_);

/*
 * @brief Умножение двух больших чисел
 * @param bn1_, bn2_ : Большие числа
 * @return Большое число из произведения двух входных
 */
BigNumber* MultiplyBN(const BigNumber* bn1_, const BigNumber* bn2_);

/*
 * @brief Деление двух больших чисел
 * @param bn1_, bn2_ : Большие числа
 * @return Частное от деления двух входных чисел
 */
BigNumber* DivideBN(const BigNumber* bn1_, const BigNumber* bn2_);

#endif // BIG_NUMBER_H
