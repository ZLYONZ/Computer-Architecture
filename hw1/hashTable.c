#include <stdio.h>
#include <stdlib.h>
#define SIZE 10000

typedef struct linkedList
{
    int value;
    struct linkedList *next;

} List;

List *bucket[SIZE];

List *createNode(int val)
{
    List *node = (List *)calloc(1, sizeof(List));
    node->value = val;
    node->next = NULL;
    return node;
}

int insert(int num)
{
    int hashVal = num % SIZE;
    if (hashVal < 0)
    {
        hashVal += SIZE;
    }

    int collision, duplicateExist = 0;
    if (bucket[hashVal] == NULL)
    {
        bucket[hashVal] = createNode(num);
        collision = 0;
    }

    else{
        collision = 1;
        List *pre = NULL;
        List *curr = bucket[hashVal];
        while (curr != NULL)
        {
            if (curr->value == num)
            {
                duplicateExist = 1;
                break;
            }
            pre = curr;
            curr = curr->next;
        }

        if (duplicateExist == 0)
            pre->next = createNode(num);
    }
    return collision;
}

int search(int num)
{
    int hashVal = num % SIZE;
    if (hashVal < 0)
    {
        hashVal += SIZE;
    }

    List *curr = bucket[hashVal];
    int valueFound = 0;
    while (curr != NULL)
    {
        if (curr->value == num)
            valueFound = 1;
        curr = curr->next;
    }
    return valueFound;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Invalid input!");
        return 0;
    }

    else
    {
        FILE *f1;
        f1 = fopen(argv[1], "r");
        if (f1 == NULL)
        {
            printf("File unfound!");
            return 0;
            exit(1);
        }
        else
        {
            int i, num;
            char ch;
            int count1 = 0, count2 = 0;
            for (i = 0; i < SIZE; i++)
            {
                bucket[i] = NULL;
            }
            int data = fscanf(f1, "%c\t %d\n", &ch, &num);
            while (data == 2)
            {
                if (ch == 'i' || ch == 'I')
                    count1 += insert(num);
                else if (ch == 's' || ch == 'S')
                    count2 += search(num);

                data = fscanf(f1, "%c\t%d\n", &ch, &num);
            }

            printf("%d\n%d\n", count1, count2);
            fclose(f1);
        }
    }
    return 0;
}
