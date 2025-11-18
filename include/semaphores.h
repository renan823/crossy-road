#ifndef SEMAPHORES_H_
#define SEMAPHORES_H_

/*
 * Definições de tipos e funções para
 * a biblioteca de semáforos.
 */

typedef struct semaphore Semaphore;

Semaphore *NewSemaphore();
void DestroySemaphore(Semaphore **semaphore);

void SemaphoreLock(Semaphore *semaphore);
void SemaphoreUnlock(Semaphore *semaphore);

#endif