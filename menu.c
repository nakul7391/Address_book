#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include "menu.h"
#include "constants.h"

/*Display the menu*/
Status disp_menu()
{
    int option, what;
    PersonDetail info;
    option = show_menu();
    switch(option)
    {
        case Insert:if(get_user_details(&info) == e_failure)
                    {
                      printf("Unable to get user details\n");
                      return e_failure;
                    }

                    if(open_phone_book(&info,option) == e_failure)
                    {
                      printf("Unable to open file\n");
                      return e_failure;
                    }

                    if(write_phone_book(&info) == e_failure)
                    {
                      printf("Unable to write file\n");
                      return e_failure;
                    }

                    if(close_phone_book(info.fptr_phonebook) == e_failure)
                    {
                      printf("Unable to close file\n");
                      return e_failure;
                    }

                    disp_menu();
                    break;
        case Search:if(disp_search_menu(info.search_query,&what) == e_success)
                    {
                        if(search_phone_book(&info,&what) == e_failure )
                        {
                          printf("No data exists\n");
                        }
                        if(close_phone_book(info.fptr_phonebook) == e_failure)
                        {
                            printf("Unable to close file\n");
                            return e_failure;
                        }
                    }
                    else
                    printf("Wrong search option\n");

                    disp_menu();
                    break;
        case Edit:if(open_phone_book(&info,Display) == e_failure)
                      {
                        printf("Unable to open file for reading\n");
                        return e_failure;
                      }

                      /*display the data*/
                      disp_data(&info);
                      printf("Enter the exact name you want to edit\n");
                      if(get_delete_name(info.search_query) == e_failure)
                      {
                        printf("Unable to get user input\n");
                      }
                      what = 1;
                      if(search_phone_book(&info,&what) == e_failure )
                      {
                        printf("No data exists\n");
                        return e_failure;
                      }

                      if(show_edit_menu(&what) == e_failure)
                      {
                        printf("Invalid option\n");
                        return e_failure;
                      }
                      //edit the data
                       if(edit_data(&info, &what) == e_failure)
                       {
                         printf("Unable to edit data\n");
                         return e_failure;
                       }
                       else
                       printf("Data successfully edited\n");

                       disp_menu();
                    break;
        case Delete:if(open_phone_book(&info,option) == e_failure)
                      {
                        printf("Unable to open file for reading\n");
                        return e_failure;
                      }

                      /*display the data*/
                      disp_data(&info);
                      /*ask user for name*/
                      printf("Enter the exact name you want to delete\n");
                      if(get_delete_name(info.search_query) == e_failure)
                      {
                        printf("Unable to get user input\n");
                      }

                      /*search if data exists or not*/
                      what = 1;
                      if(delete_data(&info,&what) == e_failure)
                      {
                        printf("Unable to delete data\n");
                      }

                      disp_menu();
                    break;
        case Display:if(open_phone_book(&info,option) == e_failure)
                      {
                        printf("Unable to open file for reading\n");
                        return e_failure;
                      }

                      /*display the data*/
                      disp_data(&info);

                      /*close file*/
                      if(close_phone_book(info.fptr_phonebook) == e_failure)
                      {
                        printf("Unable to close file\n");
                        return e_failure;
                      }

                      disp_menu();
                    break;
        case Exit:printf("Program exiting...\n");
                    exit(0);
                    break;
        case Error:printf("Invalid option\n");
                    __fpurge(stdin);
                    disp_menu();
                    break;
    }

    return e_success;
}

Option show_menu()
{
    int opt;
    printf("***************MENU***************\n");
    printf("1. %-7s| 2. %-7s| 3. %-7s| 4. %-7s| 5. %-7s| 6. %-7s\n",INSERT,EDIT,SEARCH,DELETE,DISPLAY,EXIT);
    printf("Enter your choice: ");
    scanf("%d", &opt );
    if( opt >= 1 && opt <=6 )
    return opt;
    else
    return Error;
}

/*get user input*/
Status get_user_details(PersonDetail *info)
{
    __fpurge(stdin);
    printf("Enter name: ");
    scanf("%[^\n]",info->name);
    __fpurge(stdin);
    printf("Enter contact number: ");
    scanf("%ld",&info->contactNo);
      __fpurge(stdin);
    printf("Enter address: ");
    scanf("%[^\n]",info->address);
      __fpurge(stdin);
    printf("Enter email address: ");
    scanf("%[^\n]s",info->email);
      __fpurge(stdin);
    printf("Enter DoB(DD MM YYYY): ");
    scanf("%d %d %d",&info->Dob.day,&info->Dob.month,&info->Dob.year);
    __fpurge(stdin);
  /*  printf("name %s\n",info->name);
    printf("address %s\n",info->address);
    printf("contact %ld\n",info->contactNo);
    printf("email %s\n",info->email );
    printf("dob %d %d %d\n",info->Dob.day,info->Dob.month,info->Dob.year);
*/
    return e_success;
}

/*open the file*/
Status open_phone_book(PersonDetail *info, int option)
{
    if(option == Insert)
    info->fptr_phonebook = fopen(PHONEBOOK,"a+");

    /*for display and search mode open with read mode*/
    if(option == Display || option == Search || option == Delete)
    info->fptr_phonebook = fopen(PHONEBOOK,"r");

    if (info->fptr_phonebook == NULL)
    {
      perror("fopen");
      fprintf(stderr, "ERROR: Unable to open file %s\n",PHONEBOOK);
      return e_failure;
    }

    fseek(info->fptr_phonebook,0L,SEEK_END);
    info->phone_book_size = ftell(info->fptr_phonebook);
    rewind(info->fptr_phonebook);
    return e_success;
}

/*write data in file*/
Status write_phone_book(PersonDetail *info)
{
  long size;
  /*write in file*/
  printf("string %s\n",info->name);
  fprintf(info->fptr_phonebook,"%s,%ld,%s,%s,%d-%d-%d\n",info->name,info->contactNo,
  info->address,info->email,info->Dob.day,info->Dob.month,info->Dob.year);

  size = sizeof(info->name)+sizeof(info->contactNo)+ sizeof(info->address) + sizeof(info->email) + sizeof(int) * 3;

  if( size == ftell(info->fptr_phonebook))
  return e_success;
  else
  e_failure;
}

void disp_data(PersonDetail *info)
{
    char buff[1000], c, *ptr;
    int i, index;
    /*display header*/
    printf("%-20s| %-11s| %-30s| %-30s| %-9s\n","Name","Contact No.","Address","Email ID","Date of Birth");
    while( info->phone_book_size > ftell(info->fptr_phonebook))
    {
        i = 0;
        while( (c = fgetc(info->fptr_phonebook)) != '\n' )
        {
            buff[i] = c;
            i++;
        }
        buff[i] = '\0';
        //printf("%s\n",buff);
        //fseek(info->fptr_phonebook,1L,SEEK_CUR);
        ptr = strtok(buff,",");
        index = 0;
        while(ptr != NULL )
        {
          printf("%*s| ",-get_width(index),ptr);
          ptr = strtok(NULL,",");
          index++;
        }
        printf("\n");
    }

}

int get_width(int i)
{
    switch(i)
    {
      case 0: return NameLen;
      case 1: return Contactlen;
      case 2: return AddressLen;
      case 3: return EmailIdlen;
      case 4: return DobLen;
    }
}

Status disp_search_menu(char *query, int *what)
{
    printf("Search by.......\n");
    printf("1. %-20s| 2. %-11s| 3. %-30s| 4. %-20s| 5. %-9s\n","Name","Contact No.","Address","Email id","Date of birth");
    printf("Enter option: ");
    scanf("%d", what );

    if(*what >= 1 && *what <=5 )
    {
        printf("Enter your search query.......\n");
        scanf("%s",query);
        return e_success;
    }

    return e_failure;
}

Status search_phone_book(PersonDetail *info, int *what)
{
    char buff[1000],tempBuff[1000], c, *ptr, *bptr;
    int i, index, lock = 0;

    if(open_phone_book(info, Search) == e_failure)
    {
      printf("Unable to open file\n");
      return e_failure;
      i = 0;
    }

    while(info->phone_book_size > ftell(info->fptr_phonebook))
    {
        i = 0;
        while( (c = fgetc(info->fptr_phonebook)) != '\n' )
        {
            buff[i] = c;
            i++;
        }
        buff[i] = '\0';
        //printf("%s\n",buff);
        //fseek(info->fptr_phonebook,1L,SEEK_CUR);
        index = *what - 1;
        strcpy(tempBuff,buff);
        /*find string get string upto comma*/
        ptr = strtok(tempBuff,",");
        /*move the pointer*/
        while(index-- && ptr !=NULL)
        {
            ptr = strtok(NULL,",");
        }

        if(ptr != NULL)
        {
          if(strstr(ptr,info->search_query) != NULL)
          {
              if(lock == 0)
              {
              printf("******************************Search results******************************\n");
              printf("%-20s| %-11s| %-30s| %-20s| %-9s\n","Name","Contact No.","Address","Email ID","Date of Birth");
              lock = 1;
              }
              bptr = strtok(buff,",");
              index = 0;
              while( bptr != NULL )
              {
                printf("%*s| ",-get_width(index),bptr);
                bptr = strtok(NULL,",");
                index++;
              }
              printf("\n");
          }
        }
    }

    if(lock == 1)
    return e_success;
    else
    return e_failure;
}

Status get_delete_name(char *name)
{
    __fpurge(stdin);
    if(scanf("%[^\n]s",name) == 1 )
    return e_success;
    else
    return e_failure;
}

Status delete_data(PersonDetail *info, int *what)
{
    char buff[1000],tempBuff[1000], c, *ptr, *bptr, prompt;
    int i, index, lock = 0;
    FILE *fptr;
    printf("Do you really want to delete entry %s (Y/N)\n",info->search_query);
    scanf(" %c", &prompt );
    if( prompt == 'n' || prompt == 'N')
    return e_failure;

    info->fptr_phonebook = fopen(PHONEBOOK,"r");
    fptr = fopen(TEMPPHONEBOOK,"w");

    while(info->phone_book_size > ftell(info->fptr_phonebook))
    {
        i = 0;
        while( (c = fgetc(info->fptr_phonebook)) != '\n' )
        {
            buff[i] = c;
            i++;
        }
        buff[i] = '\0';
        strcpy(tempBuff,buff);
        /*find string get string upto comma*/
        ptr = strtok(tempBuff,",");
        if(ptr != NULL)
        {
          //exact match delete the string
          if(strcmp(ptr,info->search_query) != 0)
          {
              fprintf(fptr,"%s\n",buff);
          }
        }
    }

    fclose(fptr);
    fclose(info->fptr_phonebook);

    fptr = fopen(TEMPPHONEBOOK,"r");
    info->fptr_phonebook = fopen(PHONEBOOK,"w");

    while( (c = fgetc(fptr)) != EOF )
    {
        fprintf(info->fptr_phonebook,"%c",c);
    }

    fclose(fptr);
    fclose(info->fptr_phonebook);

    return e_success;
}

Status show_edit_menu(int *what)
{
    printf("What you want to edit\n");
    printf("1. %-15s| 2. %-15s| 3. %-15s| 4. %-15s| 5. %-15s\n","Name","Contact No.","Address","Email id","Date of birth");
    scanf("%d",what);
    if(*what >= 1 && *what <= 5)
    return e_success;
    else
    return e_failure;
}

Status edit_data(PersonDetail *info, int *what)
{
    FILE *fptr;
    char buff[1000],tempBuff[1000],finalBuff[1000],*ptr, c, *nptr;
    int i, index;
    __fpurge(stdin);
    switch(*what)
    {
        case 1:printf("Enter name: ");
                scanf("%[^\n]s",info->edit_data_string);
              break;
        case 2:printf("Enter contact no.: ");
              scanf("%[^\n]s",info->edit_data_string);
              break;
        case 3:printf("Enter address: ");
              scanf("%[^\n]s",info->edit_data_string);
              break;
        case 4:printf("Enter email if: ");
              scanf("%[^\n]s",info->edit_data_string);
              break;
        case 5:printf("Enter Date of birth(DD-MM-YYYY): ");
              scanf("%[^\n]s",info->edit_data_string);
              break;
    }

    //open file for reading
    info->fptr_phonebook = fopen(PHONEBOOK,"r");
    //open temp file
    fptr = fopen(TEMPPHONEBOOK,"w");
    if(fptr == NULL)
    {
      perror("");
      return e_failure;
    }

    while(info->phone_book_size > ftell(info->fptr_phonebook))
    {
        i = 0;
        while( ( c = fgetc(info->fptr_phonebook)) != '\n' )
        {
           buff[i] = c;
           i++;
        }
        buff[i] = '\0';
        strcpy(tempBuff,buff);
        ptr = strtok(tempBuff,",");

        if( strcmp(ptr,info->search_query) == 0)
        {
            strcpy(tempBuff,buff);
            index = *what - 1;
            ptr = strtok(tempBuff,",");
            finalBuff[0] = '\0';
            while(index-- && ptr != NULL )
            {
                if(finalBuff[0] == '\0')
                strcat(finalBuff,ptr);
                else
                {
                strcat(finalBuff,",");
                strcat(finalBuff,ptr);
                }
          //      printf("%s\n",finalBuff);
                ptr = strtok(NULL,",");
            }
            if( *what != 1 )
            sprintf(finalBuff,"%s,",finalBuff);
            //printf("ptr %s sprit %s\n",ptr,finalBuff);
            strcat(finalBuff,info->edit_data_string);
            sprintf(finalBuff,"%s",finalBuff);
            //printf("sprint %s\n",finalBuff);
            strcpy(tempBuff,buff);
            ptr = strtok(tempBuff,",");
            index = *what;
            while(index--)
            {
              //  printf("%s\n",ptr);
                ptr = strtok(NULL,",");
            }
            while( index < 4 - *what)
            {
                sprintf(finalBuff,"%s,%s",finalBuff,ptr);
                ptr = strtok(NULL,",");
                index++;
            }
            printf("%s\n",finalBuff);
            fwrite(finalBuff,1,strlen(finalBuff),fptr);
            fprintf(fptr,"%s","\n");
        }
        else
        {
            fwrite(buff,1,strlen(buff),fptr);
            fprintf(fptr,"%s","\n");
        }

    }

    fclose(fptr);
    fclose(info->fptr_phonebook);

    fptr = fopen(TEMPPHONEBOOK,"r");
    info->fptr_phonebook = fopen(PHONEBOOK,"w");

    while( (c = fgetc(fptr)) != EOF )
    {
        fprintf(info->fptr_phonebook,"%c",c);
    }

    fclose(fptr);
    fclose(info->fptr_phonebook);

    return e_success;

}



Status close_phone_book(FILE *fptr_phonebook)
{
    if(fclose(fptr_phonebook) == 0)
    return e_success;

    return e_failure;
}
