/**
 * C program to authenticate User ID and Password
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
and split into the words, seprated by # */

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
    char hashpass[DATA_SIZE];
    char line[BUFF_SIZE];
    char sep[10];
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

    // converting the resultant hash(int) to hashpass(char)
    unsigned long hash1 = hash(passwd);
    snprintf( hashpass, DATA_SIZE, "%d", hash1 );


    /*
     * Open file in r (read) mode.
     * "user_auth.txt" is relative path to create file
     */
    fPtr = fopen("user_auth.txt", "r");

    /* fopen() return NULL if last operation was unsuccessful */
    if(fPtr == NULL)
    {
        /* File not created hence exit */
        printf("Unable to read user_auth.txt file.\n");
        exit(EXIT_FAILURE);
    }

    /*Reading each line of the file*/
    while (fgets(line, BUFF_SIZE, fPtr)) {
      char arr[10][20];
      int n=0;
	    n=getWords(line,arr);
      //printf("%s,%s\n",arr[0], arr[1]);
      //printf("%s,%s\n",userid, passwd);
      //printf("%d,%d\n",strcmp(arr[0], userid), strcmp(arr[1], passwd) );
      if((strcmp(arr[0], userid) == 0) && (strcmp(arr[1], hashpass) == 10))
      {
        printf("Hello %s. Log in successful", arr[0]);
        fclose(fPtr);
        exit(EXIT_SUCCESS);
      }
    }

    if(fgets(line, BUFF_SIZE, fPtr) == NULL){
      printf("Incorrect Login Credentials\n");
    }
    /* Close file to save file data */
    fclose(fPtr);

}
