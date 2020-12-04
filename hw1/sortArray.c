#include <stdio.h>

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    printf("Invalid Input!\n");
    return 0;
  }

  FILE *f1;
  // Open file, perform read operation
  if ((f1 = fopen(argv[1], "r")) == NULL)
  {
    printf("File unfound!");
    return 0;
  }

  int len, i, j, temp;

  // Read file
  fscanf(f1, "%d", &len);

  int arr[len];
  for (i = 0; i < len; i++)
  {
    fscanf(f1, "%d", &arr[i]);
  }

  // Let arr contains even and then odd
  int index;
  for (i = 0; i < len; i++)
  {
    if (arr[i] % 2 == 0)
    {
      index = i;
      continue;
    }
    for (j = i + 1; j < len; j++)
    {
      if (arr[j] % 2 == 0)
      {
        temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
        index = i;
        break;
      }
    }
  }

  // Sort array
  for (i = 0; i < len; i++)
  {
    if (i <= index)
    {
      // Ascending order
      for (j = i + 1; j <= index; j++)
      {
        if (arr[i] > arr[j])
        {
          temp = arr[i];
          arr[i] = arr[j];
          arr[j] = temp;
        }
      }
    }
    else
    {
      // Descending order
      for (j = i + 1; j < len; j++)
      {
        if (arr[i] < arr[j])
        {
          temp = arr[i];
          arr[i] = arr[j];
          arr[j] = temp;
        }
      }
    }
  }

  // Output
  for (i = 0; i < len; i++)
    printf("%d\t", arr[i]);

  fclose(f1);
  return 0;
}