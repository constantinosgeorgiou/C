#include <stdio.h>
#include <unistd.h>

int main(int argc, char const *argv[]) {
    int id = fork();
    printf("ID: %d\n", id);

    if (id == 0) {
        printf("Child process.\n");
    } else {
        printf("Main process.\n");
    }

    return 0;
}
