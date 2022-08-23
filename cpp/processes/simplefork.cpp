#include <iostream>
#include <sys/wait.h>
#include <unistd.h>

int
main (int argc, char const *argv[])
{
  pid_t cpid = fork ();
  if (cpid == -1)
    {
      perror ("fork");
      exit (EXIT_FAILURE);
    }

  if (cpid > 0)
    {
      std::cout << "Hello from parent!" << std::endl;
      wait (nullptr);
    }

  if (cpid == 0)
    {
      std::cout << "Hello from child " << getpid () << std::endl;
      exit (EXIT_SUCCESS);
    }

  return EXIT_SUCCESS;
}
