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

    // �������� ������ ��� ����� �����
    bn->digits = (digit*)calloc(bn->size + 1, sizeof(digit)); // +1 ��� ������� '\0'
    if (bn->digits == NULL)
    {
        free(bn);
        return NULL;
    }

    // ��������� ������ ������� �����
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
    // ���� ������ ����� ����� 1 � ��� ������������ ����� ����� ����, �� ����� ����
    return (bn_->size == 1 && bn_->digits[0] == 0);
}

void SetBN(BigNumber* bn_, char* number_)
{
    // ����������� ������, ���������� ��� ����� �����
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

    // �������� ������ ��� ������ ���� �����
    bn_->digits = (digit*)calloc(bn_->size, sizeof(digit));
    if (bn_->digits == NULL)
    {
        free(bn_);
        return;
    }

    // ��������� ������ ���� �����
    for (size_t i = 0; i < bn_->size; ++i)
        bn_->digits[i] = number_[i] - '0';
}

BigNumber* AppendDigitBN(BigNumber* bn_, digit digit_)
{
    bn_->size++;
    bn_->digits = (digit*)realloc(bn_->digits, bn_->size * sizeof(digit));
    if (bn_->digits == NULL)
    {
        // ������ ��������� ������
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

    // ���������, ��� ��� ����� �����
    if (!IsIntBigNumber(bn1_) || !IsIntBigNumber(bn2_))
        return NULL;

    // ����������, ����� ����� ����� ������� �����
    const BigNumber* longer_bn = (bn1_->size >= bn2_->size) ? bn1_ : bn2_;
    const BigNumber* shorter_bn = (bn1_->size < bn2_->size) ? bn1_ : bn2_;

    // ������� �������������� ������� �����
    BigNumber* result = (BigNumber*)malloc(sizeof(BigNumber));
    if (result == NULL)
        return NULL;

    // �������� ������ ��� ������� ���� ����������
    result->digits = (digit*)calloc(longer_bn->size + 1, sizeof(digit)); // ������������ ����� ���������� - �� 1 ������ ������������ ����� �� ���� �����
    if (result->digits == NULL)
    {
        free(result);
        return NULL;
    }

    // �������������� ���������� ��� ���������� ��������
    int carry = 0; // ���������� ��� �������� �������
    size_t i, j;
    int sum;

    // ��������� �������� � ������ ���������
    if (bn1_->is_negative == bn2_->is_negative)
    {
        // ���� ����� ����������, ��������� ����� ����� ��
        result->is_negative = bn1_->is_negative;
    }
    else
    {
        // ���� ����� ������, ���������� ��������� �������� ���������
        int compare_result = CompareBN(bn1_, bn2_);
        if (compare_result > 0)
        {
            // ���� ������ ����� �� ������ ������ �������, ��������� ����� � ��� �� ������, ��� � ������ �����
            result->is_negative = bn1_->is_negative;
        }
        else if (compare_result < 0)
        {
            // ���� ������ ����� �� ������ ������ �������, ��������� ����� � ��� �� ������, ��� � ������ �����
            result->is_negative = bn2_->is_negative;
            // ������ ������� ����� ��� ����������� ���������� �������� ���������
            const BigNumber* temp = bn1_;
            bn1_ = bn2_;
            bn2_ = temp;
        }
        else
        {
            // ���� ����� ����� �� ������, ��������� ����� ����
            result->is_negative = false;
            result->digits[0] = 0;
            result->size = 1;
            return result;
        }
    }

    // ��������� �������� ���� ����� ���� �� ��������� ����� ����� ��������� �����
    for (i = shorter_bn->size, j = longer_bn->size; i > 0; i--, j--)
    {
        // ���������� ����� � ��������� �������
        sum = shorter_bn->digits[i - 1] + longer_bn->digits[j - 1] + carry;

        // ���� ����� ������ 9, �������� 10 � ������������� �������
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

    // ��������� � ���������� ������ �������� ����� �������, ���� �� ����
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

    // ���� ����� �������� ��� ��� ������� �������, ����������� ������ ���������� � �������� �����
    if (carry > 0)
    {
        result->size = longer_bn->size + 1;
        memmove(result->digits + 1, result->digits, longer_bn->size);
        result->digits[0] = 1;
    }
    else
    {
        // ������������� ������ ����������
        result->size = longer_bn->size;
    }

    return result;
}



BigNumber* SubBN(const BigNumber* bn1_, const BigNumber* bn2_)
{
    if (bn1_ == NULL || bn2_ == NULL)
        return NULL;

    // ��������, �������� �� ����� ������
    if (!IsIntBigNumber(bn1_) || !IsIntBigNumber(bn2_))
    {
        printf("���� �� ����� �� �������� �����.\n");
        return NULL;
    }

    // ����������, ����� ����� ����� ������� �����
    const BigNumber* longer_bn = (bn1_->size >= bn2_->size) ? bn1_ : bn2_;
    const BigNumber* shorter_bn = (bn1_->size < bn2_->size) ? bn1_ : bn2_;

    // ������� �������������� ������� �����
    BigNumber* result = (BigNumber*)malloc(sizeof(BigNumber));
    if (result == NULL)
        return NULL;

    // �������� ������ ��� ������� ���� ����������
    result->digits = (digit*)calloc(longer_bn->size, sizeof(digit)); // ������������ ����� ���������� - ������������ ����� �� ���� �����
    if (result->digits == NULL)
    {
        free(result);
        return NULL;
    }

    // �������������� ���������� ��� ���������� ���������
    int borrow = 0; // ���������� ��� ����� �������
    size_t i, j;
    int diff;

    // ��������� �������� � ������ �����
    if (bn1_->is_negative != bn2_->is_negative)
    {
        // ���� ����� ������, ��������� ����� � ��� �� ������, ��� � ������ �����
        result->is_negative = bn1_->is_negative;
    }
    else
    {
        // ���� ����� ����������, ���������� ��������� �������� ���������
        int compare_result = CompareBN(bn1_, bn2_);
        if (compare_result > 0)
        {
            // ���� ������ ����� �� ������ ������ �������, ��������� ����� � ��� �� ������, ��� � ������ �����
            result->is_negative = bn1_->is_negative;
        }
        else if (compare_result < 0)
        {
            // ���� ������ ����� �� ������ ������ �������, ��������� ����� � ��������������� ������
            result->is_negative = !bn1_->is_negative;
            // ������ ������� ����� ��� ����������� ���������� �������� ���������
            const BigNumber* temp = bn1_;
            bn1_ = bn2_;
            bn2_ = temp;
        }
        else
        {
            // ���� ����� ����� �� ������, ��������� ����� ����
            result->is_negative = false;
            result->digits[0] = 0;
            result->size = 1;
            return result;
        }
    }

    // ��������� ��������� ���� ����� ���� �� ��������� ����� ����� ��������� �����
    for (i = shorter_bn->size, j = longer_bn->size; i > 0; i--, j--)
    {
        // �������� ����� � ��������� ����
        diff = longer_bn->digits[j - 1] - shorter_bn->digits[i - 1] - borrow;

        // ���� �������� ������ 0, ��������� 10 � ������������� ����
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

    // ��������� ���������� ����� �������� ����� � ������ �����
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

    // ������� ������� ����
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

    // ���������, �������� �� ������� ������ ������ �������
    if (!IsIntBigNumber(bn1_) || !IsIntBigNumber(bn2_))
    {
        printf("������: ���� �� ����� �� �������� ����� ������.\n");
        return NULL;
    }

    // ���������� ���� ���������� ���������
    bool result_negative = (bn1_->is_negative != bn2_->is_negative);

    // ���������� ����� ���������� ���������
    size_t result_length = bn1_->size + bn2_->size;

    // �������� ������ ��� ������� ���� ����������
    digit* result_digits = (digit*)calloc(result_length, sizeof(digit));
    if (result_digits == NULL)
        return NULL;

    // �������������� ������ ������
    for (size_t i = 0; i < result_length; ++i)
        result_digits[i] = 0;

    // ��������� ��������� ���� �����
    for (size_t i = 0; i < bn1_->size; ++i)
    {
        for (size_t j = 0; j < bn2_->size; ++j)
        {
            size_t product_index = i + j; // ������ ���������� ��� �������� ������������
            int product = bn1_->digits[i] * bn2_->digits[j]; // ������������ ���� ����

            // ��������� ������������ � ���������������� ������� ����������
            while (product > 0)
            {
                product += result_digits[product_index]; // ��������� ������� ��������
                result_digits[product_index] = product % 10; // ��������� �������
                product /= 10; // ���������� ����� ��� ���������� �������
                product_index++; // ��������� � ���������� �������
            }
        }
    }

    // ������� ������� ���� �� ����������
    size_t result_size = result_length;
    while (result_size > 1 && result_digits[result_size - 1] == 0)
        result_size--;

    // ������� ��������� ��� �������� ����������
    BigNumber* result = (BigNumber*)malloc(sizeof(BigNumber));
    if (result == NULL)
    {
        free(result_digits);
        return NULL;
    }

    // ��������� ��������� ����������� ���������
    result->size = result_size;
    result->digits = result_digits;
    result->is_negative = result_negative;

    return result;
}

BigNumber* DivBN(const BigNumber* bn1_, const BigNumber* bn2_)
{
    if (bn1_ == NULL || bn2_ == NULL)
        return NULL;

    // ���������, ��� ��� ����� �����
    if (!IsIntBigNumber(bn1_) || !IsIntBigNumber(bn2_))
        return NULL;

    // �������� �� ������� �� ����
    if (IsZeroBN(bn2_))
    {
        printf("��������� ������� �� ����.\n");
        return NULL;
    }

    // ���������� ���� ���������� �������
    bool result_negative = (bn1_->is_negative != bn2_->is_negative);

    // ������� ����� �������� �����, ����� �� �������� ��������
    BigNumber* dividend = CopyBN(bn1_);
    if (dividend == NULL)
        return NULL;

    // ������� ����� �������� �����, ����� �� �������� ��������
    BigNumber* divisor = CopyBN(bn2_);
    if (divisor == NULL)
    {
        DeleteBN(&dividend);
        return NULL;
    }

    // �������������� ��������� ������� �����
    BigNumber* result = CreateBN("0");
    if (result == NULL)
    {
        DeleteBN(&dividend);
        DeleteBN(&divisor);
        return NULL;
    }

    // ��� �������� �������������� �����, ����� �������� ������� � ���������� ��������
    ReverseBN(dividend);
    ReverseBN(divisor);

    // ���������� ��� �������� �������� ������� �� �������
    BigNumber* remainder = CreateBN("0");
    if (remainder == NULL)
    {
        DeleteBN(&dividend);
        DeleteBN(&divisor);
        DeleteBN(&result);
        return NULL;
    }

    // �������� �� ���� �������� �������� �����
    for (size_t i = 0; i < dividend->size; ++i)
    {
        // �������� ������� �� 10 � ��������� ��������� ������ �� �������� �����
        remainder = MultBN(remainder, CreateBN("10"));
        SetDigitBN(remainder, 0, dividend->digits[i]);

        // �������� �������
        digit quotient_digit = 0;
        while (CompareBN(remainder, divisor) >= 0)
        {
            remainder = SubBN(remainder, divisor);
            quotient_digit++;
        }

        // ��������� ����� �������� � ��������� �������
        result = AppendDigitBN(result, quotient_digit);
    }

    // ����������� ������ �� �������� ��������
    DeleteBN(&dividend);
    DeleteBN(&divisor);
    DeleteBN(&remainder);

    // ������������� ���������� ���� ����������
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
    result->is_negative = bn1_->is_negative; // ��������� ���� ������� �����

    result->digits = (digit*)malloc(new_size * sizeof(digit));
    if (result->digits == NULL)
    {
        free(result);
        return NULL;
    }

    // �������� ����� ������� �����
    for (size_t i = 0; i < bn1_->size; ++i)
        result->digits[i] = bn1_->digits[i];

    // �������� ����� ������� �����
    for (size_t i = 0; i < bn2_->size; ++i)
        result->digits[bn1_->size + i] = bn2_->digits[i];

    return result;
}

int CompareBN(BigNumber* bn1_, BigNumber* bn2_)
{
    if (bn1_ == NULL || bn2_ == NULL)
        return 0;

    // ��������� ����� �����
    if (bn1_->is_negative && !bn2_->is_negative)
        return -1;
    else if (!bn1_->is_negative && bn2_->is_negative)
        return 1;

    // ���������� ���������� ���� � ������
    if (bn1_->size > bn2_->size)
        return 1;
    else if (bn1_->size < bn2_->size)
        return -1;

    // ���������� ����� ����� ����� �������
    for (size_t i = 0; i < bn1_->size; ++i)
    {
        if (bn1_->digits[i] > bn2_->digits[i])
            return 1;
        else if (bn1_->digits[i] < bn2_->digits[i])
            return -1;
    }

    // ����� �����
    return 0;
}

BigNumber* SumMultipleBN(int num_numbers, BigNumber** numbers)
{
    if (num_numbers <= 0 || numbers == NULL)
        return NULL;

    // ������� ������������ ������ ����� �����
    size_t max_size = 0;
    for (int i = 0; i < num_numbers; ++i)
    {
        if (numbers[i]->size > max_size)
            max_size = numbers[i]->size;
    }

    // ������� ����� BigNumber ��� ����������
    BigNumber* result = (BigNumber*)malloc(sizeof(BigNumber));
    if (result == NULL)
        return NULL;

    // �������� ������ ��� ����� ����������
    result->size = max_size + 1; // ������������ ������ ����� ���� ���� ����� ��� ���������� ��������
    result->digits = (digit*)calloc(result->size, sizeof(digit));
    if (result->digits == NULL)
    {
        free(result);
        return NULL;
    }

    int carry = 0; // �������

    // ���� ���� ����� � ����� �� ����� ��� ���� �������
    for (size_t digit_index = result->size - 1; digit_index > 0; --digit_index)
    {
        int sum = carry;

        // ��������� ����� � ������ ������� �� ���� �����
        for (int i = 0; i < num_numbers; ++i)
        {
            if (numbers[i]->size > digit_index - 1)
                sum += numbers[i]->digits[numbers[i]->size - digit_index];
        }

        // ���� ����� ������ 9, ���� �������
        carry = sum / 10;

        // ���������� ������� �� ������� ����� �� 10 � ������� ����� ����������
        result->digits[digit_index] = sum % 10;
    }

    // ��������� ���� ����������
    result->is_negative = numbers[0]->is_negative; // ���� ������� �����

    return result;
}


BigNumber* SubMultipleBN(int num_numbers, BigNumber** numbers)
{
    if (num_numbers <= 0 || numbers == NULL)
        return NULL;

    // ������� ������������ ������ �������� �����
    size_t max_size = 0;
    for (int i = 0; i < num_numbers; ++i)
    {
        if (numbers[i]->size > max_size)
            max_size = numbers[i]->size;
    }

    // ������� ����� BigNumber ��� ����������
    BigNumber* result = (BigNumber*)malloc(sizeof(BigNumber));
    if (result == NULL)
        return NULL;

    // �������� ������ ��� ����� ����������
    result->size = max_size;
    result->digits = (digit*)calloc(result->size, sizeof(digit));
    if (result->digits == NULL)
    {
        free(result);
        return NULL;
    }

    int borrow = 0; // ����

    // ���� ���� ����� � ����� �� �����
    for (size_t digit_index = result->size - 1; digit_index > 0; --digit_index)
    {
        int sub = borrow;

        // �������� ����� � ������ ������� �� ������� �����
        if (numbers[0]->size > digit_index - 1)
            sub += numbers[0]->digits[numbers[0]->size - digit_index];

        // �������� ����� � ������ ������� �� ��������� �����
        for (int i = 1; i < num_numbers; ++i)
        {
            if (numbers[i]->size > digit_index - 1)
                sub -= numbers[i]->digits[numbers[i]->size - digit_index];
        }

        // ���� �������� ������ ����, ����� ���� �� ��������� ����� ������� �������
        if (sub < 0)
        {
            sub += 10;
            borrow = -1;
        }
        else
            borrow = 0;

        // ���������� ������� �� ��������� � ������� ����� ����������
        result->digits[digit_index] = sub;
    }

    // ��������� ���� ����������
    result->is_negative = numbers[0]->is_negative; // ���� ������� �����

    return result;
}

BigNumber* MultMultipleBN(int num_numbers, BigNumber** numbers)
{
    if (num_numbers <= 0 || numbers == NULL)
        return NULL;

    // ������� ������������ ����� ���������� ���������
    size_t max_result_size = 0;
    for (int i = 0; i < num_numbers; ++i)
    {
        max_result_size += numbers[i]->size;
    }

    // ������� ����� BigNumber ��� ����������
    BigNumber* result = (BigNumber*)malloc(sizeof(BigNumber));
    if (result == NULL)
        return NULL;

    // �������� ������ ��� ����� ����������
    result->size = max_result_size + 1; // ������������ ������ ���������� ���� ���� ����� ��� ���������� ��������
    result->digits = (digit*)calloc(result->size, sizeof(digit));
    if (result->digits == NULL)
    {
        free(result);
        return NULL;
    }

    // �������������� ��������� ������
    for (size_t i = 0; i < result->size; ++i)
        result->digits[i] = 0;

    // ��������� ��������� ���� �����
    for (int i = 0; i < num_numbers; ++i)
    {
        BigNumber* temp_result = MultBN(result, numbers[i]);
        if (temp_result == NULL)
        {
            // ���� ��������� ������ ��� ���������, ����������� ������� � ���������� NULL
            DeleteBN(&result);
            return NULL;
        }
        // ����������� ������, ���������� ��� ����������� ����������
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

    // ������� ����� ������� �����
    BigNumber* result = CopyBN(numbers[0]);

    // ��������� ������� �� ������ ����������� �����
    for (int i = 1; i < num_numbers; ++i)
    {
        BigNumber* temp_result = result;
        result = DivBN(temp_result, numbers[i]);
        DeleteBN(&temp_result);
    }

    return result;
}