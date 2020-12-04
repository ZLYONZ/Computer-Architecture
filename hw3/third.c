#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Invalid input");
        return 0;
    } 
 
    int x1;
    x1 = atoi(argv[1]);
    
    int i = 0;
    int j = 15;
    for (i = 0; i < j; i++)
    {
        if (((x1 >> j) & 1) != ((x1 >> i) & 1))
        {
            printf("Not-Palindrome\n");
            return 0;
        }
        j--;
    }
    printf("Is-Palindrome\n");
    return 0;
}