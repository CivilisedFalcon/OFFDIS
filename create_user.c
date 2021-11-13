/**
 * C program to create User ID and Password and save it to a file.
 * Before executing this program, please create a file (if not already present)
 * named "user_auth.txt" in the same directory as the executable of this code.
 * generate executable by: gcc create_user.c -o create_user
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define DATA_SIZE 100
#define BUFF_SIZE 1000
#define TRUE 1

/*Hash Function: Convert the password into a hash*/
unsigned long hash(unsigned char *str)
{
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

/*this function will get the string array
and split into the words, seprated by #
Here, it will help to check for duplicate Username*/

int getWords(char *base, char target[10][20])
{
	int n=0,i,j=0;

	for(i=0;TRUE;i++)
	{
		if(base[i]!='#'){
			target[n][j++]=base[i];
		}
		else{
			target[n][j++]='\0';//insert NULL
			n++;
			j=0;
		}
		if(base[i]=='\0')
		    break;
	}
	return n;
}

int main()
{
    /* Variable to store userid and password content */
    char userid[DATA_SIZE];
    char passwd[DATA_SIZE];
    char conf_passwd[DATA_SIZE];
    char buffer[BUFF_SIZE];
    char line[BUFF_SIZE];
    char hashpass[DATA_SIZE];
    char sep[5];

    /*Initialising seperator that will seperate different data fields*/
    strcpy(sep, "#");

    /* File pointer to hold reference to our file */
    FILE * fPtr;

    /* Input contents from user to store in file */
    printf("Please enter your User Name: \n");
    fgets(userid, DATA_SIZE, stdin);
    userid[ strlen(userid) - 1 ] = '\0'; //removing the trailing newline character

    printf("Please enter your Password: \n");
    fgets(passwd, DATA_SIZE, stdin);
    passwd[ strlen(passwd) - 1 ] = '\0'; //removing the trailing newline character

    printf("Please Confirm your Password: \n");
    fgets(conf_passwd, DATA_SIZE, stdin);
    conf_passwd[ strlen(conf_passwd) - 1 ] = '\0'; //removing the trailing newline character

    /*checking for duplicate Username*/
    fPtr = fopen("user_auth.txt", "r");

    /* fopen() return NULL if last operation was unsuccessful */
    if(fPtr == NULL)
    {
        /* File not created hence exit */
        printf("Unable to read file for checking username.\n");
        exit(EXIT_FAILURE);
    }

    while (fgets(line, BUFF_SIZE, fPtr)) {
      char arr[10][20];
      int n=0;
	    n=getWords(line,arr);
      if(strcmp(arr[0], userid) == 0)
      {
        printf("User Already exists");
        fclose(fPtr);
        exit(EXIT_FAILURE);
      }
    }
    /* Close file to save file data */
    fclose(fPtr);



    if(strcmp(passwd, conf_passwd) == 0)
    {

      printf("The Passwords Match. Creating User Acccount. \n");

      /*
       * Open file in a+ (append) mode.
       * "user_auth.txt" is relative path to create file
       */
      fPtr = fopen("user_auth.txt", "a+");

      /* fopen() return NULL if last operation was unsuccessful */
      if(fPtr == NULL)
      {
          /* File not created hence exit */
          printf("Unable to create file.\n");
          exit(EXIT_FAILURE);
      }


      // converting the resultant hash(int) to hashpass(char)
      unsigned long hash1 = hash(passwd);
      snprintf( hashpass, DATA_SIZE, "%d\n", hash1 );

      //iota(hash1, hashpass, 10); //another method to convert a int to a char

      /*Concatenate User Name, seperator, and hash(Password) */
      strcpy(buffer, userid);
      strcat(buffer, sep);
      strcat(buffer, hashpass);

      /* Write data to file */
      fputs(buffer, fPtr);

      /* Close file to save file data */
      fclose(fPtr);

      /* Success message */
      printf("User %s created and saved Successfully :) \n", userid);
    }
    else
    {
      printf("Passwords not match. Sorry. Try Again :( \n");
    }

    return 0;
}
