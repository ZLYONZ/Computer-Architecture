#include <stdio.h>
#include <stdlib.h>

struct node
{
  int value;
  struct node *next;
};

struct node *insert(struct node *head, int val)
{
  struct node *prev = NULL;
  struct node *curr = head;
  struct node *temp = (struct node *)malloc(sizeof(struct node));

  temp->value = val;
  temp->next = NULL;

  if (head == NULL)
    head = temp;
  else
  {
    curr = head;
    prev = head;

    while (curr != NULL && curr->value < val)
    {
      prev = curr;
      curr = prev->next;
    }

    if (curr == NULL)
    {
      prev->next = temp;
    }
    else
    {
      if (curr != head)
      {
        temp->next = curr;
        prev->next = temp;
      }
      else
      {
        temp->next = curr;
        head = temp;
      }
    }
  }
  return head;
}

struct node *delete (struct node *head, int val)
{
  struct node *prev = NULL;
  struct node *curr = head;
  while (curr != NULL)
  {
    if (val < curr->value)
      return head;
    else if (val == curr->value)
      break;
    prev = curr;
    curr = curr->next;
  }
  if (curr == NULL)
    return head;
  if (prev == NULL)
    head = curr->next;
  else
    prev->next = curr->next;
  free(curr);
  return head;
}

void traverse(struct node *head)
{
  struct node *curr = head;
  if (head != NULL)
  {
    printf("%d", curr->value);
    curr = curr->next;
    while (curr != NULL)
    {
      printf("\t%d", curr->value);
      curr = curr->next;
    }
  }
  printf("\n");
}

int size(struct node *head)
{
  struct node *curr = head;
  int count = 0;
  while (curr != NULL)
  {
    count++;
    curr = curr->next;
  }
  return count;
}

void freeList(struct node *head){
  struct node *temp;
  while (head != NULL)
  {
    temp = head;
    head = head->next;
    free(temp);
  }
  
}

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    printf("Invalid Input");
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
  int val;

  while (fscanf(f1, "%c\t %d\n", &op, &val) == 2)
  {
    if (op == 'i')
    {
      head = insert(head, val);
    }
    else if (op == 'd')
      head = delete (head, val);
  }

  printf("%d\n", size(head));
  traverse(head);
  freeList(head);
  fclose(f1);
}