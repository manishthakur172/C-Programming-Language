#include<stdio.h>
void print_array(const int *,int);
void bubble_sort(int *,int);
main()
{
	int ele;
	int a[5]={1000,200,30,400,50};
	ele=sizeof(a)/sizeof(a[0]);
	printf("before\n");
	print_array(a,ele);
	bubble_sort(a,ele);
	printf("after......\n");
	print_array(a,ele);
	printf("\n");
}
void print_array(const int *p,int n)
{
	int i;
	for(i=0;i<n;i++)
		printf("%d ",p[i]);
	printf("\n");
}
void bubble_sort(int *p,int ele)
{
	int i,j,t;
	for(i=0;i<ele-1;i++)
	{
		for(j=0;j<ele-1-i;j++)
		{
			if(p[j]>p[j+1])
			{
				t=p[j];
				p[j]=p[j+1];
				p[j+1]=t;
			}
		}
	}
}
