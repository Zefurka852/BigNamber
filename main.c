#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

#include "big_number.h"

int main()
{
    setlocale(LC_ALL, "Russian");

    char str1[1000], str2[1000];

    printf("Введите первое большое число: ");
    scanf_s("%999s", str1, (unsigned)_countof(str1));

    printf("Введите второе большое число: ");
    scanf_s("%999s", str2, (unsigned)_countof(str2));

    BigNumber* bn1 = CreateBN(str1);
    BigNumber* bn2 = CreateBN(str2);

    if (bn1 == NULL || bn2 == NULL) {
        printf("Одно из больших чисел равно NULL!\n");
        return 1;
    }

    while (1) {
        PrintMenu();
        int choice;
        scanf_s("%d", &choice);

        BigNumber* result = NULL;

        switch (choice) {
        case 1:
            result = SumBN(bn1, bn2);
            if (result != NULL) {
                printf("Сумма: ");
                PrintBN(result);
                DeleteBN(&result);
            }
            break;
        case 2:
            result = SubtractBN(bn1, bn2);
            if (result != NULL) {
                printf("Разность: ");
                PrintBN(result);
                DeleteBN(&result);
            }
            break;
        case 3:
            result = MultiplyBN(bn1, bn2);
            if (result != NULL) {
                printf("Произведение: ");
                PrintBN(result);
                DeleteBN(&result);
            }
            break;
        case 4:
            result = DivideBN(bn1, bn2);
            if (result != NULL) {
                printf("Частное: ");
                PrintBN(result);
                DeleteBN(&result);
            }
            break;
        case 5:
            DeleteBN(&bn1);
            DeleteBN(&bn2);
            printf("Выход из программы.\n");
            return 0;
        default:
            printf("Неверный выбор. Пожалуйста, попробуйте снова.\n");
        }
    }

    return 0;
}