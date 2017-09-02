#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stddef.h>
#include "C.h"

/* do zrobienia: zarejestrowac na report_tva nie tylko report ale */
/* rowniez sume actualisowana ca_r, ca_s, a_payer_s, a_payer_r,   */
/* a_deduire poszczegolnymi kwartalami; zapytac sie czy to jest   */
/* kwartalna czy roczana, i roczna wyliczyc z calosci operacji    */
/* minus zarejestrowane sumy.                                     */

int edit_tva(void)
{
  FILE *ifp, *ofp;
  int op, o_n, r = 0;
  int date_op;
  float tva_db, tva_cr, taux_tva;
  float a_payer_s = 0, a_payer_r = 0, ca_s = 0, ca_r = 0;
  float a_deduire = 0;
  int report = 0, solde = 0;
  
  char print_debut[100], print_fin[100];
  char file_debut[100], file_fin[100];

  const time_t debut = first_date();
  const time_t fin = last_date();

  strftime(print_debut, 100, "%d %b %Y", localtime(&debut));
  strftime(print_fin, 100, "%d %b %Y", localtime(&fin));
  strftime(file_debut, 100, "%Y%m%d", localtime(&debut));
  strftime(file_fin, 100, "%Y%m%d", localtime(&fin));

  ifp = fopen ("data/report_tva", "r");
  fscanf (ifp, "%d", &report);
  fclose(ifp);

  ifp = fopen ("data/4450", "r");
  
  while ((fscanf (ifp, "%d", &op) != EOF)) {
    fscanf (ifp, "%d", &date_op);
    fscanf (ifp, "%f", &tva_db);
    fscanf (ifp, "%f", &tva_cr);
    fscanf (ifp, "%f", &taux_tva);
    if ((date_op >= atoi(file_debut)) &&
	(date_op <= atoi(file_fin))   &&
	(taux_tva != 0))
      {
	if (tva_db != 0)
	  a_deduire += tva_db;
	else
	  {
	    if (taux_tva >= 18.60)
	      {
		a_payer_s += tva_cr;
		ca_s += tva_cr/(taux_tva/100);
	      }
	    else
	      {
		a_payer_r += tva_cr;
		ca_r += tva_cr/(taux_tva/100);
	      }
	  }
      }
  }
  fclose (ifp);

  

  ofp = fopen ("data/declar_tva", "w");

  fprintf (ofp, "\nVoici les elements de votre declaration tva\n");
  fprintf (ofp, "       du %s au %s\n\n", print_debut, print_fin);
  fprintf (ofp, "%-2s  %-23s  %10d\n",
	        "01", "Ventes ...", (int)ca_s + (int)ca_r);
  fprintf (ofp, "%-2s  %-23s  %10d  %10d\n",
	        "07", "Taux 19.6 % ...", (int)ca_s, (int)a_payer_s);
  fprintf (ofp, "%-2s  %-23s  %10d  %10d\n",
	        "08", "Taux  5.5 % ...", (int)ca_r, (int)a_payer_r);
  fprintf (ofp, "%-2s  %-23s  %22d\n",
	        "14", "Total ...", (int)a_payer_s + (int)a_payer_r);
  fprintf (ofp, "%-2s  %-23s  %22d\n",
	        "17", "Autre biens ...", (int)a_deduire+1);
  fprintf (ofp, "%-2s  %-23s  %10d\n",
	        "19", "Report ...", report);
  fprintf (ofp, "%-2s  %-23s  %22d\n",
	        "21", "Total ...", (int)a_deduire+1 + report);

  if ((solde = ((int)a_deduire+1 + report) - 
               ((int)a_payer_s + (int)a_payer_r)) >= 0)
    {
      fprintf (ofp, "%-2s  %-23s  %10d\n",
	            "22", "Credit ...", solde);
      printf ("\nDemander remboursement - 1; Reporter -2: ");
      scanf ("%d", &o_n);
      if (o_n == 1)
	{
	  fprintf (ofp, "%-2s  %-23s  %10d\n",
	                "23", "Remboursement ...", solde);
	  r = 0;
	}
      else
	{
	  fprintf (ofp, "%-2s  %-23s  %10d\n",
	                "24", "Credit a reporter ...", solde);
	  r = 1;
	}
    }  
  else
    {
      fprintf (ofp, "%-2s  %-23s  %22d\n",
	            "25", "TVA nette due ...", -solde);
      r = 0;
    }
  fclose (ofp);

  if (r == 0)
    {
      ofp = fopen ("data/report_tva", "w");
      fprintf (ofp, "%d", 0);
      fclose (ofp);
    }
  else
    {
      ofp = fopen ("data/report_tva", "w");
      fprintf (ofp, "%d", solde);
      fclose (ofp);
    }

  system("vim data/declar_tva");

  return 0;
}

int print_tva(void)
{
  system("lpr data/declar_tva");

  return 0;
}
