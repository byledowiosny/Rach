#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stddef.h>
#include "C.h"

int mouv_stock(void)
{
  FILE *ifp, *ofp;
  time_t calendar;
  int op_id=0, r_id=0, co_id=0, a_id=0, kg;
  char r_lib[100], co_lib[100], a_lib[100];
  char r_file[100], co_file[100], a_file[100]; 
  char print_date[100], date_op[100];
  float prix_revient_prod=0, prix_marche_gros=0, prix_revient_con=0;
  float prix_ruche_vide=0, prix=0;
  float montant=0, taux_tva=0, taux=0;
  float r_db=0, r_cr=0, co_db=0, co_cr=0, a_db=0, a_cr=0, tva_db=0, tva_cr=0;
  int d=0, p, d_c=0, o_n=0;
  compte plan[100];
  int i = 0, j;
  
  ifp = fopen ("data/id_file", "r");
  fscanf (ifp, "%d", &op_id);
  fclose(ifp);

  ifp = fopen ("data/prix_file", "r");
  while ((fscanf (ifp, "%f", &prix_revient_prod) != EOF))
    {
      fscanf (ifp, "%f", &prix_marche_gros);
      fscanf (ifp, "%f", &prix_revient_con);
      fscanf (ifp, "%f", &prix_ruche_vide);
    }
  fclose(ifp);

  ifp = fopen ("data/plan_compta", "r");
  while ((fscanf (ifp, "%d", &plan[i].id) != EOF))
    {
	
	  fscanf (ifp, "%s", plan[i].lib);
	  fscanf (ifp, "%d", &plan[i].auto_t);
	  fscanf (ifp, "%s", plan[i].file);
	  fscanf (ifp, "%d", &plan[i].auto_c);
	  i++;
    }
  fclose(ifp);

  do
    {
      do
	{
	  d = 0; 
	  printf ("\nChoisissez un compte de stocks.\n\n"
		  " 0 - Aucun, je veux sortir du programme !\n");

	  for (j=0; j<i; j++)
	    {
	      if ((plan[j].id >= 3000) && (plan[j].id < 4000))
		printf (" %4d - %-24s\n", plan[j].id, plan[j].lib);
	    }
  
	  printf ("\nStocks Nr: ");
	  scanf ("%d", &r_id);
	  if (r_id == 0)
	    exit (1);
	  else
	    {
	      for (j=0; j<i; j++)
		{
		  if (plan[j].id == r_id)
		    {
		      sprintf(r_lib, "%s", plan[j].lib);
		      sprintf(r_file, "%s", plan[j].file);
		      d = -1;   
		    }
		}
	    }
	}
      while (d == 0);

      do
	{
	  calendar = calendar_date();
	  strftime(print_date, 100, "%d %b %Y", localtime(&calendar));
	  strftime(date_op, 100, "%Y%m%d", localtime(&calendar));

	  if ((r_id >= 3100) && (r_id < 3150))
	    prix = prix_marche_gros;
	  else if ((r_id >= 3150) && (r_id < 3200))
	    prix = prix_revient_con;
	  else if ((r_id >= 3500) && (r_id < 3550))
	    prix = prix_revient_prod;
	  else if ((r_id >= 3550) && (r_id < 3600))
	    prix = prix_revient_con;
	  else if ((r_id >= 3300) && (r_id < 3400))
	    prix = prix_ruche_vide;
	  else
	    {
	      printf ("\nPrix H.T. d'un kg: ");
	      scanf ("%f", &prix);
	    }
      
	  printf ("Quantite Kg: ");
	  scanf ("%d", &kg);
	  if (kg == 0)
	    exit (1);
	  else
	    {
	      do
		{
		  d = 0;
		  printf ("Debiter (1) ou crediter (2) %s ?: ", r_lib);
		  scanf ("%d", &d_c);
		  if (d_c == 0)
		    exit (1);
		  else
		    {
		      if (d_c == 1)
			{
			  montant = r_db = kg * prix;
			  r_cr = 0;
			}
		      else if (d_c == 2)
			{ 
			  montant = r_cr = kg * prix;
			  r_db = 0;
			}
		      else
			d = -1;
		    }
		}
	      while (d != 0);
	    } 

	  do
	    {
	      d = 0;
	      printf ("Compte correspondant (Nr): ");
	      scanf ("%d", &co_id);
	      if (co_id == 0)
		exit (1);
	      else
		{
		  for (j=0; j<i; j++)
		    {
		      if (plan[j].id == co_id)
			{
			  sprintf(co_lib, "%s", plan[j].lib);
			  sprintf(co_file, "%s", plan[j].file);
			  taux = plan[j].auto_t;
			  a_id = plan[j].auto_c;
			  d = -1;   
			}
		    }
		}
	    }
	  while (d == 0);

	  if (taux != 0)
	    {
	      if (taux == 4)
		{
		  printf ("Taux tva (pour cent): ");
		  scanf ("%f", &taux_tva);
		}
	      else
		taux_tva = taux/10;
	    }

	  if (d_c == 1)
	    {
	      co_cr = montant / (1 + taux_tva / 100);
	      co_db = 0;
	      tva_cr = montant - co_cr;
	      tva_db = 0;
	    }
	  else
	    {
	      co_db = montant / (1 + taux_tva / 100);
	      co_cr = 0;
	      tva_db = montant - co_db;
	      tva_cr = 0;
	    }
	 
	  if (a_id != 0)
	    {
	      do
		{
		  d = 0;
		  if (a_id == 4)
		    {
		      printf ("Compte analytique (Nr): ");
		      scanf ("%d", &a_id);
		    }
		  if (a_id == 0)
		    d = -1;
		  else
		    {
		      for (j=0; j<i; j++)
			{
			  if (plan[j].id == a_id)
			    {
			      sprintf(a_lib, "%s", plan[j].lib);
			      sprintf(a_file, "%s", plan[j].file);
			      d = -1;   
			    }
			}
		    }
		}
	      while (d == 0);

	      
	      if (d_c == 1)
		{
		  a_cr = montant / (1 + taux_tva / 100);
		  a_db = 0;
		}
	      else
		{
		  a_db = montant / (1 + taux_tva / 100);
		  a_cr = 0;
		}
	    }
	  else
	    {
	      a_db = 0;
	      a_cr = 0;
	      sprintf(a_lib, "%s", "No_concerné");
	    }
	
	  for (p=0; p<74; p++)
	    printf ("-");
	  printf ("\n%-12d%-13s%-25s%12s%12s\n",
		  op_id, "Nr_cpt", "Lib_cpt", "Db", "Cr");
	  for (p=0; p<74; p++)
	    printf ("-");
	  printf ("\n%-12s%-13d%-25s%12.2f%12.2f\n",
		  "Registre", r_id, r_lib, r_db, r_cr);
	  printf ("\n%-12s%-13d%-25s%12.2f%12.2f\n",
		  "Co_compte", co_id, co_lib, co_db, co_cr);
	  printf ("\n%-12s%-13d%-25s%12.2f%12.2f\n",
		  "Analytique", a_id, a_lib, a_db, a_cr);
	  for (p=0; p<74; p++)
	    printf ("-");
	  printf ("\n%-12s%-13s%-25s%12d%12.2f\n",
		  "Date", print_date, "Kg/Prix", kg, prix);
	  for (p=0; p<74; p++)
	    printf ("-");
	
	  printf ("\nEnregistrer l'operation Nr %d ? Oui - 1; Non - 2: ",
		  op_id);
	  scanf ("%d", &o_n);
  
	  if (o_n == 1)
	    {
	      ofp = fopen ("data/stock", "a");
	      fprintf (ofp, "%4d %12s %4d %10.2f %10.2f %4d %4d %5.2f\n",
		       op_id, date_op, r_id, r_db, r_cr, co_id, a_id,
		       prix);
	      fclose (ofp);

	      ofp = fopen (r_file, "a");
	      fprintf (ofp, "%4d %12s %10.2f %10.2f %5.2f\n",
		       op_id, date_op, r_db, r_cr, prix);
	      fclose (ofp);
	      
	      ofp = fopen (co_file, "a");
	      fprintf (ofp, "%4d %12s %10.2f %10.2f %1d\n",
		       op_id, date_op, co_db, co_cr, 0);
	      fclose (ofp);

	      if (taux_tva != 0)
		{
		  ofp = fopen ("data/4450", "a");
		  fprintf (ofp, "%4d %12s %10.2f %10.2f %5.2f\n",
			   op_id, date_op, tva_db, tva_cr, taux_tva);
		  fclose (ofp);
		}

	      if (a_id != 0)
		{
		  ofp = fopen (a_file, "a");
		  fprintf (ofp, "%4d %12s %10.2f %10.2f %1d\n",
			   op_id, date_op, a_db, a_cr, 0);
		  fclose (ofp);
		}
	      
	      op_id++;
	      ofp = fopen ("data/id_file", "w");
	      fprintf (ofp, "%d", op_id);
	      fclose (ofp);  
	    }
	  r_db = 0;
	  r_cr = 0;
	  co_db = 0;
	  co_cr = 0;
	  tva_db = 0;
	  tva_cr = 0;
	  taux_tva = 0;
	  a_db = 0;
	  a_cr = 0;
	  printf ("Un autre enregistrement dans %s ? Oui - 1; Non - 2: ",
		  r_lib);
	  scanf ("%d", &o_n);
	}
      while (o_n == 1);
      printf ("Un autre stocks ? Oui - 1; Non - 2: ");
      scanf ("%d", &o_n);
    }
  while (o_n == 1);

  return 0;
}
