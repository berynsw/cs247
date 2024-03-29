/*
 Beryn Staub-Waldenberg
 Assignment 2 for cs247 spring2019
 I received skeleton code from my instructor, Susheel Gopalan




 [5]endtime Sun Oct 15 12:30:26 2017
 Sun Oct 15 12:30:26 2017
  Sun Oct 15 12:30:26 2017

Thread# [5] id [0x7f9d0a247700] exiting
DisplayThreadSchdAttributes:
        threadID	= 0x7f9d0a247700
        policy 		= SCHED_FIFO
        priority 	= 62
        startTime 	= Sun Oct 15 12:29:49 2017
        endTime 	= Sun Oct 15 12:30:26 2017
        Task start TimeStamp in micro seconds [1508095789066499]
        Task end   TimeStamp in micro seconds [1508095801582953]   Delta [12516454]us
        Task end   TimeStamp in micro seconds [1508095814150624]   Delta [12567671]us
        Task end   TimeStamp in micro seconds [1508095826671818]   Delta [12521194]us

 *
 */

/*
	CreateEvent
	pthread_cond_init

	CloseHandle
	pthread_cond_destroy
	pthread_cond_wait

	WaitForSingleObject
	pthread_cond_timedwait
	pthread_cond_signal

	SetEvent
	pthread_cond_signal
	pthread_cond_broadcast
*/
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <inttypes.h>
#include <signal.h>
#include <errno.h>

#define	MAX_THREAD_COUNT		3


#define LOW_THREAD_PRIORITY		50

#define STACK_SIZE				0x400000
#define	MAX_TASK_COUNT			3

//define error handling later on
#define handle_error_en(en, msg) \
        do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)


pthread_mutex_t		g_DisplayMutex = PTHREAD_MUTEX_INITIALIZER;;
pthread_mutex_t 	g_SignalMutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct{
	int			threadCount;
	pthread_t	threadId;
	int		threadPolicy;
	int			threadPri;
  struct sched_param param;
	long		processTime;
	//long		timeStamp[MAX_TASK_COUNT+1];
	int64_t		timeStamp[MAX_TASK_COUNT+1];
	time_t		startTime;
	time_t		endTime;

	int sig;
	sigset_t alarm_sig;
	int wakeups_missed;

	timer_t timer_id;
	int timer_Period;
    //sigset_t timer_signal;
} ThreadArgs;

ThreadArgs g_ThreadArgs[MAX_THREAD_COUNT];

void InitThreadArgs(void)
{
	for(int i=0;i<MAX_THREAD_COUNT;i++)
	{
		g_ThreadArgs[i].threadCount = 0;
		g_ThreadArgs[i].threadId =0;
		g_ThreadArgs[i].threadPri = 0;
		g_ThreadArgs[i].processTime =0;
		for(int y=0; y<MAX_TASK_COUNT+1; y++)
		{
			g_ThreadArgs[i].timeStamp[y] = 0;
		}
	}

	pthread_mutex_init ( &g_DisplayMutex, NULL);
}

void DisplayThreadSchdAttributes( void )
{
	   int policy;
	   struct sched_param param;

	   pthread_getschedparam(pthread_self(), &policy, &param);

	   printf("DisplayThreadSchdAttributes:\n        threadID = 0x%lx\n        policy = %s\n        priority = %d\n",
			pthread_self(),
		   (policy == SCHED_FIFO)  ? "SCHED_FIFO" :
		   (policy == SCHED_RR)	   ? "SCHED_RR" :
		   (policy == SCHED_OTHER) ? "SCHED_OTHER" :
		   "???",
		   param.sched_priority);
}

void DisplayThreadArgs(ThreadArgs*	myThreadArg)
{

	pthread_mutex_lock	(&g_DisplayMutex);
	if( myThreadArg )
	{
		//printf("DisplayThreadArgs: Thread Id [%lx] Pri [%d] ProcTime [%ld]\n",
		//		myThreadArg->threadId, myThreadArg->threadPri, myThreadArg->processTime);

			DisplayThreadSchdAttributes();
      printf("        wakeups_missed = %d\n",myThreadArg->wakeups_missed );
      printf("        startTime = %s        endTime = %s\n", ctime(&myThreadArg->startTime), ctime(&myThreadArg->endTime));
      printf("        TimeStamp [%"PRId64"]\n", myThreadArg->timeStamp[0] );


			for(int y=1; y<MAX_TASK_COUNT+1; y++)
			{
				printf("        TimeStamp [%"PRId64"]   Delta [%"PRId64"]us     Jitter[%"PRId64"]us\n", myThreadArg->timeStamp[y],
							(myThreadArg->timeStamp[y]-myThreadArg->timeStamp[y-1]),
							(myThreadArg->timeStamp[y]-myThreadArg->timeStamp[y-1]-myThreadArg->timer_Period));
			}


	}
	else
	{

		for(int i=0;i<MAX_THREAD_COUNT;i++)
		{
			//printf("DisplayThreadArgs: Thread Id [%lx] Pri [%d] ProcTime [%ld]\n",
			//	g_ThreadArgs[i].threadId, g_ThreadArgs[i].threadPri, g_ThreadArgs[i].processTime);

			DisplayThreadSchdAttributes();
			printf("   TimeStamp (task starting) 	[%ld]\n", g_ThreadArgs[i].timeStamp[0] );

			for(int y=1; y<MAX_TASK_COUNT+1; y++)
			{
				printf("   TimeStamp (task completed)	[%ld]   Delta [%ld]\n", g_ThreadArgs[i].timeStamp[y],
							(g_ThreadArgs[i].timeStamp[y]-g_ThreadArgs[i].timeStamp[y-1]));
			}
		}
	}

	pthread_mutex_unlock	(&g_DisplayMutex);
}

void CreateAndArmTimer(int unsigned period, ThreadArgs* info)
{
	int ret;
	unsigned int nanoseconds;
	unsigned int seconds;
	struct sigevent mySignalEvent;
	struct itimerspec timerSpec;



	//1) Find way to initialized info->sig to have a unique value b/t SIGRTMIN and SIGRTMAX range: 32-64
  //2) Ensure that this initialization is thread safe
  ret = pthread_mutex_lock ( &g_SignalMutex );
  if(ret)
    handle_error_en(ret, "pthread_mutex_lock");

    int sigvalue = SIGRTMAX - info->threadCount;
  	if(sigvalue >= SIGRTMIN)
  		info->sig = sigvalue;
  	else
  		handle_error_en(sigvalue, "invalid signal value");

  ret = pthread_mutex_unlock ( &g_SignalMutex );
  if(ret)
    handle_error_en(ret, "pthread_mutex_unlock");




	//3) Initialize info->timer_Period to the argument period - done in main function

	//4) Use "sigemptyset" to initialize info->alarm_sig
	ret = sigemptyset( &info->alarm_sig );
  if(ret == -1)
    handle_error_en(ret, "sigemptyset");

	//5) Use "signaddset" to add info->sig to info->alarm_sig
	ret = sigaddset( &info->alarm_sig, info->sig );
  if(ret == -1)
    handle_error_en(ret, "sigaddset");

	//6) Initialize local variable "mySignalEvent" and Call "timer_create" to create timer
	struct sigevent mysigevent;
	mysigevent.sigev_notify = SIGEV_SIGNAL;
	mysigevent.sigev_signo = info->sig;
	mysigevent.sigev_value.sival_ptr = (void *)&(info->timer_id);
	ret = timer_create(CLOCK_MONOTONIC, &mysigevent, &info->timer_id);
  if(ret == -1)
    handle_error_en(ret, "timer_create");

	//7) Initialize local variable "timerSpec" and call "timer_settime" to set the time out
  // Arm the timer for a period defined in microseconds
  seconds= period/1000000;
  nanoseconds= (period -(seconds * 1000000)) * 1000;
  timerSpec.it_interval.tv_sec = seconds;
  timerSpec.it_interval.tv_nsec = nanoseconds;
  timerSpec.it_value.tv_sec = seconds;
  timerSpec.it_value.tv_nsec = nanoseconds;
  ret = timer_settime (info->timer_id, 0, &timerSpec, NULL);
  if(ret == -1)
    handle_error_en(ret, "timer_settime");
}

static void wait_period (ThreadArgs *info)
{
	//int sig;
  int error;

	//1) Call "sigwait" to wait on info->alarm_sig
  error = sigwait( &info->alarm_sig, &info->sig );
  if(error)
    handle_error_en(error, "sigwait");

	//2) Update "info->wakeups_missed" with the return from "timer_getoverrun"
  error = timer_getoverrun( info->timer_id);
  if(error == -1)
    handle_error_en(error, "timer_getoverrun");
  else
    info->wakeups_missed += timer_getoverrun(info->timer_id);


}
void* threadFunction(void *arg)
{
	ThreadArgs*	myThreadArg;
	struct timeval	t1;
	struct timespec tms;
	int y, retVal;

	myThreadArg = (ThreadArgs*)arg;

	if( myThreadArg->threadId != pthread_self() )
	{
		printf("mismatched thread Ids... exiting...\n");
		pthread_exit(arg);
	}
	else
	{

		retVal = pthread_setschedparam(pthread_self(), myThreadArg->threadPolicy, &myThreadArg->param);		//SCHED_FIFO, SCHED_RR, SCHED_OTHER
		if(retVal != 0){
			handle_error_en(retVal, "pthread_setschedparam");
		}
		myThreadArg->processTime = 0;
		DisplayThreadSchdAttributes();
	}

	CreateAndArmTimer(myThreadArg->timer_Period, myThreadArg);
	myThreadArg->startTime = time(NULL);
	//printf("\n [%d]starttime %s\n",myThreadArg->threadCount, ctime(&myThreadArg->startTime));

	y=0;
	while (y < MAX_TASK_COUNT)
	{
		//gettimeofday(&t1, NULL);
		clock_gettime(CLOCK_REALTIME, &tms);
		myThreadArg->timeStamp[y] = tms.tv_sec *1000000;
		myThreadArg->timeStamp[y] += tms.tv_nsec/1000;
		if(tms.tv_nsec % 1000 >= 500 )  myThreadArg->timeStamp[y]++;

		wait_period(myThreadArg);

		y++;

		clock_gettime(CLOCK_REALTIME, &tms);
		myThreadArg->timeStamp[y] = tms.tv_sec *1000000;
		myThreadArg->timeStamp[y] += tms.tv_nsec/1000;
		if(tms.tv_nsec % 1000 >= 500 )  myThreadArg->timeStamp[y]++;
	}

	time_t tmp;
	tmp = time(NULL);
	myThreadArg->endTime = time(NULL);

	DisplayThreadArgs(myThreadArg);

	pthread_exit(NULL);


	return NULL;
}

int main (int argc, char *argv[])
{
	int threadCount = 0;
  int err, i;
	int fifoPri = 60;
	int period = 1;
	int retVal;

	pthread_attr_t threadAttrib;

   sigset_t timer_signal;
   sigemptyset(&timer_signal);
   for (int i = SIGRTMIN; i <=SIGRTMAX; i++)
      sigaddset(&timer_signal, i);
   sigprocmask(SIG_BLOCK, &timer_signal, NULL);


	InitThreadArgs();

	for (int i = 0; i < MAX_THREAD_COUNT; i ++)
	{
		g_ThreadArgs[i].threadCount = i+1;
	  g_ThreadArgs[i].threadPolicy = SCHED_FIFO;
	  g_ThreadArgs[i].param.sched_priority = fifoPri++;
		g_ThreadArgs[i].timer_Period = (period << i)*1000000;

	  retVal = pthread_create(&g_ThreadArgs[i].threadId, NULL, threadFunction, &g_ThreadArgs[i]);
		if(retVal != 0)
		{
			handle_error_en(retVal, "pthread_create");
		}
	}

	for(int i = 0; i < MAX_THREAD_COUNT; i++)
	{
      pthread_join(g_ThreadArgs[i].threadId, NULL);
    }

	printf("Main thread is exiting\n");

    return 0;
}
