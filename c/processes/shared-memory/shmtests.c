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

typedef struct shared_memory_segment
{
  int lucky_number;
} sms_t;

sms_t *
shmem_create (char *name, size_t message_length)
{

  return sms;
}

sms_t *
shmem_open (char *name)
{
  int oflag = O_RDWR; // Open for read-write access.
  mode_t mode = 0;    // 00000 Nobody has permission to do nothing.
  int fd = shm_open (name, oflag, mode);
  if (fd == -1)
    {
      perror ("shm_open");
      return 1;
    }

  // Shared memory segment is 0 bytes due to shm_open.
  // Resize to size of shared_memory struct.
  off_t shared_memory_offset = sizeof (sms_t);
  if (ftruncate (fd, sizeof (sms_t)) == -1)
    {
      perror ("ftruncate");
      return 1;
    }

  // Map shared_memory into client's address space.
  int prot = PROT_READ     // Pages may be read.
             | PROT_WRITE; // Pages may be written.
  size_t shared_memory_size = sizeof (sms_t);
  sms_t *shmem = mmap (NULL, shared_memory_size, prot, MAP_SHARED, fd, 0);
  if (shmem == MAP_FAILED)
    {
      perror ("mmap");
      return 1;
    }

  return shmem;
}

int
shmem_destroy (char *name)
{
  sms_t *shmem = shmem_open (name);
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
  sms_t *shmem = shmem_open (SHARED_MEMORY_NAME);
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
  /*
   ****************************************************************************
   *  Create shared memory segment                                            *
   ****************************************************************************
   */

  const int oflag = O_CREAT   // Create if it does not exist.
                    | O_EXCL  // If already exists return error.
                    | O_RDWR; // Open for read-write access.

  mode_t mode = S_IRUSR    // 00400 User has read permission.
                | S_IWUSR; // 00200 User has write permission.

  int prot = PROT_READ     // Pages may be read.
             | PROT_WRITE; // Pages may be written.

  // Create shared memory segment.
  int fd = shm_open (SHARED_MEMORY_NAME, oflag, mode);
  if (fd == -1)
    {
      perror ("shm_open");
      return 1;
    }

  // Shared memory segment is 0 bytes due to shm_open.
  // Resize to size of shared_memory struct.
  off_t shared_memory_offset = sizeof (sms_t);
  if (ftruncate (fd, shared_memory_offset) == -1)
    {
      perror ("ftruncate");
      return 1;
    }

  // Map shared_memory into server's address space.
  size_t shared_memory_size = sizeof (sms_t);
  sms_t *sms = mmap (NULL, shared_memory_size, prot, MAP_SHARED, fd, 0);
  if (sms == MAP_FAILED)
    {
      perror ("mmap");
      return 1;
    }

  // Initialize shared memory segment members.
  sms->lucky_number = 1312;

  printf ("sms->lucky_number: %d\n", sms->lucky_number);

  /*
   ****************************************************************************
   *  Delete mappings of shared memory segment                                *
   ****************************************************************************
   */
  if (munmap (sms, shared_memory_size) == -1)
    {
      perror ("munmap");
      return 1;
    }

  printf ("sms->lucky_number: %d\n", sms->lucky_number);

  return 0;
}
