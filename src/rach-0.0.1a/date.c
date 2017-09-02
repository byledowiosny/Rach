#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stddef.h>
#include "C.h"

int this_day(void)
{
  time_t now = time ((time_t *) NULL);
  struct tm *today = localtime(&now);
  return today -> tm_mday;
}

int this_month(void)
{
  time_t now = time ((time_t *) NULL);
  struct tm *today = localtime(&now);
  return today -> tm_mon + 1;
}

int this_year(void)
{
  time_t now = time ((time_t *) NULL);
  struct tm *today = localtime(&now);
  return today -> tm_year + 1900;
}

int this_century(void)
{
  static int century = 0;

  if (!century) {
    time_t now = time ((time_t *) NULL);
    now /= 3600;
    now /= 24;
    now /= 365.25;
    now /= 100;
    now += 1970;
    century = now - (now % 100);
  }
  return century;
}

time_t calendar_date(void)
{
  FILE *ifp;
  struct tm *tp;
  time_t now;
  int date, jour = 0, mois = 0;
  static time_t calendar;
  int annee_comptable = 0;

  ifp = fopen ("data/annee", "r");
  fscanf (ifp, "%d", &annee_comptable);
  fclose(ifp);

  printf ("(aujourd'hui - 0; ");
  printf ("un autre jour - j;\n");
  printf ("un autre mois - jmm; ");
  printf ("l'annee comptable - %d): ", annee_comptable);
  scanf ("%d", &date);
  if (date == 0) {
    jour = this_day();
    mois = this_month();
  }
  else if (date >= 0 && date <= 31) {
    jour = date;
    mois = this_month();
  }
  else if (date >= 101 && date <= 3112) {
    jour = date / 100;
    mois = date % 100;
  }

  tp = localtime(&now);

  tp -> tm_sec = 0;
  tp -> tm_min = 0;
  tp -> tm_hour = 0;
  tp -> tm_mday = jour;
  tp -> tm_mon = mois - 1;
  tp -> tm_year = annee_comptable - 1900;
  
  calendar = mktime(tp);

  return calendar;
}

char *broken_date(time_t calen)
{
  static char bd[100];

  strftime(bd, 100, "%d %b %Y", localtime(&calen));

  return bd;
}

time_t first_date(void)
{
  time_t debut;

  printf ("\nLa date du debut ");
  debut = calendar_date();
  return debut;
}

time_t last_date(void)
{
  time_t fin;

  printf ("\nLa date de la fin ");
  fin = calendar_date();
  return fin; 
}

