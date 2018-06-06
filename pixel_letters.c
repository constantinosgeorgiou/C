#include <stdio.h>
#include <stdlib.h>

int main(void){ 
	int ch = getchar();
	while (ch != EOF){
		putchar(ch);
		ch = getchar();
	}
	putchar('\n');
}