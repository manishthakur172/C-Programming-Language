#include<stdio.h>
char *my_strcmp(const char *,const char *);
main()
{
char s[20],s1[20],*r;
printf("enter s:\n");
scanf("%s",s);
printf("enter s1:\n");
scanf("%s",s1);
r=my_strcmp(s,s1);
printf("r=%d\n",r);
}
char *my_strcmp(const char *s, const char *s1)
{
int i;
for(i=0;s[i];i++)
{
if(s[i]!=s1[i])
break;
}
if(s[i]==s1[i])
return 0;
else if (s[i]>s1[i])
return 1;
else
return -1;
}
