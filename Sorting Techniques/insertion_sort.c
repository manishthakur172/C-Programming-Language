#include<stdio.h>
main()
{
	int a[5],ele,i,j,key;
	printf("enter the 5 elements:\n");
	ele=sizeof(a)/sizeof(a[0]);
	for(i=0;i<ele;i++)
		scanf("%d",&a[i]);
	for(i=0;i<ele;i++)
		printf("%d ",a[i]);
	printf("\n");
	for(i=1;i<ele;i++)
	{
		key=a[i];
		for(j=i-1;j>=0&&a[j]>key;j--)
			a[j+1]=a[j];
		a[j+1]=key;
	}
	printf("after insertion sort.....\n");
	for(i=0;i<ele;i++)
		printf("%d ",a[i]);
	printf("\n");
}
