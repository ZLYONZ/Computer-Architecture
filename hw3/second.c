#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool get(unsigned int n)
{
    bool parity = 0;
    while (n)
    {
        parity = !parity;
        n = n & (n - 1);
    }
    return parity;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Invaild input");
        return 0;
    }

    int n;
    n = atoi(argv[1]);
    int count = 0;

    int temp = n;
    while (n > 0)
    {
        if (((n >> 0) & 1) == 1)
        {
            n = n >> 1;
            if (((n >> 0) & 1) == 1)
            {
                n = n >> 1;
                count++;
            }
        }
        else
            n = n >> 1;
    }
    if (get(temp))
        printf("Odd-Parity\t");
    else
        printf("Even-Parity\t");

    printf("%u\n", count);
    return 0;
}