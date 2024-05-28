#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "big_number.h"
#include "my_string.h"

BigNumber* CreateBN(const char* number_) {
    if (number_ == NULL || strlen(number_) == 0 || !IsIntString(number_)) {
        return NULL;
    }

    BigNumber* bn = (BigNumber*)malloc(sizeof(BigNumber));
    if (bn == NULL) {
        return NULL;
    }

    size_t size = strlen(number_);
    if (number_[0] == '-') {
        bn->size = size - 1;
        bn->is_negative = true;
        number_++;
    }
    else {
        bn->size = size;
        bn->is_negative = false;
    }

    bn->digits = (digit*)calloc(bn->size, sizeof(digit));
    if (bn->digits == NULL) {
        free(bn);
        return NULL;
    }

    for (size_t i = 0; i < bn->size; ++i) {
        bn->digits[i] = number_[i] - '0';
    }

    return bn;
}

void DeleteBN(BigNumber** bn_) {
    if (bn_ == NULL || *bn_ == NULL) {
        return;
    }

    free((*bn_)->digits);
    free(*bn_);
    *bn_ = NULL;
}

void PrintBN(const BigNumber* bn_) {
    if (bn_ == NULL) {
        printf("BigNumber is empty!\n");
        return;
    }

    if (bn_->is_negative) {
        printf("-");
    }

    for (size_t i = 0; i < bn_->size; ++i) {
        printf("%u", bn_->digits[i]);
    }

    printf("\n");
}

BigNumber* SumBN(BigNumber* bn1, BigNumber* bn2) {
    if (bn1 == NULL || bn2 == NULL)
        return NULL;

    // Если оба числа отрицательные или оба положительные
    if (bn1->is_negative == bn2->is_negative) {
        BigNumber* result = (BigNumber*)malloc(sizeof(BigNumber));
        if (result == NULL)
            return NULL;

        size_t max_size = (bn1->size > bn2->size) ? bn1->size : bn2->size;
        result->digits = (digit*)calloc(max_size + 1, sizeof(digit)); // +1 для возможного переноса
        if (result->digits == NULL) {
            free(result);
            return NULL;
        }

        result->size = max_size + 1; // Максимально возможный размер результата
        result->is_negative = bn1->is_negative; // Знак результата такой же, как и у слагаемых

        int carry = 0;
        for (int i = 0; i < result->size; ++i) {
            int digit1 = (i < bn1->size) ? bn1->digits[bn1->size - 1 - i] : 0;
            int digit2 = (i < bn2->size) ? bn2->digits[bn2->size - 1 - i] : 0;

            int sum = digit1 + digit2 + carry;
            result->digits[result->size - 1 - i] = sum % 10;
            carry = sum / 10;
        }

        // Удаляем ведущие нули
        while (result->size > 1 && result->digits[0] == 0) {
            memmove(result->digits, result->digits + 1, --result->size);
        }

        return result;
    }
    else {
        // Если одно из чисел отрицательное, используем вычитание
        if (bn1->is_negative) {
            bn1->is_negative = false;
            BigNumber* result = SubtractBN(bn2, bn1);
            bn1->is_negative = true;
            return result;
        }
        else {
            bn2->is_negative = false;
            BigNumber* result = SubtractBN(bn1, bn2);
            bn2->is_negative = true;
            return result;
        }
    }
}

BigNumber* SubtractBN(BigNumber* bn1, BigNumber* bn2) {
    if (bn1 == NULL || bn2 == NULL)
        return NULL;

    // Если оба числа имеют одинаковый знак
    if (bn1->is_negative == bn2->is_negative) {
        bool result_negative = false;
        BigNumber* larger = NULL;
        BigNumber* smaller = NULL;

        // Определяем большее по абсолютной величине число
        if (bn1->size > bn2->size || (bn1->size == bn2->size && memcmp(bn1->digits, bn2->digits, bn1->size * sizeof(digit)) >= 0)) {
            larger = bn1;
            smaller = bn2;
            result_negative = bn1->is_negative;
        }
        else {
            larger = bn2;
            smaller = bn1;
            result_negative = !bn1->is_negative;
        }

        BigNumber* result = (BigNumber*)malloc(sizeof(BigNumber));
        if (result == NULL)
            return NULL;

        result->digits = (digit*)calloc(larger->size, sizeof(digit));
        if (result->digits == NULL) {
            free(result);
            return NULL;
        }

        result->size = larger->size;
        result->is_negative = result_negative;

        int borrow = 0;
        for (int i = 0; i < result->size; ++i) {
            int digit1 = (i < larger->size) ? larger->digits[larger->size - 1 - i] : 0;
            int digit2 = (i < smaller->size) ? smaller->digits[smaller->size - 1 - i] : 0;

            int diff = digit1 - digit2 - borrow;
            if (diff < 0) {
                diff += 10;
                borrow = 1;
            }
            else {
                borrow = 0;
            }

            result->digits[result->size - 1 - i] = diff;
        }

        // Удаляем ведущие нули
        while (result->size > 1 && result->digits[0] == 0) {
            memmove(result->digits, result->digits + 1, --result->size);
        }

        return result;
    }
    else {
        // Если одно из чисел отрицательное, выполняем сложение их абсолютных значений
        if (bn1->is_negative) {
            bn1->is_negative = false;
            BigNumber* result = SumBN(bn1, bn2);
            bn1->is_negative = true;
            result->is_negative = true;
            return result;
        }
        else {
            bn2->is_negative = false;
            BigNumber* result = SumBN(bn1, bn2);
            bn2->is_negative = true;
            return result;
        }
    }
}

BigNumber* MultiplyBN(const BigNumber* bn1_, const BigNumber* bn2_) {
    if (bn1_ == NULL || bn2_ == NULL) {
        return NULL;
    }

    BigNumber* result = (BigNumber*)malloc(sizeof(BigNumber));
    if (result == NULL) {
        return NULL;
    }

    result->size = bn1_->size + bn2_->size;
    result->digits = (digit*)calloc(result->size, sizeof(digit));
    if (result->digits == NULL) {
        free(result);
        return NULL;
    }
    result->is_negative = bn1_->is_negative != bn2_->is_negative;

    for (int i = bn1_->size - 1; i >= 0; i--) {
        int carry = 0;
        for (int j = bn2_->size - 1; j >= 0; j--) {
            int prod = bn1_->digits[i] * bn2_->digits[j] + result->digits[i + j + 1] + carry;
            carry = prod / 10;
            result->digits[i + j + 1] = prod % 10;
        }
        result->digits[i] += carry;
    }

    // Remove leading zeros
    size_t non_zero_index = 0;
    while (non_zero_index < result->size && result->digits[non_zero_index] == 0) {
        non_zero_index++;
    }
    if (non_zero_index == result->size) {
        result->size = 1;
        result->digits[0] = 0;
    }
    else if (non_zero_index > 0) {
        memmove(result->digits, result->digits + non_zero_index, result->size - non_zero_index);
        result->size -= non_zero_index;
    }

    return result;
}
BigNumber* DivideBN(BigNumber* bn1, BigNumber* bn2) {
    if (bn1 == NULL || bn2 == NULL || bn2->size == 0)
        return NULL;

    BigNumber* result = (BigNumber*)malloc(sizeof(BigNumber));
    if (result == NULL)
        return NULL;

    // Учитываем знаки чисел
    result->is_negative = (bn1->is_negative != bn2->is_negative);

    // Определяем знаки чисел для корректного вычисления остатка
    bool dividend_negative = bn1->is_negative;
    bool divisor_negative = bn2->is_negative;
    bn1->is_negative = false;
    bn2->is_negative = false;

    // Преобразуем большие числа в массивы целых чисел
    int* dividend = (int*)malloc(bn1->size * sizeof(int));
    int* divisor = (int*)malloc(bn2->size * sizeof(int));
    if (dividend == NULL || divisor == NULL) {
        free(result);
        free(dividend);
        free(divisor);
        return NULL;
    }

    // Заполняем массивы цифрами чисел в обратном порядке
    for (size_t i = 0; i < bn1->size; ++i)
        dividend[i] = bn1->digits[bn1->size - 1 - i];
    for (size_t i = 0; i < bn2->size; ++i)
        divisor[i] = bn2->digits[bn2->size - 1 - i];

    // Выполняем деление
    int quotient;
    int remainder;
    long long temp = 0;

    // Определяем начальный размер частного
    size_t quotient_size = bn1->size - bn2->size + 1;

    // Выделяем память под массив частного
    result->digits = (digit*)malloc(quotient_size * sizeof(digit));
    if (result->digits == NULL) {
        free(result);
        free(dividend);
        free(divisor);
        return NULL;
    }

    for (size_t i = 0; i < quotient_size; ++i) {
        // Вычисляем следующую цифру частного
        temp = temp * 10 + dividend[bn2->size + i - 1];
        quotient = temp / divisor[0];
        remainder = temp % divisor[0];
        result->digits[quotient_size - 1 - i] = quotient;

        // Обновляем остаток
        temp = remainder;
    }

    // Удаляем ведущие нули
    while (quotient_size > 1 && result->digits[quotient_size - 1] == 0)
        quotient_size--;

    // Задаем размер частного
    result->size = quotient_size;

    // Освобождаем выделенную память
    free(dividend);
    free(divisor);

    // Восстанавливаем знаки чисел
    bn1->is_negative = dividend_negative;
    bn2->is_negative = divisor_negative;

    return result;
}
