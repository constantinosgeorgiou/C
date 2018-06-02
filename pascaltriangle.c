#include <stdio.h>

int factorial(int number){
  if (number == 0)
    return 1;
  else
    return(number * factorial(number-1));
}

int binomial(int number, int choose){
    if (number == 0)
        return 1;
    else
        return factorial(number)/(factorial(choose)*factorial(number-choose));
}

void ptriangle(int number){
    int i,spaces,choose;
    for(i=0; i <= number; i++){
        for(spaces = number; spaces >= i; spaces--){
            printf(" ");
        }
        for(choose = 0; choose <= i; choose++){
        printf("%d ",binomial(i,choose));
        }
        printf("\n");
    }
}

int main(void){
    int number,choose;
    printf("Give me a number: ");
    scanf("%d", &number);
    ptriangle(number);

    return 0;
}
