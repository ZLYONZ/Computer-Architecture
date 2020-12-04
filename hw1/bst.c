#include <stdio.h>
#include <stdlib.h>

struct node
{
    int val;
    struct node *left;
    struct node *right;
};

int search(struct node *curr, int val)
{
    struct node *head = curr;
    if (head == NULL)
        return 0;

    while (head != NULL)
    {
        if (head->val == val)
            return 1;
        if (head->val > val)
            head = head->left;
        else
            head = head->right;
    }
    return 0;
}

void traverse(struct node *head)
{

    if (head == NULL)
    {
    }

    if (head->left)
        traverse(head->left);

    printf("%d\t", head->val);

    if (head->right)
        traverse(head->right);
}

int count;
struct node *create_node(int i)
{
    struct node *ptr;
    ptr = malloc(sizeof(struct node));
    ptr->val = i;
    ptr->left = NULL;
    ptr->right = NULL;
    count++;
    return ptr;
}

struct node *insert(struct node *curr, int val)
{
    struct node *head = curr;
    if (head == NULL)
        return create_node(val);
    else if (head->val < val)
        head->right = insert(head->right, val);
    else
        head->left = insert(head->left, val);

    return head;
}

void cleanup(struct node *head)
{
    if (head->left != NULL)
        cleanup(head->left);
    if (head->right != NULL)
        cleanup(head->right);

    free(head);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Invalid input");
        return 0;
    }

    FILE *f1; 
    f1 = fopen(argv[1], "r");
    if (f1 == NULL)
    {
        printf("File unfound");
        return 0;
    }

    struct node *head = NULL;
    char op;
    int val, res;

    while (fscanf(f1, "%c\t %d\n", &op, &val) == 2)
    {
        if (op == 'i')
            res = search(head, val);
        if (res == 0)
            head = insert(head, val);
    }

    traverse(head);
    cleanup(head);
    fclose(f1);
    return 0;
}