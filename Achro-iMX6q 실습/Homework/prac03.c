#include "replace.h"
#include <string.h>
extern void replace();
int main()
{
	char str[50];
	scanf("%[^\n]s",str);

	replace(str);
	printf("\n");
return 0;
}
