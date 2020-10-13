#include <stdio.h>
#include <string.h>

int funcBreak() 
{
	int n = 100;
	int a[n], b[n], c[n];
	int y = 0;	

	for(int x = 0; x < n; x++)
		b[x] = c[x] = x;

	while(y < 100)
	{
		a[y] = b[y] + c[y];

		if(a[y] > 15) 
			break;

		++y;
	}

	return 0;
}

int main(){

  funcBreak();


  return 0;
}
