#include <stdio.h>
#include "C.h"

int menu(void)
{
  int r = 0;
  do
    {
      printf ("\n 0 - Sortir du programme;\n"
	      " 1 - Saisir des operations, à l'excusion de stocks;\n"
	      " 2 - Editer les prix de revient;\n"
	      " 3 - Saisir des mouvements de stocks;\n"
	      " 4 - Editer une declaration de TVA;\n"
	      " 5 - Imprimer la declaration de TVA;\n"
	      " 6 - Editer un compte de resultat;\n"
	      " 7 - Imprimer le compte de resultat;\n"
	      " 8 - Editer un bilan;\n"
	      " 9 - Imprimer le bilan;\n"
	      "10 - Editer un rapport analytique;\n"
	      "11 - Imprimer le rapport analytique;\n"
	      "12 - Editer le plan comptable;\n"
	      "13 - Imprimer le plan comptable;\n"
	      "14 - Actualiser le système de comptes;\n"
	      "15 - Opérations d'ouverture d'un nouvel exercice: ");

      scanf ("%d", &r);

      if      (r ==  1) saisir();
      else if (r ==  2) edit_prix();
      else if (r ==  3) mouv_stock();
      else if (r ==  4) edit_tva();
      else if (r ==  5) print_tva();
      else if (r ==  6) edit_result();
      else if (r ==  7) print_result();
      else if (r ==  8) edit_bilan();
      else if (r ==  9) print_bilan();
      else if (r == 10) edit_analytique();
      else if (r == 11) print_analytique();
      else if (r == 12) edit_plan();
      else if (r == 13) print_plan();
      else if (r == 14) touch_compta();
      else if (r == 15) menu_new();
      else              exit(1);
    }
  while(r != 0);

  return 0;
}
