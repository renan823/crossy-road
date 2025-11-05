#include <stdlib.h>
#include <semaphore.h>

#include "semaphores.h"

/*
 * Definição do tipo "Semaphore".
 * Armazena apenas o identificador
 * do semáforo em questão.
 */
struct semaphore {
	sem_t id;
};

/*
 * Cria um novo "Semaphore".
 * Usa a biblioteca "semaphore.h"
 * para iniciar o semáforo.
 */
Semaphore *NewSemaphore() {
	Semaphore *semaphore = (Semaphore*) malloc(sizeof(Semaphore));
	if (semaphore == NULL) {
		return NULL;
	}
	
	// Init semaphore
	sem_init(&semaphore->id, 0, 1);
	
	return semaphore;
}

/*
 * Desaloca um "Semaphore".
 */
void DestroySemaphore(Semaphore **semaphore) {
	if (*semaphore == NULL) {
		return;
	}
	
	free(*semaphore);
	*semaphore = NULL;
}

/*
 * Bloqueia o semáforo contido
 * no "Semaphore".
 */
void SemaphoreLock(Semaphore *semaphore) {
	if (semaphore == NULL) {
		return;
	}
	
	sem_wait(&semaphore->id);
}

/*
 * Desbloqueia o semáforo contido
 * no "Semaphore".
 */
void SemaphoreUnlock(Semaphore *semaphore) {
	if (semaphore == NULL) {
		return;
	}
	
	sem_post(&semaphore->id);
}