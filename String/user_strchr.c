#include<stdio.h>
char *my_strchr(const char *,char);
main()
{
//char r;
char s[10],ch,*p;
printf("enter a string:\n");
scanf("%s",s);
printf("enter a character:\n");
scanf(" %c",&ch);
//printf("JHYuy\n");
p=my_strchr(s,ch);
if(p==0)
printf("not present\n");
else
printf("present\n",p);
}
char *my_strchr(const char *p, char ch)
{
int i;
char *q=0;
for(i=0;p[i];i++)
{
if(p[i]==(ch))
q=p+i;
}
return q;
}
/*{
int i;
char *q=0;
for(i=0;p[i];i++);
for(i=i-1;i>=0;i--)
{
if(p[i]==ch)
return p+i;
}
return 0;
}*/
