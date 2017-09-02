#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stddef.h>
#include "C.h"

int import_prix(void)
{
  char file[100], command[100];

  printf ("\nFichier des prix de revient (chemin absolut): ");
  scanf ("%s", file);
  
  sprintf(command, "cp %s ./data", file);
  system(command);

  return 0;
}

int edit_prix(void)
{
  system("vim data/prix_file");

  return 0;
}
