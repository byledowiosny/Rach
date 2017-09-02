#include <stdio.h>
#include "C.h"

int main(int argc, char **argv)
{
  int r = 0;
  do
    {
      printf ("\n0 - Sortir du programme;\n"
	      "1 - Opérations courantes;\n"
	      "2 - Opérations d'ouverture d'un nouvel exercice: ");
      scanf ("%d", &r);

      if      (r == 1) menu();
      else if (r == 2) menu_new();
      else             exit(1);
    }
  while(r != 0);

  return 0;
}


  



    
