#include "replace.h"


void replace(char ch[])
{
	for(int i = 0 ; i < strlen(ch); i++)
	{
		if(ch[i] == 'i')
		{
			ch[i] = 'x';
		}
		else if(ch[i] == 'I')
		{
			ch[i] = 'X';
		}
		printf("%c",ch[i]);
	}
}

