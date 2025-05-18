#include <xinu.h>

void sndA(void), sndB(void);

void hello() {
      int idA, idB;
      idA = create(sndA, 1024, 20, "process 1", 0);
      idB = create(sndB, 1024, 20, "process 2", 0);
      printf("Hola mundo! \n");
      resume(idA);
      resume(idB);
      sleepms(10);
      kill(idA);
      kill(idB);
      putc(CONSOLE, '\n');
}

void sndA(void) {
  while(1){
    putc(CONSOLE, 'A');
    sleepms(2);
  }
}

void sndB(void) {
  while(1){
    putc(CONSOLE, 'B');
    sleepms(3);
  }
}

