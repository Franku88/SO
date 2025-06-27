// Implementacion de mutex. Se debe ubicar en /xinu-pc/system
#include <xinu.h>
#include <mutex.h>

// Inicializa mutex
void mutex_init(struct mutex *m) {
		m->owner_pid = -1; // Sin propietario inicial
		m->sem = semcreate(1); // Crea semaforo binario
}

// Adquiere mutex
void mutex_lock(struct mutex *m) {
	wait(m->sem); // Espera al semaforo
	m->owner_pid = getpid(); // Reestablece propietario
}

// Desbloquea mutex
void mutex_unlock(struct mutex *m) {
	if (m->owner_pid == getpid()) { // Si el llamador es duenio actual
		m->owner_pid = -1; // Desocupa, estableciendo mutex sin duenio
		signal(m->sem); // Devuelve permiso al semaforo
	} else { // Si no es duenio del mutex
		kprintf("ERROR: Proceso no puede desbloquear mutex, no es propietario \n");
	}
}
