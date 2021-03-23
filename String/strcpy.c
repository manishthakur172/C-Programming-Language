#include<stdio.h>
#include<string.h>
//void my_strcpy(char *,char *);
main()
{
char s[20],d[20],r;
printf("enter source string:\n");
scanf("%s",s);
r=strcpy(d,s);
printf("%s %s",s,d);
}
