#include <stdlib.h>
#include "kernel.h"
#include "list.h"

#ifndef NULL
#define NULL 0
#endif


/*****************************************************************************
 * Global variables
 *****************************************************************************/

static uint32_t id=1;
Task * tsk_running = NULL;	/* pointer to ready task list : the first
                                     node is the running task descriptor */
Task * tsk_prev = NULL;
Task * tsk_sleeping = NULL;	/* pointer to sleeping task list */

/*****************************************************************************
 * SVC dispatch
 *****************************************************************************/
/* sys_add
 *   test function
 */
int sys_add(int a, int b)
{
    return a+b;
}


/* syscall_dispatch
 *   dispatch syscalls
 *   n      : syscall SVC number
 *   args[] : array of the parameters (4 max)
 */
int32_t svc_dispatch(uint32_t n, uint32_t args[])
{
    int32_t result = -1;

    switch(n) {
        case 0:

            result = sys_add((int)args[0], (int)args[1]);
            break;

        case 1:

               result = (int32_t) malloc(args[0]);
            break;

        case 2:
        	   free(args[0]);
        	   result = 0;
              break;
        case 3:
           	  result = sys_os_start();
           	  break;
        case 4:
           	  result = sys_task_new((TaskCode)args[0], args[1]);
              break;
        case 5:
           	  result = sys_task_id();
           	  break;
        case 6:
           	  result = sys_task_wait(args[0]);
           	  break;
        case 7:
           	  result = sys_task_kill();
           	  break;
        case 8:
           	  result = sys_sem_new(args[0]);
           	  break;
        case 9:
           	  result = sys_sem_p((Semaphore*)args[0]);
           	  break;
        case 10:
           	  result = sys_sem_v((Semaphore*)args[0]);
           	  break;


    }

    return result;
}

void sys_switch_ctx()
{
	SCB->ICSR |= 1<<28; // set PendSV to pending
}
/*****************************************************************************
 * Round robin algorithm
 *****************************************************************************/
#define SYS_TICK  10	// system tick in ms

uint32_t sys_tick_cnt=0;

/* tick_cb
 *   system tick callback: task switching, ...
 */
void sys_tick_cb()
{
	//	Task* prev = tsk_running;
		tsk_running->status = TASK_SLEEPING;
		tsk_prev = tsk_running;
		tsk_running = tsk_prev->next;
		tsk_running->status = TASK_RUNNING;
		sys_switch_ctx();

		//delay management

		int size = list_size(tsk_sleeping);
		for(int i = 0; i < size; ++i)
		{
			tsk_sleeping->delay -= SYS_TICK;
			if (tsk_sleeping->delay <= 0)//if timeout
			{
				tsk_sleeping->delay = 0;//reset delay

				Task* tskDelay;
				//get next sleeping task and store current to tskDelay
				tsk_sleeping = list_remove_head(tsk_sleeping,&tskDelay);
				tskDelay->status = TASK_READY;//set current task to ready
				tsk_running = list_insert_tail(tsk_running,tskDelay);//add it to running tasks
			}
			else//no timeout, just skip it
				tsk_sleeping = tsk_sleeping->next;

			sys_switch_ctx();
		}
}

void SysTick_Handler(void)
{
	sys_tick_cnt++;

	if (sys_tick_cnt == SYS_TICK) {
		sys_tick_cnt = 0;
		sys_tick_cb();
	}
}

/*****************************************************************************
 * General OS handling functions
 *****************************************************************************/

/* sys_os_start
 *   start the first created task
 */
int32_t sys_os_start()
{
	/* A COMPLETER */
	tsk_running->status = TASK_RUNNING;

	sys_switch_ctx();
    // Reset BASEPRI
    __set_BASEPRI(0);

	// Set systick reload value to generate 1ms interrupt
    SysTick_Config(SystemCoreClock / 1000U);
    return 0;
}

/*****************************************************************************
 * Task handling functions
 *****************************************************************************/
void task_kill();

/* sys_task_new
 *   create a new task :
 *   func      : task code to be run
 *   stacksize : task stack size
 *
 *   Stack frame:
 *      |    xPSR    |
 *      |     PC     |
 *      |     LR     |
 *      |     R12    |    ^
 *      |     R3     |    ^
 *      |     R2     |    | @++
 *      |     R1     |
 *      |     R0     |
 *      +------------+
 *      |     R11    |
 *      |     R10    |
 *      |     R9     |
 *      |     R8     |
 *      |     R7     |
 *      |     R6     |
 *      |     R5     |
 *      |     R4     |
 *      +------------+
 *      | EXC_RETURN |
 *      |   CONTROL  | <- sp
 *      +------------+
 */
int32_t sys_task_new(TaskCode func, uint32_t stacksize) {
    // Ensure the stack size is at least 96 bytes and aligned to an 8-byte boundary
    // Explanation: ARM Cortex-M processors require the stack to be aligned to 8 bytes for proper
    // functioning of the hardware stack pointer. The stack size is rounded up to the next multiple
    // of 8 bytes if necessary. The minimum size of 96 bytes ensures that even the smallest task
    // has enough space for context saving and local variables.
    uint32_t size = stacksize > 96 ? 8 * (((stacksize - 1) / 8) + 1) : 96;

    // Allocate memory for the Task structure and its stack
    // Explanation: The memory layout consists of the `Task` structure followed by the stack memory.
    // The stack is allocated as part of this memory block to simplify memory management.
    Task* myTask = (Task*)malloc(sizeof(Task) + size);
    if (!myTask) {
        // If memory allocation fails, return an error code
        // Explanation: Memory allocation can fail if the system is out of memory. In such a case,
        // return `-1` to signal the error to the caller.
        return -1;
    }

    // Initialize the task fields
    myTask->id = id++;            // Assign a unique ID to the task
    myTask->status = TASK_READY;  // Set the initial status of the task to "READY"
    // Explanation: The unique ID is incremented for every new task, ensuring that each task has a distinct identifier.
    // The status "READY" means the task is ready to be scheduled for execution.

    // Set up the stack pointers
    // Explanation: The stack grows downward in memory. `splim` points to the start of the allocated stack space,
    // and `sp` is initialized to the end of the stack (top of the stack in memory).
    myTask->splim = (uint32_t*)(myTask + 1); // `splim` points to the start of the stack area
    myTask->sp = myTask->splim + (size / 4); // `sp` points to the top of the stack area

    // Reserve space for context saving (18 words for ARM context saving)
    // Explanation: When a context switch occurs, 18 words of stack space are needed to save the CPU state,
    // including general-purpose registers, program counter (PC), link register (LR), and program status register (xPSR).
    myTask->sp -= 18;

    // Initialize the stack frame with default values for the task's context
    // Explanation: The task starts execution with this stack frame. It includes initial values for
    // the CONTROL register, EXC_RETURN, LR, PC, and xPSR, which define the execution state of the task.
    myTask->sp[0] = 0x0 | (0x1 << 0);    // CONTROL: Set the task to unprivileged mode (bit 0 set)
    myTask->sp[1] = 0xFFFFFFFD;          // EXC_RETURN: Indicate thread mode with PSP (Process Stack Pointer)
    myTask->sp[15] = (uint32_t)task_kill;// LR: Points to `task_kill` function, called when the task ends
    myTask->sp[16] = (uint32_t)func;     // PC: Points to the entry function of the task
    myTask->sp[17] = 1 << 24;            // xPSR: Set the Thumb bit (bit 24) as required by ARM Thumb instructions

    // Add the new task to the running task list
    // Explanation: `tsk_running` is a global list that maintains all tasks that are ready or running.
    // `list_insert_tail` adds the new task to the end of this list.
    tsk_running = list_insert_tail(tsk_running, myTask);
    if (tsk_running == NULL) {
        // If adding the task to the list fails, free the allocated memory and return an error code
        // Explanation: If the task list insertion fails due to an internal error, cleanup is performed
        // by freeing the allocated memory for the task, and the function returns `-1` to indicate failure.
        if (myTask != NULL) {
            free(myTask); // Free allocated memory for the task
        }
        return -1; // Return error code
    }

    // Return the unique ID of the newly created task
    // Explanation: The task ID serves as a handle for the caller to refer to the created task.
    return myTask->id;
}


/* sys_task_kill
 *   kill oneself
 */
int32_t sys_task_kill()
{
	Task *tskToKill;
		tsk_running = list_remove_head(tsk_running,&tskToKill);
		tsk_running->status = TASK_RUNNING;
		sys_switch_ctx();
		free(tskToKill);

		return 0;
}

/* sys_task_id
 *   returns id of task
 */
int32_t sys_task_id()
{

	return tsk_running->id;
}


/* sys_task_yield
 *   run scheduler to switch to another task
 */
int32_t sys_task_yield()
{

    return -1;
}

/* task_wait
 *   suspend the current task until timeout
 */
int32_t sys_task_wait(uint32_t ms)
{
	tsk_running = list_remove_head(tsk_running, &tsk_prev);

	tsk_sleeping = list_insert_tail(tsk_sleeping, tsk_prev);

	tsk_prev->delay = ms;
	tsk_prev->status = TASK_WAITING;
	tsk_running->status = TASK_RUNNING;

	sys_switch_ctx();

	return 0;
}


/*****************************************************************************
 * Semaphore handling functions
 *****************************************************************************/

/* sys_sem_new
 *   create a semaphore
 *   init    : initial value
 */
Semaphore * sys_sem_new(int32_t init)
{
	Semaphore* sema = (Semaphore*)malloc(sizeof(Semaphore));
	sema->count = init;
	sema->waiting = NULL;

    return sema;
}

/* sys_sem_p
 *   take a token
 */
int32_t sys_sem_p(Semaphore * sem)
{
	--sem->count;
		if(sem->count < 0)
		{
			Task* tsk;

			tsk_running = list_remove_head(tsk_running, &tsk);

			sem->waiting = list_insert_tail(sem->waiting, tsk);
			sem->waiting->status = TASK_WAITING;
			tsk_running->status = TASK_RUNNING;

			tsk_prev = tsk;
			sys_switch_ctx();
		}

		return sem->count;
}

/* sys_sem_v
 *   release a token
 */
int32_t sys_sem_v(Semaphore * sem)
{
	++sem->count;
		if(sem->waiting != NULL)
		{
			Task* task;

			sem->waiting = list_remove_head(sem->waiting, &task);

			tsk_prev = tsk_running;

			tsk_running = list_insert_head(tsk_running, task);
			task->status = TASK_RUNNING;
			tsk_prev->status = TASK_READY;

			sys_switch_ctx();
		}

		return sem->count;
}


void print_semaphore_status(Semaphore* mutex) {
    printf("Mutex count: %d\n", mutex->count);

    if (mutex->waiting != NULL) {
        printf("Waiting tasks id: %d\n", mutex->waiting->id);
    } else {
        printf("No tasks are waiting.\n");
    }
}
