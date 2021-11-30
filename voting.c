#define _CRT_SECURE_NOWARNINGS
#include<stdio.h>
#include<string.h>    // strcmp 함수가 선언된 헤더 파일
#define MAX 1000
#define BUFFER_SIZE 256

void Vote(int choice);

int main()
{
	char s1[100];
	char s2[100];
	char* s3 = "#vote";
    char s4[100];
    char* s5 = "#result";
    char buffer[MAX] = { 0, };
     
    FILE* fp = fopen("vote.txt", "r+");

	int n, stop;

	printf(" ");
	scanf("%s", s2);

	if (strcmp(s2,s3) == 0)
	{
		printf(" ");
	    scanf("%[^\n]s", s1);
	    printf("%s\n", s1);
    }
    do
    { 
        printf("1)YES\n2)NO\n3)NONE\n");
        scanf("%d", &n);
        if (n > 0 && n <= 3)
        {
            Vote(n);
        }
        else
        {
            printf("Enter Valid input\n");
        }
        printf("Any number to continue else 0 to finish Voting and find who won the Vote\n");
		scanf("%d", &stop);
    }while(stop);
    Vote(stop);

        printf(" ");
        scanf("%s", s4);
        if (strcmp(s4, s5) == 0)
        {
        fread(buffer, 1, MAX, fp); //전체 읽기
        printf("%s\n", buffer);

        fclose(fp);
        }
       
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
				fprintf(fp, "YES won with votes of '%d'\n", a);
			}
			if ((b > a) && (b > c))
			{
				fprintf(fp, "NO won with votes of '%d'\n", b);
			}
			if ((c > b) && (c > a))
			{
				fprintf(fp, "NONE won with votes of '%d'\n", c);
			}
            if (a == b)
            {
                fprintf(fp, "YES and NO Won Equal with votes of '%d' '%d'\n", a, b);
            }
            if (a == c)
            {
                fprintf(fp, "YES and NONE Won Equal with votes of '%d' '%d'\n", a, c);
            }
            if (b == c)
            {
                fprintf(fp, "NO and NONE Won Equal with votes of '%d' '%d'\n", b, c);
            }
           
        }
        fclose(fp);
	}
		
       