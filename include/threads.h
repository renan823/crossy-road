#ifndef THREADS_H_
#define THREADS_H_

/*
 * Definições dos tipos e funções
 * para a biblioteca de threads.
 */

#include "semaphores.h"

typedef struct thread Thread;

typedef void *Routine(void*, Semaphore*);

Thread *NewThread(Routine *routine, void *ctx, Semaphore *semaphore);
void DestroyThread(Thread **thread);

void RunThread(Thread *thread);
void *GetThreadResult(Thread *thread);

int GetThreadCreateStatus(Thread *thread);
int GetThreadRunStatus(Thread *thread);

#endif