#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stddef.h>
#include "C.h"

int touch_compta(void)
{
  char file[100], command[100], snport;
  FILE *ifp;
  int dnport, i;

  i = 0;
  ifp = fopen ("data/plan_compta", "r");
  while ((fscanf (ifp, "%d", &dnport) != EOF))
    {
      fscanf (ifp, "%s", &snport);
      fscanf (ifp, "%d", &dnport);
      fscanf (ifp, "%s", file);
      fscanf (ifp, "%d", &dnport);
      sprintf(command, "touch %s", file);
      system(command);
      i++;
    }
  fclose(ifp);

  printf("\nLes %d comptes de votre système ont été actualisés.\n", i);
  return 0;
}

int edit_plan(void)
{
  system("vim data/plan_compta");

  return 0;
}

int print_plan(void)
{
  system("lpr data/plan_compta");

  return 0;
}

int import_plan(void)
{
  char file[100], command[100];

  printf ("\nFichier du plan comptable (chemin absolut): ");
  scanf ("%s", file);
  
  sprintf(command, "cp %s ./data", file);
  system(command);

  return 0;
}
