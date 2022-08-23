#include <atomic>
#include <filesystem>
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::filesystem::exists;

constexpr std::atomic<int> FORK_NUM = 6;
constexpr std::atomic<int> handler_exit_code = 103;

std::atomic<int> child_pid;
std::atomic<int> *children;

void
sigquitHandler (int signal_number)
{
  for (int i = 0; i < FORK_NUM; i++)
    {
      kill (children[i], SIGTERM);
    }
  while ((child_pid = wait (nullptr)) > 0)
    {
      _exit (handler_exit_code);
    }
}

pid_t
spawnChild (const char *program, char **arg_list)
{
  pid_t ch_pid = fork ();
  if (ch_pid == -1)
    {
      perror ("fork");
      exit (EXIT_FAILURE);
    }

  if (ch_pid == 0)
    {
      execve (program, arg_list, nullptr);
      perror ("execve");
      exit (EXIT_FAILURE);
    }

  cout << "spawn child with pid - " << ch_pid << endl;
  return ch_pid;
}

int
main (int argc, char const *argv[])
{
  string program_name ("child");
  char *arg_list[] = { program_name.data (), nullptr };

  if (!exists (program_name))
    {
      cout << "Program file 'child' does not exist in current directory!\n";
      exit (EXIT_FAILURE);
    }

  children = reinterpret_cast<std::atomic<int> *> (new int[FORK_NUM]);
  signal (SIGQUIT, sigquitHandler);

  for (int i = 0; i < FORK_NUM; ++i)
    {
      children[i] = spawnChild (program_name.c_str (), arg_list);
    }
  cout << endl;

  while ((child_pid = wait (nullptr)) > 0)
    {
      cout << "child " << child_pid << " terminated" << endl;
    }

  return EXIT_SUCCESS;
}
