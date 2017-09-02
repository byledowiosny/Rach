#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stddef.h>
#include "C.h"

int open(void)
{
  FILE *ifp, *ofp;
  int op_id;
  time_t calendar;
  int id=0, r_id=0, co_id=0, dnport=0;
  int d=0;
  char snport[100], r_lib[100];
  char r_file[100], co_file[100], print_date[100], date_op[100];
  float montant=0;
  float r_db=0, r_cr=0, co_db=0, co_cr=0;
  int d_c, o_n=0;
  
  ifp = fopen ("data/id_file", "r");
  fscanf (ifp, "%d", &op_id);
  fclose(ifp);

  ifp = fopen ("data/ctp_report", "r");
  fscanf (ifp, "%d", &co_id);
  fclose(ifp);

  do
    {
      do
	{
	  d = 0; 
	  printf ("\nChoisissez un compte de registre.\n\n"
		  " 0 - Aucun, je veux sortir du programme !\n");
	  ifp = fopen ("data/plan_compta", "r");
	  while ((fscanf (ifp, "%d", &id) != EOF))
	    {
	      fscanf (ifp, "%s", r_lib);
	      fscanf (ifp, "%d", &dnport);
	      fscanf (ifp, "%s", snport);
	      fscanf (ifp, "%d", &dnport);
	      if (((id >= 1000) && (id < 3000))
	      || ((id >= 4000) && (id < 6000)))
		printf (" %4d - %-24s\n", id, r_lib);
	    }
	  fclose(ifp);  
	  printf ("\nRegistre Nr: ");
	  scanf ("%d", &r_id);
	  if (r_id == 0)
	    exit (1);
	  else
	    {
	      ifp = fopen ("data/plan_compta", "r");
	      while ((fscanf (ifp, "%d", &id) != EOF))
		{
		  if (r_id == id)
		    {
		      fscanf (ifp, "%s", r_lib);
		      fscanf (ifp, "%d", &dnport);
		      fscanf (ifp, "%s", r_file);
		      fscanf (ifp, "%d", &dnport);
		      d = -1;   
		    }
		  else if (co_id == id)
		    {
		      fscanf (ifp, "%s", snport);
		      fscanf (ifp, "%d", &dnport);
		      fscanf (ifp, "%s", co_file);
		      fscanf (ifp, "%d", &dnport);
		    }
		  else
		    {
		      fscanf (ifp, "%s", snport);
		      fscanf (ifp, "%d", &dnport);
		      fscanf (ifp, "%s", snport);
		      fscanf (ifp, "%d", &dnport);
		    }
		}	
	      fclose(ifp);  
	    }
	}
      while (d == 0);

      calendar = calendar_date();
      strftime(print_date, 100, "%d %b %Y", localtime(&calendar));
      strftime(date_op, 100, "%Y%m%d", localtime(&calendar));
      
      printf ("Montant du solde: ");
      scanf ("%f", &montant);
      if (montant == 0)
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
			  r_db = co_cr = montant;
			  r_cr = co_db = 0;
			}
		      else if (d_c == 2)
			{ 
			  r_cr = co_db = montant;
			  r_db = co_cr = 0;
			}
		      else
			d = -1;
		    }
		}
	      while (d != 0);
	    }

      ofp = fopen ("data/livre", "a");
      fprintf (ofp, "%4d %12s %4d %10.2f %10.2f %4d %4d %5d\n",
	       op_id, date_op, r_id, r_db, r_cr, 1100, 0, 0);
      fclose (ofp);

      ofp = fopen (r_file, "a");
      fprintf (ofp, "%4d %12s %10.2f %10.2f %1d\n",
	       op_id, date_op, r_db, r_cr, 0);
      fclose (ofp);

      ofp = fopen (co_file, "a");
      fprintf (ofp, "%4d %12s %10.2f %10.2f %1d\n",
	       op_id, date_op, co_db, co_cr, 0);
      fclose (ofp);

      op_id++;
      ofp = fopen ("data/id_file", "w");
      fprintf (ofp, "%d", op_id);
      fclose (ofp);
  
      printf ("Un autre registre ? Oui - 1; Non - 2: ");
      scanf ("%d", &o_n);
    }
  while (o_n == 1);
  
  return 0;
}
