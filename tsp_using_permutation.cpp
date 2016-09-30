#include<stdio.h>

void swap(int *,int*);
void permute(void);
int noofpermutations();
void display(void);

int m,n,noofterms,count=0;
int a[100];
int minpath=9999;
int mat[10][10];

void swap(int *p1,int *p2)
{   int temp;
temp = *p1;
*p1 = *p2;
*p2 = temp;
}

int noofpermutations()
{   int permutations=1,x;
for(x=1;x<=noofterms-1;x++)
permutations=permutations*x;
return permutations;
}

void display()
{   int x=0;
	 int sum=0;
do
{
	sum=sum+mat[a[(x)%noofterms]][a[(x+1)%noofterms]];
	printf("%d",a[x]);
	x++;
}
while(sum<=minpath && x<noofterms);
if(x<noofterms || minpath<sum)
	{
	printf("\nnot best path\n");
	}
else
	{
	printf("\nsum is %d\n",sum);
	minpath=sum;
	}
count++;
}
void permute()
{   int y;
while(count<noofpermutations())
	{
	for(y=1;y<noofterms-1;y++)
		{
			swap(&a[y],&a[y+1]);
			display();
		}
	swap(&a[1],&a[2]);
	display();
	for(y=noofterms-1;y>1;y--)
		{
		swap(&a[y],&a[y-1]);
		display();
		}
	swap(&a[noofterms-1],&a[noofterms-2]);
	display();
	}
}

main()
{   int x,i,j;

printf("Enter no. of nodes : ");
scanf("%d",&noofterms);
//printf("Enter the terms : ");
for(x=0;x<noofterms;x++)
a[x]=x ;
//scanf("%d",&a[x]);
printf("\nenter the adjacency matrix :");
for(i=0;i<noofterms;i++)
	{
	 for(j=0;j<noofterms;j++)
	 {
	 scanf("%d",&mat[i][j]);
	 }
	}
printf("\nPaths are are : \n");
permute();
printf("\nmin length is%d",minpath);
return 0;
}


