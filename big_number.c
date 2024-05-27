#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "big_number.h"
#include "my_string.h"

BigNumber* CreateBN(const char* number_)
{
    if (number_ == NULL || strlen(number_) == 0 || !IsIntBigNumber(number_))
        return NULL;

    BigNumber* bn = (BigNumber*)malloc(sizeof(BigNumber));
    if (bn == NULL)
        return NULL;

    size_t size = strlen(number_);
    bn->size = size;
    bn->is_negative = (number_[0] == '-');

    // Выделяем память под цифры числа
    bn->digits = (digit*)calloc(bn->size + 1, sizeof(digit)); // +1 для символа '\0'
    if (bn->digits == NULL)
    {
        free(bn);
        return NULL;
    }

    // Заполняем массив цифрами числа
    for (size_t i = 0; i < bn->size; ++i)
        bn->digits[i] = number_[i] - '0';

    return bn;
}

void DeleteBN(BigNumber** bn_)
{
    if (bn_ == NULL || *bn_ == NULL)
        return;

    free((*bn_)->digits);
    free(*bn_);

    *bn_ = NULL;
}

void PrintBN(const BigNumber* bn_)
{
    if (bn_ == NULL)
    {
        printf("BigNumber is empty!\n");
        return;
    }

    if (bn_->is_negative)
        printf("-");

    for (size_t i = 0; i < bn_->size; ++i)
        printf("%u", bn_->digits[i]);

    printf("\n");
}

bool IsZeroBN(BigNumber* bn_)
{
    // Если размер числа равен 1 и его единственная цифра равна нулю, то число ноль
    return (bn_->size == 1 && bn_->digits[0] == 0);
}

void SetBN(BigNumber* bn_, char* number_)
{
    // Освобождаем память, выделенную под цифры числа
    free(bn_->digits);

    size_t size = strlen(number_);
    if (*number_ == '-')
    {
        bn_->size = size - 1;
        bn_->is_negative = true;
        number_++;
    }
    else
    {
        bn_->size = size;
        bn_->is_negative = false;
    }

    // Выделяем память под массив цифр числа
    bn_->digits = (digit*)calloc(bn_->size, sizeof(digit));
    if (bn_->digits == NULL)
    {
        free(bn_);
        return;
    }

    // Заполняем массив цифр числа
    for (size_t i = 0; i < bn_->size; ++i)
        bn_->digits[i] = number_[i] - '0';
}

BigNumber* AppendDigitBN(BigNumber* bn_, digit digit_)
{
    bn_->size++;
    bn_->digits = (digit*)realloc(bn_->digits, bn_->size * sizeof(digit));
    if (bn_->digits == NULL)
    {
        // Ошибка выделения памяти
        return NULL;
    }
    bn_->digits[bn_->size - 1] = digit_;
    return bn_;
}

void ReverseBN(BigNumber* bn_)
{
    for (size_t i = 0; i < bn_->size / 2; ++i)
    {
        digit temp = bn_->digits[i];
        bn_->digits[i] = bn_->digits[bn_->size - i - 1];
        bn_->digits[bn_->size - i - 1] = temp;
    }
}

void SetDigitBN(BigNumber* bn_, size_t index, digit digit_)
{
    if (index < bn_->size)
    {
        bn_->digits[index] = digit_;
    }
}

BigNumber* SumBN(const BigNumber* bn1_, const BigNumber* bn2_)
{
    if (bn1_ == NULL || bn2_ == NULL)
        return NULL;

    // Проверяем, что оба числа целые
    if (!IsIntBigNumber(bn1_) || !IsIntBigNumber(bn2_))
        return NULL;

    // Определяем, какое число имеет большую длину
    const BigNumber* longer_bn = (bn1_->size >= bn2_->size) ? bn1_ : bn2_;
    const BigNumber* shorter_bn = (bn1_->size < bn2_->size) ? bn1_ : bn2_;

    // Создаем результирующее большое число
    BigNumber* result = (BigNumber*)malloc(sizeof(BigNumber));
    if (result == NULL)
        return NULL;

    // Выделяем память для массива цифр результата
    result->digits = (digit*)calloc(longer_bn->size + 1, sizeof(digit)); // Максимальная длина результата - на 1 больше максимальной длины из двух чисел
    if (result->digits == NULL)
    {
        free(result);
        return NULL;
    }

    // Инициализируем переменные для выполнения сложения
    int carry = 0; // Переменная для переноса разряда
    size_t i, j;
    int sum;

    // Учитываем различия в знаках слагаемых
    if (bn1_->is_negative == bn2_->is_negative)
    {
        // Если знаки одинаковые, результат будет таким же
        result->is_negative = bn1_->is_negative;
    }
    else
    {
        // Если знаки разные, необходимо выполнить операцию вычитания
        int compare_result = CompareBN(bn1_, bn2_);
        if (compare_result > 0)
        {
            // Если первое число по модулю больше второго, результат будет с тем же знаком, что и первое число
            result->is_negative = bn1_->is_negative;
        }
        else if (compare_result < 0)
        {
            // Если второе число по модулю больше первого, результат будет с тем же знаком, что и второе число
            result->is_negative = bn2_->is_negative;
            // Меняем местами числа для корректного выполнения операции вычитания
            const BigNumber* temp = bn1_;
            bn1_ = bn2_;
            bn2_ = temp;
        }
        else
        {
            // Если числа равны по модулю, результат равен нулю
            result->is_negative = false;
            result->digits[0] = 0;
            result->size = 1;
            return result;
        }
    }

    // Выполняем сложение цифр чисел пока не достигнем конца более короткого числа
    for (i = shorter_bn->size, j = longer_bn->size; i > 0; i--, j--)
    {
        // Складываем цифры и учитываем перенос
        sum = shorter_bn->digits[i - 1] + longer_bn->digits[j - 1] + carry;

        // Если сумма больше 9, вычитаем 10 и устанавливаем перенос
        if (sum > 9)
        {
            result->digits[j] = sum - 10;
            carry = 1;
        }
        else
        {
            result->digits[j] = sum;
            carry = 0;
        }
    }

    // Добавляем к оставшимся цифрам длинного числа перенос, если он есть
    for (; j > 0; j--)
    {
        sum = longer_bn->digits[j - 1] + carry;
        if (sum > 9)
        {
            result->digits[j] = sum - 10;
            carry = 1;
        }
        else
        {
            result->digits[j] = sum;
            carry = 0;
        }
    }

    // Если после сложения все еще остался перенос, увеличиваем размер результата и сдвигаем цифры
    if (carry > 0)
    {
        result->size = longer_bn->size + 1;
        memmove(result->digits + 1, result->digits, longer_bn->size);
        result->digits[0] = 1;
    }
    else
    {
        // Устанавливаем размер результата
        result->size = longer_bn->size;
    }

    return result;
}



BigNumber* SubBN(const BigNumber* bn1_, const BigNumber* bn2_)
{
    if (bn1_ == NULL || bn2_ == NULL)
        return NULL;

    // Проверка, являются ли числа целыми
    if (!IsIntBigNumber(bn1_) || !IsIntBigNumber(bn2_))
    {
        printf("Одно из чисел не является целым.\n");
        return NULL;
    }

    // Определяем, какое число имеет большую длину
    const BigNumber* longer_bn = (bn1_->size >= bn2_->size) ? bn1_ : bn2_;
    const BigNumber* shorter_bn = (bn1_->size < bn2_->size) ? bn1_ : bn2_;

    // Создаем результирующее большое число
    BigNumber* result = (BigNumber*)malloc(sizeof(BigNumber));
    if (result == NULL)
        return NULL;

    // Выделяем память для массива цифр результата
    result->digits = (digit*)calloc(longer_bn->size, sizeof(digit)); // Максимальная длина результата - максимальная длина из двух чисел
    if (result->digits == NULL)
    {
        free(result);
        return NULL;
    }

    // Инициализируем переменные для выполнения вычитания
    int borrow = 0; // Переменная для заема разряда
    size_t i, j;
    int diff;

    // Учитываем различия в знаках чисел
    if (bn1_->is_negative != bn2_->is_negative)
    {
        // Если знаки разные, результат будет с тем же знаком, что и первое число
        result->is_negative = bn1_->is_negative;
    }
    else
    {
        // Если знаки одинаковые, необходимо выполнить операцию вычитания
        int compare_result = CompareBN(bn1_, bn2_);
        if (compare_result > 0)
        {
            // Если первое число по модулю больше второго, результат будет с тем же знаком, что и первое число
            result->is_negative = bn1_->is_negative;
        }
        else if (compare_result < 0)
        {
            // Если второе число по модулю больше первого, результат будет с противоположным знаком
            result->is_negative = !bn1_->is_negative;
            // Меняем местами числа для корректного выполнения операции вычитания
            const BigNumber* temp = bn1_;
            bn1_ = bn2_;
            bn2_ = temp;
        }
        else
        {
            // Если числа равны по модулю, результат равен нулю
            result->is_negative = false;
            result->digits[0] = 0;
            result->size = 1;
            return result;
        }
    }

    // Выполняем вычитание цифр чисел пока не достигнем конца более короткого числа
    for (i = shorter_bn->size, j = longer_bn->size; i > 0; i--, j--)
    {
        // Вычитаем цифры и учитываем заем
        diff = longer_bn->digits[j - 1] - shorter_bn->digits[i - 1] - borrow;

        // Если разность меньше 0, добавляем 10 и устанавливаем заем
        if (diff < 0)
        {
            result->digits[j - 1] = diff + 10;
            borrow = 1;
        }
        else
        {
            result->digits[j - 1] = diff;
            borrow = 0;
        }
    }

    // Добавляем оставшиеся цифры длинного числа с учетом заема
    for (; j > 0; j--)
    {
        diff = longer_bn->digits[j - 1] - borrow;
        if (diff < 0)
        {
            result->digits[j - 1] = diff + 10;
            borrow = 1;
        }
        else
        {
            result->digits[j - 1] = diff;
            borrow = 0;
        }
    }

    // Убираем ведущие нули
    while (result->size > 1 && result->digits[0] == 0)
    {
        memmove(result->digits, result->digits + 1, result->size - 1);
        result->size--;
    }

    return result;
}


BigNumber* MultBN(const BigNumber* bn1_, const BigNumber* bn2_)
{
    if (bn1_ == NULL || bn2_ == NULL)
        return NULL;

    // Проверяем, являются ли входные строки целыми числами
    if (!IsIntBigNumber(bn1_) || !IsIntBigNumber(bn2_))
    {
        printf("Ошибка: Одно из чисел не является целым числом.\n");
        return NULL;
    }

    // Определяем знак результата умножения
    bool result_negative = (bn1_->is_negative != bn2_->is_negative);

    // Определяем длину результата умножения
    size_t result_length = bn1_->size + bn2_->size;

    // Выделяем память для массива цифр результата
    digit* result_digits = (digit*)calloc(result_length, sizeof(digit));
    if (result_digits == NULL)
        return NULL;

    // Инициализируем массив нулями
    for (size_t i = 0; i < result_length; ++i)
        result_digits[i] = 0;

    // Выполняем умножение цифр чисел
    for (size_t i = 0; i < bn1_->size; ++i)
    {
        for (size_t j = 0; j < bn2_->size; ++j)
        {
            size_t product_index = i + j; // Индекс результата для текущего произведения
            int product = bn1_->digits[i] * bn2_->digits[j]; // Произведение двух цифр

            // Добавляем произведение к соответствующему разряду результата
            while (product > 0)
            {
                product += result_digits[product_index]; // Добавляем текущее значение
                result_digits[product_index] = product % 10; // Сохраняем остаток
                product /= 10; // Оставшаяся часть для следующего разряда
                product_index++; // Переходим к следующему разряду
            }
        }
    }

    // Убираем ведущие нули из результата
    size_t result_size = result_length;
    while (result_size > 1 && result_digits[result_size - 1] == 0)
        result_size--;

    // Создаем структуру для хранения результата
    BigNumber* result = (BigNumber*)malloc(sizeof(BigNumber));
    if (result == NULL)
    {
        free(result_digits);
        return NULL;
    }

    // Заполняем структуру результатом умножения
    result->size = result_size;
    result->digits = result_digits;
    result->is_negative = result_negative;

    return result;
}

BigNumber* DivBN(const BigNumber* bn1_, const BigNumber* bn2_)
{
    if (bn1_ == NULL || bn2_ == NULL)
        return NULL;

    // Проверяем, что оба числа целые
    if (!IsIntBigNumber(bn1_) || !IsIntBigNumber(bn2_))
        return NULL;

    // Проверка на деление на ноль
    if (IsZeroBN(bn2_))
    {
        printf("Произошло деление на ноль.\n");
        return NULL;
    }

    // Определяем знак результата деления
    bool result_negative = (bn1_->is_negative != bn2_->is_negative);

    // Создаем копию делимого числа, чтобы не изменять оригинал
    BigNumber* dividend = CopyBN(bn1_);
    if (dividend == NULL)
        return NULL;

    // Создаем копию делителя числа, чтобы не изменять оригинал
    BigNumber* divisor = CopyBN(bn2_);
    if (divisor == NULL)
    {
        DeleteBN(&dividend);
        return NULL;
    }

    // Инициализируем результат деления нулем
    BigNumber* result = CreateBN("0");
    if (result == NULL)
    {
        DeleteBN(&dividend);
        DeleteBN(&divisor);
        return NULL;
    }

    // Для удобства переворачиваем числа, чтобы начинать деление с наибольших разрядов
    ReverseBN(dividend);
    ReverseBN(divisor);

    // Переменная для хранения текущего остатка от деления
    BigNumber* remainder = CreateBN("0");
    if (remainder == NULL)
    {
        DeleteBN(&dividend);
        DeleteBN(&divisor);
        DeleteBN(&result);
        return NULL;
    }

    // Проходим по всем разрядам делимого числа
    for (size_t i = 0; i < dividend->size; ++i)
    {
        // Умножаем остаток на 10 и добавляем следующий разряд из делимого числа
        remainder = MultBN(remainder, CreateBN("10"));
        SetDigitBN(remainder, 0, dividend->digits[i]);

        // Начинаем деление
        digit quotient_digit = 0;
        while (CompareBN(remainder, divisor) >= 0)
        {
            remainder = SubBN(remainder, divisor);
            quotient_digit++;
        }

        // Добавляем цифру частного в результат деления
        result = AppendDigitBN(result, quotient_digit);
    }

    // Освобождаем память из ненужных структур
    DeleteBN(&dividend);
    DeleteBN(&divisor);
    DeleteBN(&remainder);

    // Устанавливаем правильный знак результата
    result->is_negative = result_negative;

    return result;
}

BigNumber* CopyBN(BigNumber* bn_)
{
    if (bn_ == NULL)
        return NULL;

    BigNumber* copy = (BigNumber*)malloc(sizeof(BigNumber));
    if (copy == NULL)
        return NULL;

    copy->size = bn_->size;
    copy->is_negative = bn_->is_negative;
    copy->digits = (digit*)malloc(bn_->size * sizeof(digit));
    if (copy->digits == NULL)
    {
        free(copy);
        return NULL;
    }

    memcpy(copy->digits, bn_->digits, bn_->size * sizeof(digit));

    return copy;
}

BigNumber* ConcatenateBN(BigNumber* bn1_, BigNumber* bn2_)
{
    if (bn1_ == NULL || bn2_ == NULL)
        return NULL;

    size_t new_size = bn1_->size + bn2_->size;

    BigNumber* result = (BigNumber*)malloc(sizeof(BigNumber));
    if (result == NULL)
        return NULL;

    result->size = new_size;
    result->is_negative = bn1_->is_negative; // Сохраняем знак первого числа

    result->digits = (digit*)malloc(new_size * sizeof(digit));
    if (result->digits == NULL)
    {
        free(result);
        return NULL;
    }

    // Копируем цифры первого числа
    for (size_t i = 0; i < bn1_->size; ++i)
        result->digits[i] = bn1_->digits[i];

    // Копируем цифры второго числа
    for (size_t i = 0; i < bn2_->size; ++i)
        result->digits[bn1_->size + i] = bn2_->digits[i];

    return result;
}

int CompareBN(BigNumber* bn1_, BigNumber* bn2_)
{
    if (bn1_ == NULL || bn2_ == NULL)
        return 0;

    // Проверяем знаки чисел
    if (bn1_->is_negative && !bn2_->is_negative)
        return -1;
    else if (!bn1_->is_negative && bn2_->is_negative)
        return 1;

    // Сравниваем количество цифр в числах
    if (bn1_->size > bn2_->size)
        return 1;
    else if (bn1_->size < bn2_->size)
        return -1;

    // Сравниваем цифры чисел слева направо
    for (size_t i = 0; i < bn1_->size; ++i)
    {
        if (bn1_->digits[i] > bn2_->digits[i])
            return 1;
        else if (bn1_->digits[i] < bn2_->digits[i])
            return -1;
    }

    // Числа равны
    return 0;
}

BigNumber* SumMultipleBN(int num_numbers, BigNumber** numbers)
{
    if (num_numbers <= 0 || numbers == NULL)
        return NULL;

    // Находим максимальный размер суммы чисел
    size_t max_size = 0;
    for (int i = 0; i < num_numbers; ++i)
    {
        if (numbers[i]->size > max_size)
            max_size = numbers[i]->size;
    }

    // Создаем новый BigNumber для результата
    BigNumber* result = (BigNumber*)malloc(sizeof(BigNumber));
    if (result == NULL)
        return NULL;

    // Выделяем память под цифры результата
    result->size = max_size + 1; // Максимальный размер суммы плюс одна цифра для возможного переноса
    result->digits = (digit*)calloc(result->size, sizeof(digit));
    if (result->digits == NULL)
    {
        free(result);
        return NULL;
    }

    int carry = 0; // Перенос

    // Пока есть цифры в одном из чисел или есть перенос
    for (size_t digit_index = result->size - 1; digit_index > 0; --digit_index)
    {
        int sum = carry;

        // Суммируем цифры в данном разряде из всех чисел
        for (int i = 0; i < num_numbers; ++i)
        {
            if (numbers[i]->size > digit_index - 1)
                sum += numbers[i]->digits[numbers[i]->size - digit_index];
        }

        // Если сумма больше 9, есть перенос
        carry = sum / 10;

        // Записываем остаток от деления суммы на 10 в текущую цифру результата
        result->digits[digit_index] = sum % 10;
    }

    // Учитываем знак результата
    result->is_negative = numbers[0]->is_negative; // Знак первого числа

    return result;
}


BigNumber* SubMultipleBN(int num_numbers, BigNumber** numbers)
{
    if (num_numbers <= 0 || numbers == NULL)
        return NULL;

    // Находим максимальный размер разности чисел
    size_t max_size = 0;
    for (int i = 0; i < num_numbers; ++i)
    {
        if (numbers[i]->size > max_size)
            max_size = numbers[i]->size;
    }

    // Создаем новый BigNumber для результата
    BigNumber* result = (BigNumber*)malloc(sizeof(BigNumber));
    if (result == NULL)
        return NULL;

    // Выделяем память под цифры результата
    result->size = max_size;
    result->digits = (digit*)calloc(result->size, sizeof(digit));
    if (result->digits == NULL)
    {
        free(result);
        return NULL;
    }

    int borrow = 0; // Заем

    // Пока есть цифры в одном из чисел
    for (size_t digit_index = result->size - 1; digit_index > 0; --digit_index)
    {
        int sub = borrow;

        // Вычитаем цифры в данном разряде из первого числа
        if (numbers[0]->size > digit_index - 1)
            sub += numbers[0]->digits[numbers[0]->size - digit_index];

        // Вычитаем цифры в данном разряде из остальных чисел
        for (int i = 1; i < num_numbers; ++i)
        {
            if (numbers[i]->size > digit_index - 1)
                sub -= numbers[i]->digits[numbers[i]->size - digit_index];
        }

        // Если разность меньше нуля, берем заем из следующей более старшей разряда
        if (sub < 0)
        {
            sub += 10;
            borrow = -1;
        }
        else
            borrow = 0;

        // Записываем остаток от вычитания в текущую цифру результата
        result->digits[digit_index] = sub;
    }

    // Учитываем знак результата
    result->is_negative = numbers[0]->is_negative; // Знак первого числа

    return result;
}

BigNumber* MultMultipleBN(int num_numbers, BigNumber** numbers)
{
    if (num_numbers <= 0 || numbers == NULL)
        return NULL;

    // Находим максимальную длину результата умножения
    size_t max_result_size = 0;
    for (int i = 0; i < num_numbers; ++i)
    {
        max_result_size += numbers[i]->size;
    }

    // Создаем новый BigNumber для результата
    BigNumber* result = (BigNumber*)malloc(sizeof(BigNumber));
    if (result == NULL)
        return NULL;

    // Выделяем память под цифры результата
    result->size = max_result_size + 1; // Максимальный размер результата плюс одна цифра для возможного переноса
    result->digits = (digit*)calloc(result->size, sizeof(digit));
    if (result->digits == NULL)
    {
        free(result);
        return NULL;
    }

    // Инициализируем результат нулями
    for (size_t i = 0; i < result->size; ++i)
        result->digits[i] = 0;

    // Выполняем умножение всех чисел
    for (int i = 0; i < num_numbers; ++i)
    {
        BigNumber* temp_result = MultBN(result, numbers[i]);
        if (temp_result == NULL)
        {
            // Если произошла ошибка при умножении, освобождаем ресурсы и возвращаем NULL
            DeleteBN(&result);
            return NULL;
        }
        // Освобождаем память, выделенную для предыдущего результата
        free(result->digits);
        free(result);
        result = temp_result;
    }

    return result;
}

BigNumber* DivMultipleBN(int num_numbers, BigNumber** numbers)
{
    if (num_numbers <= 0 || numbers == NULL)
        return NULL;

    // Создаем копию первого числа
    BigNumber* result = CopyBN(numbers[0]);

    // Выполняем деление на каждое последующее число
    for (int i = 1; i < num_numbers; ++i)
    {
        BigNumber* temp_result = result;
        result = DivBN(temp_result, numbers[i]);
        DeleteBN(&temp_result);
    }

    return result;
}