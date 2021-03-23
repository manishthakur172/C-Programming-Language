#include<stdio.h>
//#include<string.h>
void my_strlen(char *);
main()
{
char s[20];
printf("enter the string:\n");
scanf("%s",s);
my_strlen(s);
}
void my_strlen(char *s)
{
int i;
for(i=0;s[i];i++);
printf("size is=%d\n",i);
}
