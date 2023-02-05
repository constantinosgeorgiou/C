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

struct shared_memory
{
  sem_t mutex;

  int coffees;
  size_t acab;

  // > TODO(challenge): Allocated number.
  int *numbers;

  // > TODO(challenge): Static array
  // int number[5];
  // > TODO(challenge):Dynamic array
  // size_t message_length;
  // char *message;
  // > TODO(challenge): Static 2D array
  // > TODO(challenge): Dynamic 2D array
};

struct shared_memory *
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
  off_t shared_memory_offset = sizeof (struct shared_memory)     //
                               + (TOTAL_NUMBERS * sizeof (int)); //
  if (ftruncate (fd, shared_memory_offset) == -1)
    {
      perror ("ftruncate");
      return NULL;
    }

  // Map shared_memory into server's address space.
  size_t shared_memory_size = sizeof (struct shared_memory)     //
                              + (TOTAL_NUMBERS * sizeof (int)); //
  struct shared_memory *shmem = mmap (NULL, shared_memory_size, prot, MAP_SHARED, fd, 0);
  if (shmem == MAP_FAILED)
    {
      perror ("mmap");
      return NULL;
    }

  // Initialize shmem members.
  if (sem_init (&shmem->mutex, 1, 1) == -1)
    {
      perror ("sem_init-mutex");
      return NULL;
    }
  shmem->acab = 0;
  shmem->coffees = 0;
  for (int i = 0; i < TOTAL_NUMBERS; i++)
    {
      shmem->numbers[i] = i;
    }

  // *shmem->pointer = 10;
  // shmem->message_length = message_length;

  return shmem;
}

struct shared_memory *
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
  if (ftruncate (fd, sizeof (struct shared_memory)) == -1)
    {
      perror ("ftruncate");
      return NULL;
    }

  // Map shared_memory into client's address space.
  int prot = PROT_READ     // Pages may be read.
             | PROT_WRITE; // Pages may be written.
  struct shared_memory *shmem = mmap (NULL, sizeof (*shmem), prot, MAP_SHARED, fd, 0);
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
  struct shared_memory *shmem = shmem_open (name);
  if (shmem == NULL)
    {
      perror ("shmem_open");
      return -1;
    }

  // Destroy shmem members.
  if (sem_destroy (&shmem->mutex) == -1)
    {
      perror ("sem_destroy-mutex");
      return -1;
    }

  return shm_unlink (name);
}

int
child (void)
{
  printf ("[%d] Child: opening shared memory\n", getpid ());
  struct shared_memory *shmem = shmem_open (SHARED_MEMORY_NAME);
  if (shmem == NULL)
    {
      perror ("shmem_open");
      return -1;
    }

  printf ("[%d] Child: acquiring mutex\n", getpid ());
  if (sem_wait (&shmem->mutex) == -1)
    {
      perror ("sem_wait-mutex");
      return -1;
    }

  printf ("[%d] Client: coffees (%d), acab(%ld)\n", getpid (), shmem->coffees, shmem->acab);
  shmem->coffees = 3;
  shmem->acab = 1;
  printf ("[%d] Client: had %d coffees, acab: %ld\n", getpid (), shmem->coffees, shmem->acab);

  printf ("[%d] Child: releasing mutex\n", getpid ());
  if (sem_post (&shmem->mutex) == -1)
    {
      perror ("sem_post-mutex");
      return -1;
    }

  return 0;
}

int
main (int argc, char const *argv[])
{
  char temporary_msg[] = "Some funky shit";

  printf ("[%d] Parent: creating shared memory\n", getpid ());
  struct shared_memory *shmem = shmem_create (SHARED_MEMORY_NAME, sizeof (temporary_msg));
  if (shmem == NULL)
    {
      perror ("shmem_create");
      return 1;
    }

  printf ("Creating child\n");
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

  printf ("[%d] Parent: acquiring mutex\n", getpid ());
  if (sem_wait (&shmem->mutex) == -1)
    {
      perror ("sem_wait-mutex");
      return -1;
    }

  printf ("[%d] Parent: coffees (%d), acab(%ld)\n", getpid (), shmem->coffees, shmem->acab);
  shmem->coffees = 1000000;
  shmem->acab = 1312;
  printf ("[%d] Parent: had %d coffees, acab: %ld\n", getpid (), shmem->coffees, shmem->acab);

  printf ("[%d] Parent: releasing mutex\n", getpid ());
  if (sem_post (&shmem->mutex) == -1)
    {
      perror ("sem_post-mutex");
      return -1;
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
