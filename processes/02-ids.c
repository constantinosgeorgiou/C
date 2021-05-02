#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char const *argv[]) {
    int id = fork();
    if (id == 0) {
        sleep(1);
    }

    printf("Current id: %d    Parent id: %d\n", getpid(), getppid());

    int result = wait(NULL);
    if (result == -1) {
        printf("No children to wait for.\n");
    } else {
        printf("%d finished execution\n", result);
    }
    return 0;
}
