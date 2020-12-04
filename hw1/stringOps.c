#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
  int i, j;
  char ch;

  for (i = 1; i < argc; ++i)
  {
    j = 0; 

    while ((ch = argv[i][j]))
    {
      if (ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u' ||
          ch == 'A' || ch == 'E' || ch == 'I' || ch == 'O' || ch == 'U')
      {
        printf("%c", ch);
      }
      j++;
    }
  }

  printf("\n");
  return 0;
}