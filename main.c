#include <stdio.h>
#include <stdlib.h>
#define MAX_DIGITS 309

typedef struct
{
    int *number;
    int length;
    int sign;
} BigInt;

BigInt add(BigInt a, BigInt b);
BigInt subtract(BigInt a, BigInt b);

int max(int a, int b)
{
    return a > b ? a : b;
}

int strlen(char *str)
{
    int i = 0;
    while (str[i] != '\0')
        i++;
    return i;
}

BigInt create_BigInt(char *s)
{
    BigInt bigInt;

    int len = strlen(s);
    bigInt.sign = s[0] == '-' ? 1 : 0;
    bigInt.length = len - bigInt.sign;
    bigInt.number = (int *)malloc(sizeof(int) * len);

    int *ptr = bigInt.number;

    for (int i = len - 1; i >= bigInt.sign; i--)
    {
        *(ptr) = s[i] - 48;
        (ptr)++;
    }

    return bigInt;
}

void display(BigInt bigInt)
{
    if (bigInt.length > MAX_DIGITS)
        printf("ERROR: Overflow");
    else
    {
        if (bigInt.sign)
            printf("-");
        for (int i = bigInt.length - 1; i >= 0; i--)
        {
            if (0 <= bigInt.number[i] && bigInt.number[i] <= 9)
                printf("%d", bigInt.number[i]);
        }
    }
    printf("\n\n");
}

int compare(BigInt a, BigInt b)
{
    int res;

    if (!a.sign && b.sign)
        res = 1;
    else if (a.sign && !b.sign)
        res = -1;
    else
    {
        if (a.length > b.length)
            res = 1;
        else if (a.length < b.length)
            res = -1;
        else
        {
            int i = a.length - 1;
            while (i >= 0 && a.number[i] == b.number[i])
                i--;

            if (i < 0)
                res = 0;
            else
            {
                if (a.number[i] > b.number[i])
                    res = 1;
                else
                    res = -1;
            }
        }
        if (a.sign && b.sign)
        {
            if (res == 1)
                res = -1;
            else if (res == -1)
                res = 1;
        }
    }
    return res;
}

BigInt subtract(BigInt a, BigInt b)
{
    BigInt res;
    int cmp = compare(a, b);

    if (!a.sign && !b.sign)
    {
        int i = 0, j = 0;
        if (cmp == -1)
            res.sign = 1;
        else
            res.sign = 0;

        res.length = max(a.length, b.length);
        res.number = (int *)malloc(sizeof(int) * res.length);
        int newVal;
        int first = -1, second = -1;
        int *ptr = res.number;

        while (i < a.length || j < b.length)
        {
            if (i < a.length)
                first = a.number[i++];

            if (j < b.length)
                second = b.number[j++];

            if (cmp == 1)
            {
                if (first != -1 && second != -1)
                {
                    if (first >= second)
                        newVal = first - second;
                    else
                    {
                        first += 10;
                        if (i < a.length)
                            a.number[i]--;
                        newVal = first - second;
                    }
                }
                else
                    newVal = first;
            }
            else
            {
                if (first != -1 && second != -1)
                {

                    if (first <= second)
                        newVal = second - first;
                    else
                    {
                        second += 10;
                        if (i < b.length)
                            b.number[j]--;
                        newVal = second - first;
                    }
                }
                else
                    newVal = second;
            }
            *(ptr) = newVal;
            ptr++;
            first = second = -1;
        }
    }
    else
    {
        if (!a.sign && b.sign)
            b.sign = 0;
        else if (a.sign && !b.sign)
            b.sign = 1;
        else
        {
            b.sign = 0;
            subtract(b, a);
        }
        res = add(a, b);
    }

    int i = res.length - 1;
    while (res.number[i] == 0)
    {
        i--;
        res.length--;
    }

    if (res.length == 0)
    {
        res.length = 1;
        res.sign = 0;
    }

    return res;
}

BigInt add(BigInt a, BigInt b)
{
    BigInt res;

    if (a.length > b.length)
        res.length = a.length + 1;
    else
        res.length = b.length + 1;

    res.number = (int *)malloc(sizeof(int) * res.length);
    int *ptr = res.number;

    if (a.sign == b.sign)
    {
        int carry = 0;
        int newVal;
        int i = 0, j = 0;
        res.sign = a.sign;

        while (i < a.length || j < b.length || carry != 0)
        {
            newVal = carry;

            if (i < a.length)
                newVal += a.number[i++];
            if (j < b.length)
                newVal += b.number[j++];

            carry = newVal / 10;
            newVal = newVal % 10;

            *(ptr) = newVal;
            ptr++;
        }
    }
    else
    {
        if (!a.sign && b.sign)
        {
            b.sign = 0;
            res = subtract(a, b);
        }
        else
        {
            a.sign = 0;
            res = subtract(b, a);
        }
    }

    int i = res.length - 1;
    while (res.number[i] == 0)
    {
        i--;
        res.length--;
    }

    if (res.length == 0)
    {
        res.length = 1;
        res.sign = 0;
    }

    return res;
}

BigInt multiply(BigInt a, BigInt b)
{
    BigInt product;
    product.length = a.length + b.length;

    product.number = (int *)malloc(sizeof(int) * (product.length));
    int i, j, carry, prod;

    for (i = 0; i < product.length; i++)
        product.number[i] = 0;

    for (i = 0; i < a.length; i++)
    {
        carry = 0;
        for (j = 0; j < b.length; j++)
        {
            prod = (a.number[i] * b.number[j]) + carry;
            carry = prod / 10;
            product.number[i + j] += (prod % 10);
            if (product.number[i + j] > 9)
            {
                product.number[i + j + 1] += 1;
                (product.number[i + j]) %= 10;
            }
        }
        if (carry != 0)
        {
            product.number[i + j] += (carry % 10);
            if (product.number[i + j] > 9)
            {
                product.number[i + j + 1] += 1;
                (product.number[i + j]) %= 10;
            }
        }
    }

    if (a.sign == b.sign)
        product.sign = 0;
    else
        product.sign = 1;

    i = product.length - 1;
    while (product.number[i] == 0)
    {
        i--;
        (product.length)--;
    }
    if (product.length == 0)
    {
        product.sign = 0;
        product.length = 1;
    }

    return product;
}

int main()
{
    char str1[MAX_DIGITS];
    char str2[MAX_DIGITS];
    printf("\nEnter 1st Number: \n");
    scanf("%s", str1);
    printf("\nEnter 2nd Number: \n");
    scanf("%s", str2);

    if (strlen(str1) > MAX_DIGITS || strlen(str2) > MAX_DIGITS)
        printf("ERROR: Overflow");
    else
    {
        BigInt a = create_BigInt(str1);
        BigInt b = create_BigInt(str2);
        printf("\nMultiplication:\n");
        display(multiply(a, b));
        printf("Addition:\n");
        display(add(a, b));
        printf("Subtract:\n");
        display(subtract(a, b));
    }
    return 0;
}