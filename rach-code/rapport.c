#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stddef.h>
#include "C.h"

int edit_analytique(void)
{
  float d_kg=0, c_kg=0;  
  int annee_comptable=0, dnport, i, j, p;
  float production=0, vente=0, prod_con=0;
  float d_fin=0, c_fin=0, pr=0, pr_con=0, ca_kg=0, prix=0;
  float val_prod_con_pr=0, val_prod_con_pm=0;
  float hypo_gros=0, hypo_con=0;
  float api=0, com=0, bat=0;
  float apiculture=0, commercial=0, ensemble=0;
  FILE *ifp, *ofp;
  char date[20], snport;
  rap_fin anal[10];
  rap_phys stock[20];

  char print_debut[100], print_fin[100];
  char file_debut[100], file_fin[100];
  char year[100];

  const time_t debut = first_date();
  const time_t fin = last_date();

  strftime(print_debut, 100, "%d %b %Y", localtime(&debut));
  strftime(print_fin, 100, "%d %b %Y", localtime(&fin));
  strftime(file_debut, 100, "%Y%m%d", localtime(&debut));
  strftime(file_fin, 100, "%Y%m%d", localtime(&fin));
  strftime(year, 100, "%Y", localtime(&fin));
  annee_comptable = atoi(year);

/* fait une structure de i comptes analytique avec */
/* compte_id, lib, file a partir de fichier plan_compta */
  i = 0;
  ifp = fopen ("data/plan_compta", "r");
  while ((fscanf (ifp, "%d", &anal[i].id) != EOF))
    {
      if (anal[i].id >= 9200)
	{
	  fscanf (ifp, "%s", anal[i].lib);
	  fscanf (ifp, "%d", &dnport);
	  fscanf (ifp, "%s", anal[i].file);
	  fscanf (ifp, "%d", &dnport);
	  i++;
	}
      else
	{
	  fscanf (ifp, "%s", &snport);
	  fscanf (ifp, "%d", &dnport);
	  fscanf (ifp, "%s", &snport);
	  fscanf (ifp, "%d", &dnport);
	}
    }
  fclose(ifp);

/* ajout a chaque compte de cette structure db et cr */
/* en monaie a partir de fichier_comptes 9210, 9220, 9230 */
/* ainsi chaque compte de cette structure pocede 5 champs */
/* suivants: compte_id, compte_lib, nom_ficher_comptable, */
/* db et cr en manaie */
  for (j=0; j<i; j++)
    {
      ifp = fopen (anal[j].file, "r");
      while ((fscanf (ifp, "%d", &dnport) != EOF))
	{
	  fscanf (ifp, "%d", &dnport);
	  fscanf (ifp, "%f", &d_fin);
	  fscanf (ifp, "%f", &c_fin);
	  fscanf (ifp, "%d", &dnport);
	  anal[j].db_fin += d_fin;
	  anal[j].cr_fin += c_fin;
	}
      fclose(ifp);
    }	

/* fait une structure de i compte_stocks avec compte_id, */
/* lib, file a partir de fichier plan_compta */
  i = 0;
  ifp = fopen ("data/plan_compta", "r");
  while ((fscanf (ifp, "%d", &stock[i].id) != EOF))
    {
      if ((stock[i].id >= 3000) && (stock[i].id < 4000))
	{
	  fscanf (ifp, "%s", stock[i].lib);
	  fscanf (ifp, "%d", &dnport);
	  fscanf (ifp, "%s", stock[i].file);
	  fscanf (ifp, "%d", &dnport);
	  i++;
	}
      else
	{
	  fscanf (ifp, "%s", &snport);
	  fscanf (ifp, "%d", &dnport);
	  fscanf (ifp, "%s", &snport);
	  fscanf (ifp, "%d", &dnport);
	}
    }
  fclose(ifp);

/* ajout a chaque compte de cette structure stock_debut */ 
/* toujour enregistre avec la date du 1.01, db, cr et */
/* stock_fin en kg a partir de fichier_comptes a_fut, */
/* a_pot, c_fut, .../prix ainsi chaque compte de cette structure */
/* pocede 7 champs suivants: compte_id, compte_lib, */
/* nom_ficher_comptable, stock_debut, db, cr et stock_fin en kg */
  for (j=0; j<i; j++)
    {
      d_kg = 0;
      c_kg = 0;
      stock[j].cr_kg = 0;
      stock[j].debut_kg = 0;
      stock[j].db_kg = 0;
      ifp = fopen (stock[j].file, "r");
      while ((fscanf (ifp, "%d", &dnport) != EOF))
	{
	  fscanf (ifp, "%s", date);
	  fscanf (ifp, "%f", &d_kg);
	  fscanf (ifp, "%f", &c_kg);
	  fscanf (ifp, "%f", &prix);
	  stock[j].cr_kg += c_kg/prix;
	  if (atoi(date) == (annee_comptable*10000 + 101))
	    {
	      stock[j].debut_kg += d_kg/prix;
	    }
	  else
	    stock[j].db_kg += d_kg/prix;
	}
      fclose(ifp);
      stock[j].fin_kg = stock[j].debut_kg + stock[j].db_kg - stock[j].cr_kg;
    }
  
  for (j=0; j<i; j++)
    {
/* miel achete conditionné: production, vente */
      if ((stock[j].id >=3150) && (stock[j].id < 3200))
	{
	  prod_con += stock[j].db_kg;
	  vente += stock[j].cr_kg;
	}
/* miel en vrac: production */
      else if ((stock[j].id >= 3500) && (stock[j].id < 3550))
	production += stock[j].db_kg;
/* miel propre conditionné: production, vente */
      else if ((stock[j].id >=3550) && (stock[j].id < 3600))
	{
	  prod_con += stock[j].db_kg;
	  vente += stock[j].cr_kg;
	}
    }

/* soldes */
  api = anal[1].cr_fin - anal[1].db_fin ;
  com = anal[2].cr_fin - anal[2].db_fin ;
  bat = anal[0].db_fin - anal[0].cr_fin ;
/* prix de revient production */
  pr = (anal[1].db_fin + bat/2) / production; 
/* valeur de production conditionnee au prix de revient et */
/* au prix de marche */
  val_prod_con_pr = prod_con * pr; 
  val_prod_con_pm = prod_con * 20; 
/* resultat d'apiculture, du commerce, et d'ensemble */
  apiculture = api - bat/2 + val_prod_con_pr;
  commercial = com - bat/2 - val_prod_con_pr;
  ensemble   = apiculture + commercial;
/* hypothèses: que l'apiculture, que le commerce */
  hypo_gros  = api - bat + val_prod_con_pm;
  hypo_con   = com - bat - val_prod_con_pm;
/* prix de revient de production conditionnée */
  pr_con = (anal[2].db_fin + bat/2) / vente;
/* 1 kg de conditionne vendu apporte en ca commerciale */
  ca_kg  = anal[2].cr_fin / vente;
  
  ofp = fopen ("data/rapport", "w");
  fprintf (ofp, "\n            Global Tomaka - Compte de résultat analytique"
	   "\n                    du %s au %s\n", print_debut, print_fin);
  
  for (p=0; p<61; p++)
    fprintf (ofp, "-");
  fprintf (ofp, "\n%-37s  %5s %4d  %5s %4d\n",
	  "", "Année", (annee_comptable - 1), "Année", annee_comptable);

  for (p=0; p<61; p++)
    fprintf (ofp, "-");

  fprintf (ofp, "\n%-37s", "SOLDES:\n");
  fprintf (ofp, "\n%-37s  %10.0f  %10.0f\n",
	   "  Bâtiments et bureau (db)", 0.0, bat);
  fprintf (ofp, "%-37s  %10.0f  %10.0f\n",
	   "  Apiculture et camion (cr)", 0.0, api);
  fprintf (ofp, "%-37s  %10.0f  %10.0f\n",
	   "  Commercialisation et voiture (cr)", 0.0, com);

  for (p=0; p<61; p++)
    fprintf (ofp, "-");
  fprintf (ofp, "\n");
  for (p=0; p<61; p++)
    fprintf (ofp, "-");

  fprintf (ofp, "\n%-37s\n\n", "RESULTATS:");
  fprintf (ofp, "%-37s  %10.0f  %10.0f\n",
	   " Résultat d'Apiculture", 0.0, apiculture);
  fprintf (ofp, "%-37s  %10.0f  %10.0f\n\n",
	   "  *Prix de revient au kg (production)", 0.0, pr);
  fprintf (ofp, "%-37s  %10.0f  %10.0f\n",
	   " Résultat de Commercialisation", 0.0, commercial);
  fprintf (ofp, "%-37s  %10.0f  %10.0f\n",
	   "  *Prix de revient au kg (commecial)", 0.0, pr_con);
  fprintf (ofp, "%-37s  %10.0f  %10.0f\n",
	   "  *1 kg vendu apporte en ca commecial", 0.0, ca_kg);
  fprintf (ofp, "%-37s  %10.0f  %10.0f\n\n",
	   "  *Pour 1000 de ca_com, il faut FR de", 0.0, (pr_con/ca_kg)*1000);
  fprintf (ofp, "\%-37s  %10.0f  %10.0f\n",
	   " Résultat de l'ensemble", 0.0, ensemble);

  for (p=0; p<61; p++)
    fprintf (ofp, "-");
  fprintf (ofp, "\n");
  for (p=0; p<61; p++)
    fprintf (ofp, "-");

  fprintf (ofp, "\n%-37s", "HYPOTHESES:\n");
  fprintf (ofp, "\n%-37s  %10.0f  %10.0f\n",
	   "  Vente en gros seule", 0.0, hypo_gros);
  fprintf (ofp, "%-37s  %10.0f  %10.0f\n",
	   "  Commercialisation seule", 0.0, hypo_con);

  for (p=0; p<61; p++)
    fprintf (ofp, "-");
  
  
  fprintf (ofp, "\n");

  fclose (ofp);

  system("vim data/rapport");

  return 0;
}

int print_analytique(void)
{
  system("lpr data/rapport");

  return 0;
}
