#define _CRT_SECURE_NOWARNINGS
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#define MAX 1000
#define BUFFER_SIZE 256

void Vote(int choice);

int main()
{
    char s1[100];
    char s2[100];
    char* s3 = "#vote";
    char s4[100];
    char* s5 = "#stop";
    char *s6 = "";
    char *s7 = " ";
    char *pnt = strstr(s1, s2);
    char buffer[MAX] = { 0, };
    FILE* fp = fopen("vote.txt", "r+");
    int n, stop;
    
    printf("");
    scanf("%s", s2);


    if (strncmp(s2, s3, 5) == 0)
    {
        
        printf("");
       
        scanf("%[^\n]s", s1);
        if(strcmp(s1,s6) == 0)
        {
            printf("Please enter the voting type.\n");
            return 0;
        }
        if(strcmp(s1,s7) == 0)
        {
            printf("Please enter the voting type.\n");
            return 0;
        }
        printf("%s\n", s1);
        printf("1) YES\n2) NO\n3) NONE\n");
        printf("");
        scanf("%d", &n);
        if (n > 0 && n <= 3)
        {
            Vote(n);
        }
        else
        {
            printf("Enter Valid input\n");
        }
        Vote(stop);

    }
    else
    {
        printf("Vote Execution Error\n");
        return 0;
    }
  

    printf("If you wait for 5 seconds, you can check the results\n");
    sleep(5);
    
        fread(buffer, 1, MAX, fp); //전체 읽기
        printf("%s", buffer);

        fclose(fp);
    
    return 0;
}

void Vote(int choice)
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
    if (choice == 0)
    {
        if ((a > b) && (a > c))
        {
            fprintf(fp, "YES won with votes of '%d'", a);
        }
        else if ((b > a) && (b > c))
        {
            fprintf(fp, "NO won with votes of '%d'", b);
        }
        else if ((c > b) && (c > a))
        {
            fprintf(fp, "NONE won with votes of '%d'", c);
        }
        else if ((a == b) && (b == c))
        {
            fprintf(fp, "Everything Equal with votes of '%d'", a);
        }
        else if (a == b)
        {
            fprintf(fp, "YES and NO Won Equal with votes of '%d' '%d'", a, b);
        }
        else if (a == c)
        {
            fprintf(fp, "YES and NONE Won Equal with votes of '%d' '%d'", a, c);
        }
        else if (b == c)
        {
            fprintf(fp, "NO and NONE Won Equal with votes of '%d' '%d'", b, c);
        }
    }
    fclose(fp);
}