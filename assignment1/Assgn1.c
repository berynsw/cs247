/*
I have recieved skeleton code from my Professor Susheel Gopalan and wrote the internals myself.
Beryn Staub-Waldenberg cs247 spring2019
*/


//========================================================================================================================================================

#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <inttypes.h>

//========================================================================================================================================================
//#defines

#define MAX_THREAD_COUNT 9
#define MAX_TASK_COUNT 3


//========================================================================================================================================================
//Typedefs
typedef struct{

	int threadCount;
	pthread_t threadId;
	int threadPolicy;
	int threadPri;
	long processTime;
	int64_t timeStamp[MAX_TASK_COUNT+1];
	time_t startTime;
	time_t endTime;
	time_t totalTime;
} ThreadArgs;

//Globals

//Try to change this to use a single condition variable
//pthread_mutex_t mutexarr[MAX_THREAD_COUNT];
//pthread_cond_t condarr[MAX_THREAD_COUNT];
int waitset = 1;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
//pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;
ThreadArgs g_ThreadArgs[MAX_THREAD_COUNT];

//========================================================================================================================================================


//========================================================================================================================================================
void DisplayThreadSchdAttributes( pthread_t threadID, int policy, int priority )
{

	long id = (long)threadID;
	printf("\nDisplayThreadSchdAttributes:\n threadID = 0x%lx\n policy = %s\n priority = %d\n",
									id,
									(policy == SCHED_FIFO) ? "SCHED_FIFO" :
									(policy == SCHED_RR)	? "SCHED_RR" :
									(policy == SCHED_OTHER) ? "SCHED_OTHER" :
									"???",
									priority);


}

//========================================================================================================================================================
void DisplayThreadArgs(ThreadArgs* myThreadArg){


	if( myThreadArg )
	{
		DisplayThreadSchdAttributes(myThreadArg->threadId, myThreadArg->threadPolicy, myThreadArg->threadPri);
		printf(" startTime = %s", ctime(&myThreadArg->startTime));
		printf(" endTime   = %s", ctime(&myThreadArg->endTime));
		printf(" totalTime = %s", ctime(&myThreadArg->totalTime));

		printf(" task start TimeStamp [%"PRId64"]\n", myThreadArg->timeStamp[0] );

		int y;
		for(y=1; y<MAX_TASK_COUNT+1; y++)
		{

			printf(" TimeStamp            [%"PRId64"] difference:[%"PRId64"]microseconds\n", myThreadArg->timeStamp[y],
			(myThreadArg->timeStamp[y] - myThreadArg->timeStamp[y-1]));
		}



	}
}

//========================================================================================================================================================
void DoProcess(void)
{
	unsigned int longVar =1 ;

	while(longVar < 0x00ffff) longVar++;
}

//========================================================================================================================================================

//sets schedule for each thread, mutex lock, wait condition, finally does tasks and records time it took
void* threadFunction(void *arg)
{
	ThreadArgs* myThreadArg;
  myThreadArg = (ThreadArgs*)arg;

	//priority
	struct sched_param param1;
  param1.sched_priority = myThreadArg->threadPri;

	//set the schedule for each thread
	int error = pthread_setschedparam( myThreadArg->threadId, myThreadArg->threadPolicy, &param1 );
	if(error){
			fprintf(stderr,"pthread_setschedparam() error, code: %d\n", error);
			//perror("error message");
	}


	//lock mutex and signal wait
	//int i = myThreadArg->threadCount
	//mutexarr[i] = PTHREAD_MUTEX_INITIALIZER;
	error = pthread_mutex_lock( &mutex1 );
	if(error)
		fprintf(stderr,"pthread_mutex_lock() error, code: %i\n", error);

//printf("got passed lock\n" );
	//condarr[i] = PTHREAD_COND_INITIALIZER;
	while(waitset){
		error = pthread_cond_wait( &cond1, &mutex1 );
		if(error)
			fprintf(stderr,"pthread_cond_wait() error, code: %i\n", error);
	}

	error = pthread_mutex_unlock( &mutex1 );
	if(error)
		fprintf(stderr,"pthread_mutex_unlock() error, code: %i\n", error);



	//record start time
	myThreadArg->startTime = time(NULL);

	int y = 0;

	struct timespec start;
	//timestamp before running processes
	clock_gettime(CLOCK_REALTIME, &start);
	myThreadArg->timeStamp[y] = start.tv_sec *1000000;

	//run task a few times
	for(y = 1; y < MAX_TASK_COUNT+1; y++){
		//record start time of each task
		clock_gettime(CLOCK_REALTIME, &start);
		myThreadArg->timeStamp[y] = start.tv_sec *1000000;
		myThreadArg->timeStamp[y] += start.tv_nsec/1000;
		if(start.tv_nsec % 1000 >= 500 )
			myThreadArg->timeStamp[y]++;

		//does a thing
		DoProcess();
	}

	//record time after doing tasks
	myThreadArg->endTime = time(NULL);
	myThreadArg->totalTime = (myThreadArg->endTime - myThreadArg->startTime);


}





/*
	2.	Use the �pthread_setscheduleparam� API to set the thread policy
	3.	Init the Condition variable and associated mutex
	4.	Wait on condition variable

	5.	Once condition variable is signaled, use the �time� function and the �clock_gettime(CLOCK_REALTIME, &tms)� to get timestamp
	6.	Call �DoProcess� to run your task
	7.	Use �time� and �clock_gettime� to find end time.
	8.	You can repeat steps 6 and 7 a few times if you wish

	*/


//========================================================================================================================================================
int main (int argc, char *argv[])
{



/*
	//init arg values for 9 threads, create threads with those values
	for(int i = 1; i <= MAX_THREAD_COUNT; i++){


		g_ThreadArgs[i].threadCount = i;

		if(i <= 3){
			g_ThreadArgs[i].threadPolicy = SCHED_RR;
			g_ThreadArgs[i].threadPri = i;
		}
		else if(i <= 6){
			g_ThreadArgs[i].threadPolicy = SCHED_FIFO;
			g_ThreadArgs[i].threadPri = i;
		}
		else{
			g_ThreadArgs[i].threadPolicy = SCHED_OTHER;
			g_ThreadArgs[i].threadPri = 0;
		}

		int error = pthread_create(&(g_ThreadArgs[i].threadId), NULL, &threadFunction, &g_ThreadArgs[i]);
		if(error)
			fprintf(stderr,"pthread_create() error, code: %i\n", error);
	}
	*/

	g_ThreadArgs[6].threadCount = 1;
	g_ThreadArgs[6].threadPolicy = SCHED_RR;
	g_ThreadArgs[6].threadPri = 6;
	pthread_create(&(g_ThreadArgs[1].threadId), NULL, &threadFunction, &g_ThreadArgs[1]);

	g_ThreadArgs[6].threadCount = 6;
	g_ThreadArgs[6].threadPolicy = SCHED_FIFO;
	g_ThreadArgs[6].threadPri = 6;
	pthread_create(&(g_ThreadArgs[6].threadId), NULL, &threadFunction, &g_ThreadArgs[6]);

	g_ThreadArgs[7].threadCount = 7;
	g_ThreadArgs[7].threadPolicy = SCHED_OTHER;
	g_ThreadArgs[6].threadPri = 0;
	pthread_create(&(g_ThreadArgs[7].threadId), NULL, &threadFunction, &g_ThreadArgs[7]);


	//signal all threads to wakeup
	pthread_mutex_lock( &mutex1 );
	waitset = 0;
	int error =	pthread_cond_broadcast( &cond1 );
	if(error)
		fprintf(stderr,"pthread_cond_broadcast() error, code: %i\n", error);
	pthread_mutex_unlock( &mutex1 );

/*
	//join all threads, preventing main from finishing until other threads have
	for(int i = 1; i <= 9; i++){

		//condition signal
		//int j = g_ThreadArgs[i].threadCount;

		int error = pthread_join(g_ThreadArgs[i].threadId, NULL);
		if(error)
			fprintf(stderr,"pthread_join() error, code: %i\n", error);
		//printf("thread%i joined\n", i);
	}

	for(int i = 1; i <= 9; i++){
		//display thread stats
		DisplayThreadArgs( &g_ThreadArgs[i] );
	}
*/

pthread_join(g_ThreadArgs[1].threadId, NULL);
pthread_join(g_ThreadArgs[6].threadId, NULL);
pthread_join(g_ThreadArgs[7].threadId, NULL);


DisplayThreadArgs( &g_ThreadArgs[1] );
DisplayThreadArgs( &g_ThreadArgs[6] );
DisplayThreadArgs( &g_ThreadArgs[7] );



	printf("exiting main function\n");
	exit(EXIT_SUCCESS);

}

/*
	1.	Call InitGlobals
	2.	Create a number of threads (start with 1 and increase to 9) using �pthread_Create�
	3.	Assign 3 threads to SCHED_OTHER, another 3 to SCHED_FIFO and another 3 to SCHED_RR
	4.	Signal the condition variable
	5.	Call �pthread_join� to wait on the thread
	6.	Display the stats on the threads
	*/





//========================================================================================================================================================




/*

************* HINTS ******************

========================================================================================================================================================
Every time you run into issues with usage of an API, please look up samples on how that API is used here...

http://www.yolinux.com/TUTORIALS/LinuxTutorialPosixThreads.html

========================================================================================================================================================


Please check the return values from all system calls and print an error message in all errclock_gettime(CLOCK_REALTIME, &tms);or cases including the error code.. That will help catch errors quickly.
========================================================================================================================================================


You can use the following technique to pass the address of the element corresponding to a particular thread to the thread function...

	void* threadFunction(void *arg)
	{
		ThreadArgs*	myThreadArg;

		myThreadArg = (ThreadArgs*)arg;

	}


	int main (int argc, char *argv[])
	{

		while(threadCount < MAX_THREAD_COUNT)
		{
		...
			if( pthread_create(&(g_ThreadArgs[threadCount].threadId), &threadAttrib, &threadFunction, &g_ThreadArgs[threadCount]) )
		...

		}
	}
========================================================================================================================================================

Here is the usage for clock_gettime�

	clock_gettime(CLOCK_REALTIME, &tms);
	myThreadArg->timeStamp[y+1] = tms.tv_sec *1000000;
	myThreadArg->timeStamp[y+1] += tms.tv_nsec/1000;
	if(tms.tv_nsec % 1000 >= 500 ) myThreadArg->timeStamp[y+1]++;

========================================================================================================================================================

Here is how you wait on a condition event�

	pthread_mutex_lock ( &g_ThreadMutex[myThreadArg->threadCount] );
	pthread_cond_wait ( &g_conditionVar[myThreadArg->threadCount], &g_ThreadMutex[myThreadArg->threadCount] );
	pthread_mutex_unlock( &g_ThreadMutex[myThreadArg->threadCount] );

========================================================================================================================================================

Note that this sample is changing the policy of the current thread... so if you follow this sample, make sure you are making the call from the thread function.


	http://man7.org/linux/man-pages/man3/pthread_setschedparam.3.html

	if (main_sched_str != NULL) {
	if (!get_policy(main_sched_str[0], &policy))
		usage(argv[0], "Bad policy for main thread (-m)\n");
		param.sched_priority = strtol(&main_sched_str[1], NULL, 0);

	s = pthread_setschedparam(pthread_self(), policy, &param);
	if (s != 0)
		handle_error_en(s, "pthread_setschedparam");
	}

========================================================================================================================================================
For those confused about my comment on trying to using a single Condition variable instead of an array... please read the following...

http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_cond_signal.html

You can use the broadcast API to wake multiple threads waiting on the same condition variable.

For those who really like to go deeper, know that you have access to the code for most of the Linux system APIs... here is the code pthread_cond_broadcast...

https://code.woboq.org/userspace/glibc/nptl/pthread_cond_broadcast.c.html

========================================================================================================================================================
*/
