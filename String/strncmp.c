#include<stdio.h>
#include<string.h>
main()
{
char s[20],s1[20],*l;
printf("enter source string:\n");
scanf("%s",s);
printf("enter other string:\n");
scanf("%s",s1);
l=strncmp(s,s1,3);
printf("%d\n",l);
} 

