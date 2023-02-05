#include <fcntl.h> // O_CREAT, O_EXCL, O_RDWR
#include <math.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdio.h> // perror()
#include <stdlib.h>
#include <sys/mman.h> // MAP_FAILED, shm_open(), mmap()
#include <sys/stat.h> // mode_t, S_IRUSR, S_IWUSR
#include <sys/wait.h>
#include <unistd.h>
#include <unistd.h> // ftruncate()

#define SHARED_MEMORY_NAME "/shared-memory-experiment"

#define TOTAL_NUMBERS 5

typedef struct shared_number
{
  int number;
} smn_t;

smn_t *
shmem_create (char *name, size_t message_length)
{
  const int oflag = O_CREAT   // Create if it does not exist.
                    | O_EXCL  // If already exists return error.
                    | O_RDWR; // Open for read-write access.

  mode_t mode = S_IRUSR    // 00400 User has read permission.
                | S_IWUSR; // 00200 User has write permission.

  int prot = PROT_READ     // Pages may be read.
             | PROT_WRITE; // Pages may be written.

  // Create shared memory segment.
  int fd = shm_open (name, oflag, mode);
  if (fd == -1)
    {
      perror ("shm_open");
      return NULL;
    }

  // Shared memory segment is 0 bytes due to shm_open.
  // Resize to size of shared_memory struct.
  off_t shared_memory_offset = TOTAL_NUMBERS * sizeof (smn_t);
  if (ftruncate (fd, shared_memory_offset) == -1)
    {
      perror ("ftruncate");
      return NULL;
    }

  // Map shared_memory into server's address space.
  size_t shared_memory_size = TOTAL_NUMBERS * sizeof (smn_t);
  smn_t *numbers = mmap (NULL, shared_memory_size, prot, MAP_SHARED, fd, 0);
  if (numbers == MAP_FAILED)
    {
      perror ("mmap");
      return NULL;
    }

  // Initialize numbers members.
  for (int i = 0; i < TOTAL_NUMBERS; i++)
    {
      numbers[i].number = i;
    }

  return numbers;
}

smn_t *
shmem_open (char *name)
{
  int oflag = O_RDWR; // Open for read-write access.
  mode_t mode = 0;    // 00000 Nobody has permission to do nothing.
  int fd = shm_open (name, oflag, mode);
  if (fd == -1)
    {
      perror ("shm_open");
      return NULL;
    }

  // Shared memory segment is 0 bytes due to shm_open.
  // Resize to size of shared_memory struct.
  off_t shared_memory_offset = TOTAL_NUMBERS * sizeof (smn_t);
  if (ftruncate (fd, sizeof (smn_t)) == -1)
    {
      perror ("ftruncate");
      return NULL;
    }

  // Map shared_memory into client's address space.
  int prot = PROT_READ     // Pages may be read.
             | PROT_WRITE; // Pages may be written.
  size_t shared_memory_size = TOTAL_NUMBERS * sizeof (smn_t);
  smn_t *shmem = mmap (NULL, shared_memory_size, prot, MAP_SHARED, fd, 0);
  if (shmem == MAP_FAILED)
    {
      perror ("mmap");
      return NULL;
    }

  return shmem;
}

int
shmem_destroy (char *name)
{
  smn_t *shmem = shmem_open (name);
  if (shmem == NULL)
    {
      perror ("shmem_open");
      return -1;
    }

  return shm_unlink (name);
}

int
child (void)
{
  printf ("[%d] Child: opening shared memory\n", getpid ());
  smn_t *shmem = shmem_open (SHARED_MEMORY_NAME);
  if (shmem == NULL)
    {
      perror ("shmem_open");
      return -1;
    }

  printf ("[%d] Child: reading shared memory\n", getpid ());
  for (int i = 0; i < TOTAL_NUMBERS; i++)
    {
      printf ("%d%s", shmem[i].number, i < TOTAL_NUMBERS - 1 ? ", " : "\n");
    }

  return 0;
}

int
main (int argc, char const *argv[])
{
  char temporary_msg[] = "Some funky shit";

  printf ("[%d] Parent: creating shared memory\n", getpid ());
  smn_t *shmem = shmem_create (SHARED_MEMORY_NAME, sizeof (temporary_msg));
  if (shmem == NULL)
    {
      perror ("shmem_create");
      return 1;
    }

  printf ("[%d] Parent: printing shared memory\n", getpid ());
  for (int i = 0; i < TOTAL_NUMBERS; i++)
    {
      printf ("%d%s", shmem[i].number, i < TOTAL_NUMBERS - 1 ? ", " : "\n");
    }

  printf ("[%d] Parent: creating child!\n", getpid ());
  pid_t cpid = fork ();
  if (cpid == -1)
    {
      perror ("fork");
      return 1;
    }

  if (cpid == 0)
    {
      return abs (child ());
    }

  printf ("Waiting for child %d\n", cpid);
  waitpid (cpid, 0, 0);

  printf ("[%d] Parent: unlinking shared memory\n", getpid ());
  if (shmem_destroy (SHARED_MEMORY_NAME) == -1)
    {
      perror ("shmem_destroy");
      return 1;
    }
  return 0;
}
