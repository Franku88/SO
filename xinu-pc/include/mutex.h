// Definicion de archivo header para mutex. Se debe ubicar en /xinu-pc/include

#ifndef MUTEX_H
#define MUTEX_H

struct mutex {
	pid32 owner_pid; 	// PID del proceso que adquiere el mutex
	sid32 sem;			// Semaforo binario del mutex
};

/* Inicializa el mutex (initialize)*/
void mutex_init(struct mutex *m);

/* Adquiere el mutex (lock)*/
void mutex_lock(struct mutex *m);

/* Inicializa el mutex (unlock)*/
void mutex_unlock(struct mutex *m);

#endif /*MUTEX_H*/
