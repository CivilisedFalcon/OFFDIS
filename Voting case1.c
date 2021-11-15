#include<stdio.h>
void Vote(int choice);
int main()
{
  int n,stop;
  do{
   printf("SW capstone design project is fun?\n1)YES\n2)NO\n3)NONE\n");
   scanf("%d",&n);
   if(n>0&&n<=3)
   {
    Vote(n);
   }
   else{
    printf("Enter Valid input\n");
   }
  printf("Any number to continue else 0 to finish Voting and find who won the Vote\n");
  scanf("%d",&stop);
  }while(stop);
  Vote(stop);
}
void Vote(int choice)
{
 static int a,b,c;
 if(choice==1)
 {
  a++;
 }
 if(choice==2)
 {
  b++;
 }
 if(choice==3)
 {
  c++;
 }
 printf("YES = %d\n",a);
 printf("NO = %d\n",b);
 printf("NONE = %d\n",c);
 if(choice==0)
 {
  if(a>b&&a>c)
  {
   printf("YES won with votes of %d\n",a);
  }
  if(b>a&&b>c)
  {
   printf("NO won with votes of %d\n",b);
  }
  if(c>b&&c>a)
  {
   printf("NONE won with votes of %d\n",c);
  }
 }
}