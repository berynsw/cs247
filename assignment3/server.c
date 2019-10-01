//Beryn Staub-Waldenberg Assginment 3 cs247 spring2019 - I received skeleton code from my instructor Susheel Gopalan

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include "shm.h"


void reterror(char* s);

/*
returns -1 if wrong number of arguments is entered
returns -2 if shm_open() fails
returns -3 if ftruncate() fails
returns -4 if mmap() fails
returns -5 if munmap() fails
returns -6 if close() fails
returns -7 if shm_unlink() fails
returns 0 if successful
*/

int main(int argc, char* argv[])
{
	int retVal = 0;

  //<Confirm argc is 2 and if not print a usage string.>
	if(argc != 2)
	{
		printf("[Server]: arg error, usage: one integer argument\n");
		retVal = -1;
	}

  //<Use the POSIX "shm_open" API to open file descriptor with "O_CREAT | O_RDWR" options and the "0666" permissions>
	//name for shared memory object
	const char* name = "memobj";
	int shmfd;
	shmfd = shm_open(name, O_CREAT | O_RDWR, 0666);
	if(shmfd == -1)
	{
		reterror("shm_open");
		close(shmfd);
		shm_unlink(name);
		return -2;

	}

  //<Use the "ftruncate" API to set the size to the size of your structure shm.h>
	const int size = sizeof(ShmData);
	retVal = ftruncate(shmfd, size);
	if(retVal == -1)
	{
		reterror("ftruncate()");
		close(shmfd);
		shm_unlink(name);
		retVal = -3;
	}

  //<Use the "mmap" API to memory map the file descriptor>
	ShmData* shmPtr;

	shmPtr = (ShmData*) mmap(NULL, size, PROT_WRITE | PROT_READ, MAP_SHARED, shmfd, 0);
	if(shmPtr == MAP_FAILED)
	{
		reterror("mmap()");
		close(shmfd);
		shm_unlink(name);
		return -4;
	}


  //<Set the "status" field to INVALID>
	shmPtr->status = INVALID;
  //<Set the "data" field to atoi(argv[1])>
	shmPtr->data = atoi(argv[1]);
  //<Set the "status" field to VALID>
	shmPtr->status = VALID;


  printf("[Server]: Server data Valid... waiting for client\n");

  while(shmPtr->status != CONSUMED)
    {
      sleep(1);
    }

  printf("[Server]: Server Data consumed!\n");

  //<use the "munmap" API to unmap the pointer>
	if(munmap(shmPtr, size) == -1)
	{
		reterror("munmap()");
		retVal = -5;
	}

  //<use the "close" API to close the file Descriptor>
	if(close(shmfd) == -1)
	{
		reterror("close()");
		retVal = -6;
	}

  //<use the "shm_unlink" API to revert the shm_open call above>
	if(shm_unlink(name) == -1)
	{
		reterror("shm_unlink()");
		retVal = -7;
	}

  printf("[Server]: Server exiting...\n");


  return retVal;

}

void reterror(char* s)
{
	printf("[Server]: ");
	perror(s);

}
