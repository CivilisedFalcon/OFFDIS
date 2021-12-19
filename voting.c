#define _CRT_SECURE_NOWARNINGS
#include<stdio.h>
#include<string.h>    // strcmp 함수가 선언된 헤더 파일
#include<unistd.h>

#define MAX 1000
#define BUFFER_SIZE 256

void Vote_data(int choice);
void Vote();
void result_vote();

int main()
{
    Vote();
    result_vote();
}

void Vote()
{
    char s1[100]; // Question
    char s2[100]; // #vote
    char* s3 = "#vote"; // Comparing with #vote
    char *s6 = ""; // Search for blanks in the questions.
    char *s7 = " "; // Search for blanks in the questions.
    
    int n, stop;
    
    printf(""); // Enter the #vote
    scanf("%s", s2);


    if (strncmp(s2, s3, 5) == 0)
    {
        
        printf("");  // Enter the Question
       
        scanf("%[^\n]s", s1);
        if(strcmp(s1,s6) == 0) // If there's a blank, it's an error.
        {
            printf("Please enter the voting type.\n");
            return 0;
        }
        if(strcmp(s1,s7) == 0) // If there's a blank, it's an error.
        {
            printf("Please enter the voting type.\n");
            return 0;
        }
        printf("%s\n", s1); // Question output.
        printf("1) YES\n2) NO\n3) NONE\n");
        printf(""); // input number
        scanf("%d", &n);
        if (n > 0 && n <= 3)
        {
            Vote_data(n);
        }
        else
        {
            printf("Enter Valid input\n");
        }
        Vote_data(stop);

    }
    else // If it's not #vote, error print out.
    {
        printf("Vote Execution Error\n");
        return 0;
    }
             
}
void Vote_data(int choice)
{
    int a = 0, b = 0, c = 0;
    FILE* fp = fopen("vote.txt", "r"); 
    if (fp == NULL)
    {
        fp = fopen("vote.txt", "w");
    }
    if (!feof(fp))
    {
        fscanf(fp, "YES = %d\n", &a);
        fscanf(fp, "NO = %d\n", &b);
        fscanf(fp, "NONE = %d\n", &c);
    }
    fp = fopen("vote.txt", "w");
    if (choice == 1)
    {
        a++;
    }
    if (choice == 2)
    {
        b++;
    }
    if (choice == 3)
    {
        c++;
    }
    fprintf(fp, "YES = %d\n", a);
    fprintf(fp, "NO = %d\n", b);
    fprintf(fp, "NONE = %d\n", c);
        if ((a > b) && (a > c))
        {
            fprintf(fp, "YES won with votes of '%d'\n", a);
        }
        else if ((b > a) && (b > c))
        {
            fprintf(fp, "NO won with votes of '%d'\n", b);
        }
        else if ((c > b) && (c > a))
        {
            fprintf(fp, "NONE won with votes of '%d'\n", c);
        }
        else if ((a == b) && (b == c))
        {
            fprintf(fp, "Everything Equal with votes of '%d'\n", a);
        }
        else if (a == b)
        {
            fprintf(fp, "YES and NO Won Equal with votes of '%d' '%d'\n", a, b);
        }
        else if (a == c)
        {
            fprintf(fp, "YES and NONE Won Equal with votes of '%d' '%d'\n", a, c);
        }
        else if (b == c)
        {
            fprintf(fp, "NO and NONE Won Equal with votes of '%d' '%d'\n", b, c);
        }
        fclose(fp);
    }

    void result_vote()
    {
    char s4[100]; // #stop
    char* s5 = "#stop"; // Comparing with #stop
    char buffer[MAX] = { 0, }; // read vote.txt buf
    FILE* fp = fopen("vote.txt", "r+"); // read 'vote.txt' file

         printf("Enter '#stop' to check the results\n");
    printf("");
    scanf("%s", s4);
    if (strcmp(s4, s5) == 0)
    {
        fread(buffer, 1, MAX, fp); //전체 읽기
        printf("%s", buffer);

        fclose(fp);
    }
    return 0;
    }