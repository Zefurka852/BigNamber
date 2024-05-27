#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

#include "big_number.h"

int main()
{
    setlocale(LC_ALL, "Russian");

    size_t count;
    printf("������� ���������� ������� �����: ");
    scanf_s("%zu", &count);

    BigNumber** bn_arr = (BigNumber**)malloc(count * sizeof(BigNumber*));
    if (bn_arr == NULL)
    {
        printf("������ ��������� ������ ��� ������� BigNumber.\n");
        return 1;
    }

    for (size_t i = 0; i < count; ++i)
    {
        char number[1001]; // �������� ������ ������ �� 1 ��� ����� ������������ ����
        printf("������� ������� ����� %zu: ", i + 1);
        scanf_s("%1000s", number, (unsigned)_countof(number)); // ������������� ����������� ����� �� 1000 ��������
        bn_arr[i] = CreateBN(number);
        if (bn_arr[i] == NULL)
        {
            printf("������ ��� �������� BigNumber �� �����.\n");
            for (size_t j = 0; j < i; ++j)
                DeleteBN(&bn_arr[j]);
            free(bn_arr);
            return 1;
        }
    }

    BigNumber* sum_result = SumMultipleBN(bn_arr, count);
    if (sum_result != NULL)
    {
        printf("��������� ��������: ");
        PrintBN(sum_result);
        DeleteBN(&sum_result);
    }
    else
    {
        printf("������ ��� ���������� ��������.\n");
    }

    BigNumber* sub_result = SubMultipleBN(bn_arr, count);
    if (sub_result != NULL)
    {
        printf("��������� ���������: ");
        PrintBN(sub_result);
        DeleteBN(&sub_result);
    }
    else
    {
        printf("������ ��� ���������� ���������.\n");
    }

    BigNumber* mult_result = MultMultipleBN(bn_arr, count);
    if (mult_result != NULL)
    {
        printf("��������� ���������: ");
        PrintBN(mult_result);
        DeleteBN(&mult_result);
    }
    else
    {
        printf("������ ��� ���������� ���������.\n");
    }

    BigNumber* div_result = DivMultipleBN(bn_arr, count);
    if (div_result != NULL)
    {
        printf("��������� �������: ");
        PrintBN(div_result);
        DeleteBN(&div_result);
    }
    else
    {
        printf("��������� ������� �� ����.\n");
    }

    for (size_t i = 0; i < count; ++i)
        DeleteBN(&bn_arr[i]);
    free(bn_arr);

    return 0;
}
