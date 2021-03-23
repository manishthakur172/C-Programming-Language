#include<stdio.h>
main()
{
	int a[5],i,j,t,ele;
	ele=sizeof(a)/sizeof(a[1]);
	printf("enter the elements:\n");
	for(i=0;i<ele;i++)
		scanf("%d",&a[i]);

	printf("before elements are:\n");
	for(i=0;i<ele;i++)
		printf("%d ",a[i]);
	printf("\n");

	for(i=0;i<ele-1;i++)
	{
		for(j=i+1;j<ele;j++)
		{
			if(a[i]>a[j])
			{
				t=a[j];
				a[j]=a[i];
				a[i]=t;
			}
		}
	}
	printf("after elements are:\n");
	for(i=0;i<ele;i++)
		printf("%d ",a[i]);
	printf("\n");


}


