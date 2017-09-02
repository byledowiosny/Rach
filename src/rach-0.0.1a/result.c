#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stddef.h>
#include "C.h"

#define DOTATIONS 36617.26

int edit_result(void)
{  
  int annee_comptable=0, i, j, who=0, dnport, date, c=0, p;
  FILE *ifp, *ofp;
  char snport;
  rap_fin result[100];
  float db=0, cr=0;
  float ventes=0, production=0, subventions=0, achats=0;
  float marge=0, services=0, valeur_excedent=0;
  float valeur=0, excedent=0, resultat=0, r_courant=0, r_net=0;
  float ventes_production=0, variations_stocks=0;
  float dotations=0, interets=0, prod_except=0;
  float ch_except=0, marge_fin=0, valeur_fin=0, excedent_fin=0;
  float resultat_fin=0, r_courant_fin=0, r_net_fin=0;
  float r_net_fin_cour=0;

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

/* fait une structure de i comptes de result avec compte_id, _lib, _file */
/* a partir de fichier plan_compta */
  i = 0;
  ifp = fopen ("data/plan_compta", "r");
  while ((fscanf (ifp, "%d", &result[i].id) != EOF))
    {
      if ((result[i].id >= 6000) && (result[i].id < 8000))
	{
	  fscanf (ifp, "%s", result[i].lib);
	  fscanf (ifp, "%d", &dnport);
	  fscanf (ifp, "%s", result[i].file);
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

  /* ajoute a chaque compte de cette structure db et cr en monaie */
  /* a partir de fichier_comptes, sommes de toutes les operations*/
  /* ainsi chaque compte de cette structure pocede 5 champs suivants: */
  /* compte_id, compte_lib, nom_ficher_comptable, db et cr total */
  for (j=0; j<i; j++)
    {
      result[j].db_fin = 0;
      result[j].cr_fin = 0;
      ifp = fopen (result[j].file, "r");
      while ((fscanf (ifp, "%d", &dnport) != EOF))
	{
	  fscanf (ifp, "%d", &date);
	  fscanf (ifp, "%f", &db);
	  fscanf (ifp, "%f", &cr);
	  fscanf (ifp, "%d", &dnport);
	  if ((date >= atoi(file_debut)) && (date <= atoi(file_fin)))
	    {
	      result[j].db_fin += db;
	      result[j].cr_fin += cr;
	    }
	}
      fclose(ifp);
    }

  for (j=0; j<i; j++)
    {
      c = result[j].id;
      db = result[j].db_fin;
      cr = result[j].cr_fin;
      /* ventes */
      if ((c >= 7000) && (c < 7100))
	{
	  ventes -= db;
	  ventes += cr;
	}
      /* production  stockée et immobilisée */
      else if ((c >= 7100) && (c < 7300))
	{
	  ventes_production -= db;
	  ventes_production += cr;
	}
      /* subventions */
      else if ((c >= 7400) && (c < 7500))
	{
	  subventions -= db;
	  subventions += cr;
	}
      /* exceptionnelles, reprises */
      else if ((c >= 7700) && (c < 7900))
	{
	  prod_except -= db;
	  prod_except += cr;
	}
      /* achats */
      else if (((c >= 6000) && (c < 6030)) || ((c >= 6060) && (c < 6100)))
	{
	  achats += db;
	  achats -= cr;
	}
      /* variations de stocks */
      else if ((c >= 6030) && (c < 6040))
	{
	  variations_stocks += db;
	  variations_stocks -= cr;
	}
      /* autres charges externes */
      else if ((c >= 6100) && (c < 6300))
	{
	  services += db;
	  services -= cr;
	}
      /* taxes, cotisations */
      else if ((c >= 6300) && (c < 6500))
	{
	  valeur_excedent += db;
	  valeur_excedent -= cr;
	}
      /* interets */
      else if ((c >= 6600) && (c < 6700))
	{
	  interets += db;
	  interets -= cr;
	}
      /* exceptionnelles */
      else if ((c >= 6700) && (c < 6800))
	{
	  ch_except += db;
	  ch_except -= cr;
	}
      /* dotations */
      else if ((c >= 6800) && (c < 6900))
	{
	  dotations += db;
	  dotations -= cr;
	}
      c = 0;
      db = 0;
      cr = 0;
    }

  production = ventes + ventes_production;
  marge = production - achats + variations_stocks;
  marge_fin = ventes - achats;
  valeur = marge - services;
  valeur_fin = marge_fin - services;
  excedent = valeur - valeur_excedent + subventions;
  excedent_fin = valeur_fin - valeur_excedent + subventions;
  resultat = excedent - dotations;
  resultat_fin = excedent_fin - DOTATIONS;
  r_courant = resultat - interets;
  r_courant_fin = resultat_fin - interets;
  r_net = r_courant + prod_except - ch_except;
  r_net_fin = r_courant_fin + prod_except - ch_except;
  r_net_fin_cour = r_courant_fin;

  printf ("0 - Un compte de résultat abrégé;\n"
	  "1 - Un compte de résultat complet: ");
  scanf ("%d", &who);

  /* ici commence l'édition du compte de résultat complet */

  if (who == 1)
    {
      ofp = fopen ("data/result", "w");

      /* titre */
      fprintf (ofp, "\n            Laurier Tomaka - Compte de résultat"
	       "\n               du %s au %s\n", print_debut, print_fin);
      
      /* LINE */
      for (p=0; p<65; p++)
	fprintf (ofp, "-");
      fprintf (ofp, "\n");

      /* colones */
      fprintf (ofp, "%-41s  %5s %4d  %5s %4d\n",
	       "", "Année", (annee_comptable - 1), "Année", annee_comptable);
      
      /* LINE */
      for (p=0; p<65; p++)
	fprintf (ofp, "-");
      fprintf (ofp, "\n");

      /* ventes */
      for (j=0; j<i; j++)
	{
	  if ((result[j].id >= 7000) && (result[j].id < 7100))
	    {
	      fprintf (ofp, "   +%-37s  %10.2f  %10.2f\n",
		       result[j].lib, 0.00,
		       result[j].cr_fin - result[j].db_fin);
	    }
	}

      /* LINE */
      for (p=0; p<65; p++)
	fprintf (ofp, "-");
      fprintf (ofp, "\n");
    
      /* ventes - ca */
      fprintf (ofp, "%-41s  %10.2f  %10.2f\n",
	       "Ventes (chiffre d'affaire)", 0.00, ventes);

      /* LINE */
      for (p=0; p<65; p++)
	fprintf (ofp, "-");
      fprintf (ofp, "\n");

      /* production  stockée, production immobilisée */
      for (j=0; j<i; j++)
	{
	  if ((result[j].id >= 7100) && (result[j].id < 7300))
	    {
	      fprintf (ofp, "   +%-37s  %10.2f  %10.2f\n",
		       result[j].lib, 0.00,
		       result[j].cr_fin - result[j].db_fin);
	    }
	}

      /* LINE */
      for (p=0; p<65; p++)
	fprintf (ofp, "-");
      fprintf (ofp, "\n");

      /* production de l'exercice */
      fprintf (ofp, "%-41s  %10.2f  %10.2f\n",
	       "Production de l'exercice", 0.00, production);

      /* LINE */
      for (p=0; p<65; p++)
	fprintf (ofp, "-");
      fprintf (ofp, "\n");

      /* achats */
      fprintf (ofp, "   -%-37s  %10.2f  %10.2f\n",
	       "Achats", 0.00, achats);
      for (j=0; j<i; j++)
	{
	  if (((result[j].id >= 6000) && (result[j].id < 6030))
	      || ((result[j].id >= 6060) && (result[j].id < 6100)))
	    {
	      fprintf (ofp, "       %-34s  %10.2f  %10.2f\n",
		       result[j].lib, 0.00,
		       result[j].db_fin - result[j].cr_fin);
	    }
	}
    
      /* variations de stocks */
      for (j=0; j<i; j++)
	{
	  if ((result[j].id >= 6030) && (result[j].id < 6040))
	    {
	      fprintf (ofp, "   -%-37s  %10.2f  %10.2f\n",
		       result[j].lib, 0.00,
		       result[j].db_fin - result[j].cr_fin);
	    }
	}

      /* LINE */
      for (p=0; p<65; p++)
	fprintf (ofp, "-");
      fprintf (ofp, "\n");

      /* marge brute */
      fprintf (ofp, "%-41s  %10.2f  %10.2f\n",
	       "Marge brute", 0.00, marge);

      /* LINE */
      for (p=0; p<65; p++)
	fprintf (ofp, "-");
      fprintf (ofp, "\n");

      /* autres charges externes */
      fprintf (ofp, "   -%-37s  %10.2f  %10.2f\n",
	       "Autres charges externes", 0.00, services);
      for (j=0; j<i; j++)
	{
	  if ((result[j].id >= 6100) && (result[j].id < 6300))
	    {
	      fprintf (ofp, "       %-34s  %10.2f  %10.2f\n",
		       result[j].lib, 0.00,
		       result[j].db_fin - result[j].cr_fin);
	    }
	}

      /* LINE */
      for (p=0; p<65; p++)
	fprintf (ofp, "-");
      fprintf (ofp, "\n");
    
      /* valeur ajoutée */
      fprintf (ofp, "%-41s  %10.2f  %10.2f\n",
	       "Valeur ajoutée", 0.00, valeur);

      /* LINE */
      for (p=0; p<65; p++)
	fprintf (ofp, "-");
      fprintf (ofp, "\n");

      /* taxes, cotisations sociales */
      for (j=0; j<i; j++)
	{
	  if ((result[j].id >= 6300) && (result[j].id < 6500))
	    {
	      fprintf (ofp, "   -%-37s  %10.2f  %10.2f\n",
		       result[j].lib, 0.00,
		       result[j].db_fin - result[j].cr_fin);
	    }
	}

      /* subventions */
      for (j=0; j<i; j++)
	{
	  if ((result[j].id >= 7400) && (result[j].id < 7500))
	    {
	      fprintf (ofp, "   +%-37s  %10.2f  %10.2f\n",
		       result[j].lib, 0.00,
		       result[j].cr_fin - result[j].db_fin);
	    }
	}

      /* LINE */
      for (p=0; p<65; p++)
	fprintf (ofp, "-");
      fprintf (ofp, "\n");

      /* excédent brut */
      fprintf (ofp, "%-41s  %10.2f  %10.2f\n",
	       "Excédent brut d'exploitation", 0.00, excedent);

      /* LINE */
      for (p=0; p<65; p++)
	fprintf (ofp, "-");
      fprintf (ofp, "\n");

      /* dotations d'amortissements */
      for (j=0; j<i; j++)
	{
	  if ((result[j].id >= 6800) && (result[j].id < 6900))
	    {
	      fprintf (ofp, "   -%-37s  %10.2f  %10.2f\n",
		       result[j].lib, 0.00,
		       result[j].db_fin - result[j].cr_fin);
	    }
	}

      /* LINE */
      for (p=0; p<65; p++)
	fprintf (ofp, "-");
      fprintf (ofp, "\n");

      /* résultat d'exploitation */
      fprintf (ofp, "%-41s  %10.2f  %10.2f\n",
	       "Résultat d'exploitation", 0.00, resultat);

      /* LINE */
      for (p=0; p<65; p++)
	fprintf (ofp, "-");
      fprintf (ofp, "\n");

      /* intérêts */
      for (j=0; j<i; j++)
	{
	  if ((result[j].id >= 6600) && (result[j].id < 6700))
	    {
	      fprintf (ofp, "   -%-37s  %10.2f  %10.2f\n",
		       result[j].lib, 0.00,
		       result[j].db_fin - result[j].cr_fin);
	    }
	}

      /* LINE */
      for (p=0; p<65; p++)
	fprintf (ofp, "-");
      fprintf (ofp, "\n");

      /* résultat courant */
      fprintf (ofp, "%-41s  %10.2f  %10.2f\n",
	       "Résultat courant", 0.00, r_courant);

      /* LINE */
      for (p=0; p<65; p++)
	fprintf (ofp, "-");
      fprintf (ofp, "\n");

      /* charges exceptionnelles */
      for (j=0; j<i; j++)
	{
	  if ((result[j].id >= 6700) && (result[j].id < 6800))
	    {
	      fprintf (ofp, "   -%-37s  %10.2f  %10.2f\n",
		       result[j].lib, 0.00,
		       result[j].db_fin - result[j].cr_fin);
	    }
	}

      /* produits exceptionnelles, reprises d'amortissments*/
      for (j=0; j<i; j++)
	{
	  if ((result[j].id >= 7700) && (result[j].id < 7900))
	    {
	      fprintf (ofp, "   +%-37s  %10.2f  %10.2f\n",
		       result[j].lib, 0.00,
		       result[j].cr_fin - result[j].db_fin);
	    }
	}

      /* LINE */
      for (p=0; p<65; p++)
	fprintf (ofp, "-");
      fprintf (ofp, "\n");

      /* résultat net */
      fprintf (ofp, "%-41s  %10.2f  %10.2f\n",
	       "Résultat net de l'exercice", 0.00, r_net);

      /* LINE */
      for (p=0; p<65; p++)
	fprintf (ofp, "-");
      fprintf (ofp, "\n");

      fclose (ofp);
    }

/************************************************************* 
 *    ici commence l'édition du compte de résultat abrégé    *
 *************************************************************/

  else
    {
      ofp = fopen ("data/fin_result", "w");

      /* titre */
      fprintf (ofp, "\n     Laurier Tomaka - Résultat financier d'exploitation"
	       "\n               du %s au %s\n", print_debut, print_fin);

      /* LINE */
      for (p=0; p<61; p++)
	fprintf (ofp, "-");
      fprintf (ofp, "\n");

      /* colones */
      fprintf (ofp, "%-37s  %5s %4d  %5s %4d\n",
	       "", "Année", (annee_comptable - 1), "Année", annee_comptable);

      /* LINE */
      for (p=0; p<61; p++)
	fprintf (ofp, "-");
      fprintf (ofp, "\n");

      /* ventes */
      for (j=0; j<i; j++)
	{
	  if ((result[j].id >= 7000) && (result[j].id < 7100))
	    {
	      fprintf (ofp, "   +%-33s  %10.2f  %10.2f\n",
		       result[j].lib, 0.00,
		       result[j].cr_fin - result[j].db_fin);
	    }
	}

      /* LINE */
      for (p=0; p<61; p++)
	fprintf (ofp, "-");
      fprintf (ofp, "\n");
    
      /* ventes - ca */
      fprintf (ofp, "%-37s  %10.2f  %10.2f\n",
	       "Ventes (chiffre d'affaire)", 0.00, ventes);

      /* LINE */
      for (p=0; p<61; p++)
	fprintf (ofp, "-");
      fprintf (ofp, "\n");

/* 
* production  stockée, production immobilisée
*      for (j=0; j<i; j++)
*	{
*	  if ((result[j].id >= 7100) && (result[j].id < 7300))
*	    {
*	      fprintf (ofp, "   +%-33s  %10.2f  %10.2f\n",
*		       result[j].lib, 0.00,
*		       result[j].cr_fin - result[j].db_fin);
*	    }
*	}
*      for (p=0; p<61; p++)
*	fprintf (ofp, "-");
*      fprintf (ofp, "\n");
*
* production de l'exercice
*      fprintf (ofp, "%-37s  %10.2f  %10.2f\n",
*	       "Production de l'exercice", 0.00, production);
*
* LINE
*      for (p=0; p<61; p++)
*	fprintf (ofp, "-");
*      fprintf (ofp, "\n");
*/

      /* achats */
      fprintf (ofp, "   -%-33s  %10.2f  %10.2f\n",
	       "Achats", 0.00, achats);
      for (j=0; j<i; j++)
	{
	  if (((result[j].id >= 6000) && (result[j].id < 6030))
	      || ((result[j].id >= 6060) && (result[j].id < 6100)))
	    {
	      fprintf (ofp, "       %-30s  %10.2f  %10.2f\n",
		       result[j].lib, 0.00,
		       result[j].db_fin - result[j].cr_fin);
	    }
	}

      /* LINE */
      for (p=0; p<61; p++)
	fprintf (ofp, "-");
      fprintf (ofp, "\n");
 
/*   
* variations de stocks
*      for (j=0; j<i; j++)
*	{
*	  if ((result[j].id >= 6030) && (result[j].id < 6040))
*	    {
*	      fprintf (ofp, "   -%-33s  %10.2f  %10.2f\n",
*		       result[j].lib, 0.00,
*		       result[j].db_fin - result[j].cr_fin);
*	    }
*	}
*/

      /* marge brute */
      fprintf (ofp, "%-37s  %10.2f  %10.2f\n",
	       "Marge brute", 0.00, marge_fin);

      /* LINE */
      for (p=0; p<61; p++)
	fprintf (ofp, "-");
      fprintf (ofp, "\n");

      /* autres charges externes */
      fprintf (ofp, "   -%-33s  %10.2f  %10.2f\n",
	       "Autres charges externes", 0.00, services);
      for (j=0; j<i; j++)
	{
	  if ((result[j].id >= 6100) && (result[j].id < 6300))
	    {
	      fprintf (ofp, "       %-30s  %10.2f  %10.2f\n",
		       result[j].lib, 0.00,
		       result[j].db_fin - result[j].cr_fin);
	    }
	}

      /* LINE */
      for (p=0; p<61; p++)
	fprintf (ofp, "-");
      fprintf (ofp, "\n");
    
      /* valeur ajoutée */
      fprintf (ofp, "%-37s  %10.2f  %10.2f\n",
	       "Valeur ajoutée", 0.00, valeur_fin);

      /* LINE */
      for (p=0; p<61; p++)
	fprintf (ofp, "-");
      fprintf (ofp, "\n");

      /* taxes, cotisations sociales */
      for (j=0; j<i; j++)
	{
	  if ((result[j].id >= 6300) && (result[j].id < 6500))
	    {
	      fprintf (ofp, "   -%-33s  %10.2f  %10.2f\n",
		       result[j].lib, 0.00,
		       result[j].db_fin - result[j].cr_fin);
	    }
	}

      /* subventions */
      for (j=0; j<i; j++)
	{
	  if ((result[j].id >= 7400) && (result[j].id < 7500))
	    {
	      fprintf (ofp, "   +%-33s  %10.2f  %10.2f\n",
		       result[j].lib, 0.00,
		       result[j].cr_fin - result[j].db_fin);
	    }
	}

      /* LINE */
      for (p=0; p<61; p++)
	fprintf (ofp, "-");
      fprintf (ofp, "\n");


      /* excédent brut */
      fprintf (ofp, "%-37s  %10.2f  %10.2f\n",
	       "Excédent brut d'exploitation", 0.00, excedent_fin);

      /* LINE */
      for (p=0; p<61; p++)
	fprintf (ofp, "-");
      fprintf (ofp, "\n");

/*
* dotations d'amortissements
*      for (j=0; j<i; j++)
*	{
*	  if ((result[j].id >= 6800) && (result[j].id < 6900))
*	    {
*	      fprintf (ofp, "   -%-33s  %10.2f  %10.2f\n",
*		       result[j].lib, 0.00,
*		       result[j].db_fin - result[j].cr_fin);
*	    }
*	}
*
* LINE
*      for (p=0; p<61; p++)
*	fprintf (ofp, "-");
*      fprintf (ofp, "\n");
*/

      /* dotations basées sur rembourcements des emprunts */
      fprintf (ofp, "   -%-33s  %10.2f  %10.2f\n",
	       "Dotations_amortissements", 0.00, DOTATIONS);

      /* LINE */
      for (p=0; p<61; p++)
	fprintf (ofp, "-");
      fprintf (ofp, "\n");

      /* résultat d'exploitation */
      fprintf (ofp, "%-37s  %10.2f  %10.2f\n",
	       "Résultat d'exploitation", 0.00, resultat_fin);

      /* LINE */
      for (p=0; p<61; p++)
	fprintf (ofp, "-");
      fprintf (ofp, "\n");

      /* intérêts */
      for (j=0; j<i; j++)
	{
	  if ((result[j].id >= 6600) && (result[j].id < 6700))
	    {
	      fprintf (ofp, "   -%-33s  %10.2f  %10.2f\n",
		       result[j].lib, 0.00,
		       result[j].db_fin - result[j].cr_fin);
	    }
	}

      /* LINE */
      for (p=0; p<61; p++)
	fprintf (ofp, "-");
      fprintf (ofp, "\n");

/*
* résultat courant
*      fprintf (ofp, "%-37s  %10.2f  %10.2f\n",
*	       "Résultat courant", 0.00, r_courant_fin);
*
* LINE
*      for (p=0; p<61; p++)
*	fprintf (ofp, "-");
*      fprintf (ofp, "\n");
*
* charges exceptionnelles
*      for (j=0; j<i; j++)
*	{
*	  if ((result[j].id >= 6700) && (result[j].id < 6800))
*	    {
*	      fprintf (ofp, "   -%-33s  %10.2f  %10.2f\n",
*		       result[j].lib, 0.00,
*		       result[j].db_fin - result[j].cr_fin);
*	    }
*	}
*
* produits exceptionnelles, reprises d'amortissments
*      for (j=0; j<i; j++)
*	{
*	  if ((result[j].id >= 7700) && (result[j].id < 7900))
*	    {
*	      fprintf (ofp, "   +%-33s  %10.2f  %10.2f\n",
*		       result[j].lib, 0.00,
*		       result[j].cr_fin - result[j].db_fin);
*	    }
*	}
*
* LINE
*      for (p=0; p<61; p++)
*	fprintf (ofp, "-");
*      fprintf (ofp, "\n");
*/

      /* résultat net */
      fprintf (ofp, "%-37s  %10.2f  %10.2f\n",
	       "Résultat net de l'exercice", 0.00, r_net_fin_cour);

      /* LINE */
      for (p=0; p<61; p++)
	fprintf (ofp, "-");
      fprintf (ofp, "\n");

      fclose (ofp);
    }

  system("vim data/result");

  return 0;
}

int print_result(void)
{
  system("lpr data/result");

  return 0;
}
