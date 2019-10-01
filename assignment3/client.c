//Beryn Staub-Waldenberg Assginment 3 cs247 spring2019 - I received skeleton code from my instructor Susheel Gopalan

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include "shm.h"

void reterror(char* s);


/*
returns -1 if shm_open() fails
returns -2 if mmap() fails
returns -3 if munmap() fails
returns 0 if successful
*/
int main(int argc, char* argv[])
{
  int retVal = 0;


  //<Use the POSIX "shm_open" API to open file descriptor with "O_RDWR" options and the "0666" permissions>
  const char* name = "memobj";
  int shmfd = shm_open(name, O_RDWR, 0666);
  if(shmfd == -1)
	{
    reterror("shm_open()");
    close(shmfd);
    shm_unlink(name);
    return -1;
	}

  //<Use the "mmap" API to memory map the file descriptor>
  ShmData* shmPtr;
  const int size = sizeof(ShmData);
	shmPtr = (ShmData*) mmap(NULL, size, PROT_WRITE | PROT_READ, MAP_SHARED, shmfd, 0);
	if(shmPtr == MAP_FAILED)
	{
		reterror("mmap()");
    close(shmfd);
    shm_unlink(name);
    return -2;
	}


  printf("[Client]: Waiting for valid data ...\n");

  while(shmPtr->status != VALID)
    {
      sleep(1);
    }

  printf("[Client]: Received %d\n",shmPtr->data);

  shmPtr->status = CONSUMED;

   //<use the "munmap" API to unmap the pointer>
   if(munmap(shmPtr, size) == -1)
 	{
 		reterror("munmap()");
    retVal = -3;
 	}

  printf("[Client]: Client exiting...\n");

  return retVal;

}

void reterror(char* s)
{
	printf("[Client]: ");
	perror(s);
}
