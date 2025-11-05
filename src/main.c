#include <stdio.h>

#include "semaphores.h"
#include "threads.h"

int shared = 0;
int sharedLock = 0;

// Com lock
void *SumLock(void *data, Semaphore *s) {
	int size = *(int*) data;
	
	if (size < 0) {
		return NULL;
	}
	
	for (int i = 0; i < size; i++) {
		SemaphoreLock(s);
		sharedLock++;
		SemaphoreUnlock(s);
	}
	
	return NULL;
}

// Sem lock
void *Sum(void *data, Semaphore *s) {
	int size = *(int*) data;
	
	if (size < 0) {
		return NULL;
	}
	
	for (int i = 0; i < size; i++) {
		shared++;
	}
	
	return NULL;
}

int main(void) {
	int n = 1000000;
	
	shared = 0;
	sharedLock = 0;
	
	// Criar Semaphore compartilhado
	Semaphore *semaphore = NewSemaphore();
	if (semaphore == NULL) {
		return -1;
	}
	
	// Função com lock
	Thread *t1 = NewThread(SumLock, &n, semaphore);
	Thread *t2 = NewThread(SumLock, &n, semaphore);
	
	// Função sem lock
	Thread *t3 = NewThread(Sum, &n, NULL);
	Thread *t4 = NewThread(Sum, &n, NULL);
	
	if (!t1 || !t2) {
		printf("Fialed to create threads!\n");
		return 1;
	}
	
	RunThread(t1);
	RunThread(t2);
	RunThread(t3);
	RunThread(t4);
	
	GetThreadResult(t1);
	GetThreadResult(t2);
	GetThreadResult(t3);
	GetThreadResult(t4);
	
	printf("Valor compartilhado sem lock %d\n", shared);
	printf("Valor compartilhado com lock %d\n", sharedLock);
	
	DestroyThread(&t1);
	DestroyThread(&t2);
	DestroyThread(&t3);
	DestroyThread(&t4);
	
	DestroySemaphore(&semaphore);
	
	return 0;
}