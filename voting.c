#include<stdio.h>
#pragma warning(disable:4996)
#define BUFFER_SIZE 256

void Vote(int choice);

int main() {
FILE* fp2;
char buffer[BUFFER_SIZE + 1];
if ((fp2 = fopen("vote.txt", "r+")) != NULL) {

memset(buffer, 0, sizeof(buffer));

while (fread(buffer, 1, BUFFER_SIZE, fp2) != 0)
printf("%s\n", buffer);

fclose(fp2);
}

int n, stop;

do {
printf("Do you like Pizza?\n1)YES\n2)NO\n3)NONE\n4)Reset");
scanf("%d", &n);
if (n > 0 && n <= 4)
{
Vote(n);
}
else {
printf("Enter Valid input\n");
}

printf("Any number to continue else 0 to finish Voting and find who won the Vote\n");
scanf("%d", &stop);

} while (stop);

Vote(stop);

}



void Vote(int choice)
{
FILE* fp = fopen("vote.txt", "r");
int a = 0, b = 0, c = 0;
if (!feof(fp))
{
fscanf(fp, "YES = %d\n",&a);
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
if (choice == 4)
{
  a=0;
  b=0;
  c=0;
}
fprintf(fp, "YES = %d\n", a);
fprintf(fp, "NO = %d\n", b);
fprintf(fp, "NONE = %d\n", c);

if (choice == 0)
{
if (a > b && a > c)
{
fprintf(fp, "YES won with votes of %d\n", a);
}
if (b > a && b > c)
{
fprintf(fp, "NO won with votes of %d\n", b);
}
if (c > b && c > a)
{
fprintf(fp, "NONE won with votes of %d\n", c);
}
}

fclose(fp);

}