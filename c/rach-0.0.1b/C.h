#include <time.h>

typedef struct
  {
    int id;
    char lib[50];
    char file[50];
    float db_fin;
    float cr_fin;
  }
  rap_fin;

typedef struct
  {
    int id;
    char lib[50];
    char file[50];
    int debut_kg;
    int db_kg;
    int cr_kg;
    int fin_kg;
  }
  rap_phys;

typedef struct
  {
    int id;
    char lib[50];
    int auto_t;
    char file[50];
    int auto_c;
  }
  compte;

int this_day(void);
int this_month(void);
int this_year(void);
int this_century(void);

time_t calendar_date(void);
char *broken_date(time_t);

time_t first_date(void);
time_t last_date(void);

int menu(void);
int saisir(void);
int edit_prix(void);
int mouv_stock(void);
int edit_tva(void);
int print_tva(void);
int edit_result(void);
int print_result(void);
int edit_bilan(void);
int print_bilan(void);
int edit_analytique(void);
int print_analytique(void);
int edit_plan(void);
int print_plan(void);
int touch_compta(void);
int menu_new(void);
int import_plan(void);
int do_files(void);
int open(void);
int import_prix(void);
int edit_prix(void);
