#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stddef.h>
#include "C.h"

int menu_new(void)
{
  int r = 0;
  do
    {
      printf ("\n0 - Sortir du programme;\n"
	      "1 - Importer le plan comptable;\n"
	      "2 - Editer le plan comptable;\n"
	      "3 - Cr�er le syst�me de comptes;\n"
	      "4 - Cr�er les autres fichiers comptables;\n"
	      "5 - Saisir les soldes de debut, � l'exclusion de stocks;\n"
	      "6 - Importer le fichier de prix de revient;\n"
	      "7 - Editer le fichier de prix de revient;\n"
	      "8 - Saisir les soldes de stocks;\n"
	      "9 - Op�rations courantes: ");

      scanf ("%d", &r);

      if (r == 1)
	import_plan();
      else if (r == 2)
	edit_plan();
      else if (r == 3)
	touch_compta();
      else if (r == 4)
	do_files();
      else if (r == 5)
	open();
      else if (r == 6)
	import_prix();
      else if (r == 7)
	edit_prix();
      else if (r == 8)
	mouv_stock();
      else if (r == 9)
	menu();
      else
	exit(1);
    }
  while(r != 0);

  return 0;
}

int do_files(void)
{
  int annee;
  FILE *ofp;

  system("touch data/annee data/id_file data/ctp_report data/report_tva data/livre data/stock data/bilan data/result data/declar_tva");

  printf ("\nAnn�e comptable (aaaa): ");
  scanf ("%d", &annee);
  ofp = fopen ("data/annee", "w");
  fprintf (ofp, "%d", annee);
  fclose (ofp);

  ofp = fopen ("data/id_file", "w");
  fprintf (ofp, "%d", 1);
  fclose (ofp);

  ofp = fopen ("data/ctp_report", "w");
  fprintf (ofp, "%d", 1100);
  fclose (ofp);

  ofp = fopen ("data/report_tva", "w");
  fprintf (ofp, "%d", 0);
  fclose (ofp);

  return 0;
}
