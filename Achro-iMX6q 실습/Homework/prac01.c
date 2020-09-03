#include <stdio.h>

int main()
{
	int num,sum = 0, avg = 0;
	for(int i = 0 ;i < 5; i++)
	{
		scanf("%d",&num);
		sum += num;
	}
	printf("sum = %d   avg = %.3lf\n",sum,(double)sum/5);

return 0;
}
