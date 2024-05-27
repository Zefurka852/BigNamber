#ifndef BIG_NUMBER_H
#define BIG_NUMBER_H

#include <stdbool.h>

typedef unsigned char digit;

// Тип большого числа (>10 знаков)
typedef struct BigNumber
{
    unsigned int size;      // Размер числа
    digit* digits;          // Массив из цифр числа
    bool is_negative;       // Знак числа
} BigNumber;

/*
* @brief Создание большого числа
* @param number_ : Строка с большим числом
* @return Указатель на выделенную память с большим числом
*/
BigNumber* CreateBN(char* number_);

/*
* @brief Удаляет выбранное большое число
* @param bn_ : Указатель на большое число
*/
void DeleteBN(BigNumber** bn_);

/*
* @brief Печать большого числа
* @param bn_ : Указатель на большое число
*/
void PrintBN(BigNumber* bn_);

/*
* @brief Сумма двух больших чисел
* @param bn1_, bn2_ : Большие числа
* @return Большое число из суммы двух входных
*/
BigNumber* SumBN(BigNumber* bn1_, BigNumber* bn2_);

/*
* @brief Разность двух больших чисел
* @param bn1_, bn2_ : Большие числа
* @return Большое число из разности двух входных
*/
BigNumber* SubBN(BigNumber* bn1_, BigNumber* bn2_);

/*
* @brief Умножение двух больших чисел
* @param bn1_, bn2_ : Большие числа
* @return Большое число из произведения двух входных
*/
BigNumber* MultBN(BigNumber* bn1_, BigNumber* bn2_);

/*
* @brief Деление двух больших чисел
* @param bn1_, bn2_ : Большие числа
* @return Большое число от деления первого на второе
*/
BigNumber* DivBN(BigNumber* bn1_, BigNumber* bn2_);

/*
* @brief Создает копию большого числа
* @param bn_ : Исходное большое число
* @return Указатель на копию исходного большого числа
*/
BigNumber* CopyBN(BigNumber* bn_);

/*
* @brief Объединяет два больших числа в одно
* @param bn1_, bn2_ : Большие числа
* @return Результат объединения двух входных больших чисел
*/
BigNumber* ConcatenateBN(BigNumber* bn1_, BigNumber* bn2_);

/*
* @brief Сравнивает два больших числа
* @param bn1_, bn2_ : Большие числа
* @return 1 если первое число больше, -1 если второе, 0 если равны
*/
int CompareBN(BigNumber* bn1_, BigNumber* bn2_);

/*
* @brief Сумма нескольких больших чисел
* @param bn_arr : Массив указателей на большие числа
* @param count : Количество чисел в массиве
* @return Результат суммы всех чисел в массиве
*/
BigNumber* SumMultipleBN(BigNumber** bn_arr, size_t count);

/*
* @brief Разность нескольких больших чисел
* @param bn_arr : Массив указателей на большие числа
* @param count : Количество чисел в массиве
* @return Результат разности всех чисел в массиве
*/
BigNumber* SubMultipleBN(BigNumber** bn_arr, size_t count);

/*
* @brief Произведение нескольких больших чисел
* @param bn_arr : Массив указателей на большие числа
* @param count : Количество чисел в массиве
* @return Результат произведения всех чисел в массиве
*/
BigNumber* MultMultipleBN(BigNumber** bn_arr, size_t count);

/*
* @brief Частное от деления нескольких больших чисел
* @param bn_arr : Массив указателей на большие числа
* @param count : Количество чисел в массиве
* @return Результат частного от деления всех чисел в массиве
*/
BigNumber* DivMultipleBN(BigNumber** bn_arr, size_t count);

/*
* @brief Проверяет, является ли большое число нулевым
* @param bn_ : Большое число
* @return true, если число нулевое, иначе false
*/
bool IsZeroBN(BigNumber* bn_);

/*
* @brief Устанавливает значение большого числа из строки
* @param bn_ : Большое число
* @param number_ : Строка с числом
*/
void SetBN(BigNumber* bn_, char* number_);

/*
* @brief Добавляет цифру к большому числу
* @param bn_ : Большое число
* @param digit_ : Цифра, которую нужно добавить
* @return Указатель на обновленное большое число
*/
BigNumber* AppendDigitBN(BigNumber* bn_, digit digit_);

/*
* @brief Переворачивает порядок цифр в большом числе
* @param bn_ : Большое число
*/
void ReverseBN(BigNumber* bn_);

/*
* @brief Устанавливает значение определенной цифры в большом числе
* @param bn_ : Большое число
* @param index : Индекс цифры, которую нужно установить
* @param digit_ : Новое значение цифры
*/
void SetDigitBN(BigNumber* bn_, size_t index, digit digit_);

#endif /* BIG_NUMBER_H */