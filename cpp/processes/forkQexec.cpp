#include <sys/wait.h>
#include <unistd.h>

#include <cstdio>
#include <queue>
#include <string>

using namespace std;

int child(string msg) {
  printf("%s\n", msg.c_str());
  return 0;
  execlp("echo", "echo", msg.c_str(), static_cast<char *>(nullptr));
  perror("execlp");
  return -1;
}

int program() {
  queue<pid_t> children;

  pid_t cpid = fork();
  if (cpid == -1) {
    perror("fork");
    return 1;
  }

  if (cpid == 0) {
    if (child("Seems like I got it!") == -1) {
      perror("child-failed");
      return 1;
    }
  }

  printf("(%d) pushing %d to queue\n", getpid(), cpid);
  children.push(cpid);

  printf("Waiting for child %d\n", cpid);
  waitpid(cpid, 0, 0);

  return 0;
}

int main(int argc, char const *argv[]) {
  if (program() == -1) {
    perror("program");
    return -1;
  }

  return 0;
}
