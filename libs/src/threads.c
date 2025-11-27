#include <sched.h>
#include <stdlib.h>
#include <pthread.h>

#include "semaphores.h"

#include "threads.h"

/*
 * Define a struct "Thread".
 * Os dados armazenados são usados
 * em diferentes momentos da execução.
 */
struct thread {
	pthread_t id;
	Routine *routine;
	void *context;
	void *result;
	int c_status;
	int r_status;
	Semaphore *semaphore;
};

/*
 * Define uma struct para armazenar
 * o contexto de execução "RoutineContext"
 */
typedef struct routine_context RoutineContext;
struct routine_context {
	Routine *routine;
	void *context;
	Semaphore *semaphore;
};

/*
 * Define uma função de encapsula a função
 * a ser executada pela thread.
 * Essa função é responsável por passar os
 * parâmetros da execução e recolher
 * um possível resultado.
 * O "sched_yield" é bom pra liberar a CPU
 * quando a thread executando demora muito.
 */
void *_RunRoutine(void *arg) {
	RoutineContext *ctx = (RoutineContext*) arg;
	
	sched_yield();
	
	void *result = ctx->routine(ctx->context, ctx->semaphore);
	free(ctx);
	
	return result;
}

/*
 * Aloca um "RoutineContext".
 * Preenche os campos com os parâmetros
 * informados.
 */
RoutineContext *_NewRoutineContext(Routine *routine, void *ctx, Semaphore *semaphore) {
	RoutineContext *rCtx = (RoutineContext *) malloc(sizeof(RoutineContext));
	if (rCtx == NULL) {
		return NULL;
	}
	
	rCtx->routine = routine;
	rCtx->context = ctx;
	rCtx->semaphore = semaphore;
	
	return rCtx;
}
 
/*
 * Cria uma nova "Thread" com os campos
 * especificados.
 */
Thread *NewThread(Routine *routine, void *ctx, Semaphore *semaphore) {
	Thread *thread = (Thread *) malloc(sizeof(Thread));
	if (thread == NULL) {
		return(NULL);
	}
	
	// Dados da thread
	thread->result = NULL;
	thread->context = ctx;
	thread->routine = routine;
	thread->semaphore = semaphore;
	
	return thread;
}

/*
 * Desaloca a "Thread".
 */
void DestroyThread(Thread **thread) {
	if (*thread == NULL) {
		return;
	}
	
	// Devolve recursos automaticamente
	pthread_detach((*thread)->id);
	
	free(*thread);
	*thread = NULL;
}

/*
 * Executa a "Thread" usando o
 * "RoutineContext" e a função
 * "_RunRoutine"
 */
void RunThread(Thread *thread) {
	if (thread == NULL) {
		return;
	}
	
	// Cria o "RoutineContext"
	RoutineContext *rCtx = _NewRoutineContext(thread->routine, thread->context, thread->semaphore);
	if (rCtx == NULL) {
		return;
	}
	
	// Cria a verdadeira thread (usa POSIX pq é mais fácil)
	thread->c_status = pthread_create(&thread->id, NULL, _RunRoutine, rCtx);
	if (thread->c_status != 0) {
		free(rCtx);
	}
}

/*
 * Retorna o resultado da execução
 * de uma "Thread".
 */
void *GetThreadResult(Thread *thread) {
	if (thread == NULL) {
		return NULL;
	}
	
	thread->r_status = pthread_join(thread->id, &thread->result);
	
	return thread->result;
}

/*
 * Retorna o status de
 * criação da thread POSIX
 * associada a "Thread".
 */
int GetThreadCreateStatus(Thread *thread) {
	if (thread == NULL) {
		return -1;
	}
	
	return thread->c_status;
}

/*
 * Retorna o staus de
 * execução da thread POSIX
 * associada a "Thread".
 */
int GetThreadRunStatus(Thread *thread) {
	if (thread == NULL) {
		return -1;
	}
	
	return thread->r_status;
}