#include<stdio.h>
#include<string.h>
main()
{
const char s[20];
char i,*p;
printf("enter the string:\n");
scanf("%s",s);
printf("enter the character:\n");
scanf(" %c",&i);
p=strchr(s,i);
if(p!=0)
printf("present\n",p);
else

printf("not present\n");
}
