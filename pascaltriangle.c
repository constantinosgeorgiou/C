#include <stdio.h>

void ptriangle(int rows);
int binomial(int number, int choose);
int factorial(int number);

int main(void){
	int rows;

	printf("Rows of the triangle: ");
	scanf("%d", &rows);
	ptriangle(rows);

	return 0;
}

void ptriangle(int rows){
	int number,spaces,choose;
	for(number=0; number <= rows; number++){
		for(spaces = rows; spaces >= number; spaces--){
			printf(" ");
		}
		for(choose = 0; choose <= number; choose++){
		printf("%d ",binomial(number,choose));
		}
		printf("\n");
	}
}

int binomial(int number, int choose){
	if (number == 0)
		return 1;
	else
		return factorial(number)/(factorial(choose)*factorial(number-choose));
}

int factorial(int number){
  if (number == 0)
	return 1;
  else
	return(number * factorial(number-1));
}