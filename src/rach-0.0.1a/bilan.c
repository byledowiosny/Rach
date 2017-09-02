#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stddef.h>
#include "C.h"

int edit_bilan(void)
{  
  int annee_comptable=0, i, j, who=0, dnport, date, c = 0, p;
  FILE *ifp, *ofp;
  char snport;
  rap_fin bilan[100];
  float db=0, cr=0, fnport;
  float immobilise = 0, stocks = 0, creances = 0;
  float disponibilites = 0, circulant = 0, actif = 0;
  float capitaux = 0, long_terme = 0;
  float court_terme = 0, passif = 0, result = 0;

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

  /* fait une structure de i comptes de bilan avec compte_id, _lib, _file */
  /* a partir de fichier plan_compta */
  i = 0;
  ifp = fopen ("data/plan_compta", "r");
  while ((fscanf (ifp, "%d", &bilan[i].id) != EOF))
    {
      if ((bilan[i].id >= 1000) && (bilan[i].id < 6000))
	{
	  fscanf (ifp, "%s", bilan[i].lib);
	  fscanf (ifp, "%d", &dnport);
	  fscanf (ifp, "%s", bilan[i].file);
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
      bilan[j].db_fin = 0;
      bilan[j].cr_fin = 0;
      ifp = fopen (bilan[j].file, "r");
      while ((fscanf (ifp, "%d", &dnport) != EOF))
	{
	  fscanf (ifp, "%d", &date);
	  fscanf (ifp, "%f", &db);
	  fscanf (ifp, "%f", &cr);
	  fscanf (ifp, "%f", &fnport);
	  if ((date >= atoi(file_debut)) && (date <= atoi(file_fin)))
	    {
	      bilan[j].db_fin += db;
	      bilan[j].cr_fin += cr;
	    }
	}
      fclose(ifp);
    }

  for (j=0; j<i; j++)
    {
      c = bilan[j].id;
      db = bilan[j].db_fin;
      cr = bilan[j].cr_fin;
      /* actif */
      if ((c >= 2000) && (c < 3000))
	{
	  immobilise += db;
	  immobilise -= cr;
	}
      else if ((c >= 3000) && (c < 4000))
	{
	  stocks += db;
	  stocks -= cr;
	}
      else if (((c >= 4100) && (c < 4200)) || ((c >= 4400) && (c < 4500)) ||
	       ((c >= 4600) && (c < 4640)))
	{
	  creances += db;
	  creances -= cr;
	}
      else if ((c >= 5000) && (c < 6000))
	{
	  disponibilites += db;
	  disponibilites -= cr;
	}
      /* passif */
      else if(((c >= 1000) && (c < 1600)) || ((c >= 4500) && (c < 4600)))
	{
	  capitaux -= db;
	  capitaux += cr;
	}
      else if ((c >= 1600) && (c < 2000))
	{
	  long_terme -= db;
	  long_terme += cr;
	}
      else if (((c >= 4000) && (c < 4100)) || ((c >= 4200) && (c < 4400))
	       || ((c >= 4640) && (c < 4700)))
	{
	  court_terme -= db;
	  court_terme += cr;
	}
      c = 0;
      db = 0;
      cr = 0;
    }

  circulant = creances + disponibilites;
  actif = immobilise + stocks + circulant;
  passif = capitaux + long_terme + court_terme;
  result = actif - passif;
  capitaux += result; 
  passif += result;

  printf ("0 - Un bilan waw\n"
	  "1 - Un bilan complet: ");
  scanf ("%d", &who);

  ofp = fopen ("data/bilan", "w");
  
  if (who == 1)
    {
      fprintf (ofp, "\n     Global Tomaka - Bilan du %s au %s\n",
	       print_debut, print_fin);
    }
  else
    {
      fprintf (ofp, "\n     Laurier Tomaka - Bilan du %s au %s\n",
	       print_debut, print_fin);
    }
  for (p=0; p<63; p++)
    fprintf (ofp, "-");
  fprintf (ofp, "\n");


  fprintf (ofp, "%-39s  %5s %4d  %5s %4d\n",
	  "ACTIF", "Ann�", (annee_comptable - 1), "Ann�", annee_comptable);
  for (p=0; p<63; p++)
    fprintf (ofp, "-");
  fprintf (ofp, "\n");

  if (who == 1)
    {
      for (j=0; j<i; j++)
	{
	  if ((bilan[j].id >= 2000) && (bilan[j].id < 3000))
	    {
	      fprintf (ofp, "   +%-35s  %10.2f  %10.2f\n",
		       bilan[j].lib, 0.00,
		       bilan[j].db_fin - bilan[j].cr_fin);
	    }
	}
      for (p=0; p<63; p++)
	fprintf (ofp, "-");
      fprintf (ofp, "\n");
    }

  fprintf (ofp, "  %-37s  %10.2f  %10.2f\n",
	   "Actif immobilis�, 0.00, immobilise);
  for (p=0; p<63; p++)
    fprintf (ofp, "-");
  fprintf (ofp, "\n");

  if (who == 1)
    {
      for (j=0; j<i; j++)
	{
	  if ((bilan[j].id >= 3000) && (bilan[j].id < 4000))
	    {
	      fprintf (ofp, "   +%-35s  %10.2f  %10.2f\n",
		       bilan[j].lib, 0.00,
		       bilan[j].db_fin - bilan[j].cr_fin);
	    }
	}
      for (p=0; p<63; p++)
	fprintf (ofp, "-");
      fprintf (ofp, "\n");
    }

  fprintf (ofp, "  %-37s  %10.2f  %10.2f\n",
	   "Stocks", 0.00, stocks);
  for (p=0; p<63; p++)
    fprintf (ofp, "-");
  fprintf (ofp, "\n");

  fprintf (ofp, "   +%-35s  %10.2f  %10.2f\n",
	   "Cr�nces", 0.00, creances);

  if (who == 1)
    {
      for (j=0; j<i; j++)
	{
	  if (((bilan[j].id >= 4100) && (bilan[j].id < 4200)) ||
	      ((bilan[j].id >= 4400) && (bilan[j].id < 4500)) ||
	      ((bilan[j].id >= 4600) && (bilan[j].id < 4640)))
	    {
	      fprintf (ofp, "      %-33s  %10.2f  %10.2f\n",
		       bilan[j].lib, 0.00,
		       bilan[j].db_fin - bilan[j].cr_fin);
	    }
	}
    }

  fprintf (ofp, "   +%-35s  %10.2f  %10.2f\n",
	   "Disponibilit�", 0.00, disponibilites);

  if (who == 1)
    {
      for (j=0; j<i; j++)
	{
	  if ((bilan[j].id >= 5000) && (bilan[j].id < 6000))
	    {
	      fprintf (ofp, "      %-33s  %10.2f  %10.2f\n",
		       bilan[j].lib, 0.00,
		       bilan[j].db_fin - bilan[j].cr_fin);
	    }
	}
    }

  for (p=0; p<63; p++)
    fprintf (ofp, "-");
  fprintf (ofp, "\n");

  fprintf (ofp, "  %-37s  %10.2f  %10.2f\n",
	   "Autre actif circulant", 0.00, circulant);
  for (p=0; p<63; p++)
    fprintf (ofp, "-");
  fprintf (ofp, "\n");

  fprintf (ofp, "%-39s  %10.2f  %10.2f\n",
	   "Total actif", 0.00, actif);
  for (p=0; p<63; p++)
    fprintf (ofp, "-");
  fprintf (ofp, "\n");

  fprintf (ofp, "%-39s  %5s %4d  %5s %4d\n",
	  "PASSIF", "Ann�", (annee_comptable - 1), "Ann�", annee_comptable);
  for (p=0; p<63; p++)
    fprintf (ofp, "-");
  fprintf (ofp, "\n");

  if (who == 1)
  {
    for (j=0; j<i; j++)
      {
	if (((bilan[j].id >= 1000) && (bilan[j].id < 1600)) ||
	    ((bilan[j].id >= 4500) && (bilan[j].id < 4600)))
	  {
	    fprintf (ofp, "   +%-35s  %10.2f  %10.2f\n",
		     bilan[j].lib, 0.00,
		     bilan[j].cr_fin - bilan[j].db_fin);
	  }
      }
  }

  fprintf (ofp, "   +%-35s  %10.2f  %10.2f\n",
	   "R�ultat de l'exercice", 0.00, result);
  for (p=0; p<63; p++)
  fprintf (ofp, "-");
  fprintf (ofp, "\n");

  fprintf (ofp, "  %-37s  %10.2f  %10.2f\n",
	   "Capitaux propres", 0.00, capitaux);
  for (p=0; p<63; p++)
    fprintf (ofp, "-");
  fprintf (ofp, "\n");

  if (who == 1)
  {
    for (j=0; j<i; j++)
      {
	if ((bilan[j].id >= 1600) && (bilan[j].id < 2000))
	  {
	    fprintf (ofp, "   +%-35s  %10.2f  %10.2f\n",
		     bilan[j].lib, 0.00,
		     bilan[j].cr_fin - bilan[j].db_fin);
	  }
      }
    for (p=0; p<63; p++)
      fprintf (ofp, "-");
    fprintf (ofp, "\n");
  }

  fprintf (ofp, "  %-37s  %10.2f  %10.2f\n",
	   "Dettes �long terme", 0.00, long_terme);
  for (p=0; p<63; p++)
    fprintf (ofp, "-");
  fprintf (ofp, "\n");

  if (who == 1)
  {
    for (j=0; j<i; j++)
      {
	if (((bilan[j].id >= 4000) && (bilan[j].id < 4100)) ||
	    ((bilan[j].id >= 4200) && (bilan[j].id < 4400)) ||
	    ((bilan[j].id >= 4640) && (bilan[j].id < 4700)))

	  {
	    fprintf (ofp, "   +%-35s  %10.2f  %10.2f\n",
		     bilan[j].lib, 0.00,
		     bilan[j].cr_fin - bilan[j].db_fin);
	  }
      }
    for (p=0; p<63; p++)
      fprintf (ofp, "-");
    fprintf (ofp, "\n");
  }

  fprintf (ofp, "  %-37s  %10.2f  %10.2f\n",
	   "Dettes �court terme", 0.00, court_terme);
  for (p=0; p<63; p++)
    fprintf (ofp, "-");
  fprintf (ofp, "\n");

  fprintf (ofp, "%-39s  %10.2f  %10.2f\n",
	   "Total passif", 0.00, passif);
  for (p=0; p<63; p++)
    fprintf (ofp, "-");

  fclose (ofp);

  system("vim data/bilan");

  return 0;
}
 
int print_bilan(void)
{
  system("lpr data/bilan");

  return 0;
}
