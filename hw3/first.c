#include <stdio.h>
#include <stdlib.h>

int cmp(char *str1, char *str2)
{
    while (*str1 && *str1 == *str2)
    {
        str1++;
        str2++;
    }
    return *str1 - *str2;
}
int main(int argc, char **argv)
{
    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL)
    {
        printf("File unfound");
        return 0;
    }
    
    int x1, n, s;
    int x2 = 0;

    char op[10];
    char set[10] = "set";
    char comp[10] = "comp";
    char get[10] = "get";

    fscanf(fp, "%d", &x1);
    while (!feof(fp))
    {
        int val = 0;
        fscanf(fp, "%s\t%d\t%d\n", op, &n, &s);
        
        if (cmp(op, set) == 0)
        {
            int temp;
            temp = (x1 >> n) & 1;
            if (temp == s)
            {
                printf("%d\n", x2);
                continue;
            }

            if (s == 0)
                x2 = x1 ^ (1 << n);
            if (s == 1)
                x2 = (1 << n) | x1;
            printf("%d\n", x2);
            x1 = x2;
        }
        else if (cmp(op, get) == 0)
        {
            val = (x1 >> n) & 1;
            printf("%d\n", val);
        }
        else if (cmp(op, comp) == 0)
        {
            int temp;
            temp = (x1 >> n) & 1;
            if (temp == 1)
                s = 0;
            else
                s = 1;
            if (s == 0)
                x2 = x1 ^ (1 << n);
            if (s == 1)
                x2 = (1 << n) | x1;
            printf("%d\n", x2);
            x1 = x2;
        }
    }
    fclose(fp);
    return 0;
}
