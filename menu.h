#ifndef MENU_H
#define MENU_H
#include "constants.h"
typedef enum
{
    Insert = 1,
    Edit,
    Search,
    Delete,
    Display,
    Exit,
    Error
}Option;

/*return success failure message*/
typedef enum
{ e_failure = 0,
  e_success
} Status;

/*store personal details*/
typedef struct _PersonDetail
{
  char name[MAX_NAME_LENGTH];
  long contactNo;
  char address[MAX_ADDRESS_LENGTH];
  char email[MAX_EMAIL_LENGTH];
  char search_query[MAX_QUERY_LENGTH];
  long phone_book_size;
  char edit_data_string[MAX_ADDRESS_LENGTH];
  struct Dob
  {
    int day;
    int month;
    int year;
  } Dob;
  FILE *fptr_phonebook;
} PersonDetail;

/*Display the menu*/
Status disp_menu();
/*show menu and return entered value*/
Option show_menu();
/*get user details in structure*/
Status get_user_details(PersonDetail *info);
/*open file and store pointer*/
Status open_phone_book(PersonDetail *info, int option);
/*write data*/
Status write_phone_book(PersonDetail *info);
/*display the data*/
void disp_data(PersonDetail *info);
/*close phone book*/
int get_width(int index);
/*display search menu */
Status disp_search_menu(char *query, int *what);
/*search phone book for query*/
Status search_phone_book(PersonDetail *info, int *what);
/*get name to delete*/
Status get_delete_name(char *name);
/*close file*/
Status delete_data(PersonDetail *info,int *what);
/*ask user what data he wants to edit*/
Status show_edit_menu(int *what);
/*edit data*/
Status edit_data(PersonDetail *info, int *what);
Status close_phone_book(FILE *fptr_phonebook);
#endif
