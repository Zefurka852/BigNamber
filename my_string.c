#include <stdio.h>
#include <stdlib.h>

#include "my_string.h"
#include "big_number.h"

bool IsIntBigNumber(const BigNumber* bn_)
{
    // Проверка на пустой указатель
    if (bn_ == NULL)
        return false;

    // Проверяем каждую цифру числа
    for (size_t i = 0; i < bn_->size; ++i)
    {
        if (bn_->digits[i] < 0 || bn_->digits[i] > 9)
        {
            return false;
        }
    }

    return true;
}