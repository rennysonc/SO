#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

#define FILOSOFOS 5

#define PENSANDO 2
#define FAMINTO 1
#define COMENDO 0

#define GARFO_1 ((identificacaoFilosofo + 4) % FILOSOFOS)
#define GARFO_2 ((identificacaoFilosofo + 1) % FILOSOFOS)


sem_t semF[FILOSOFOS];

int filosofos[FILOSOFOS];
int tipodeGarfo[FILOSOFOS];


pthread_mutex_t lock;

void comendo(int identificacaoFilosofo) {
  printf("O Filosofo %d leva o garfo %d e o %d\n",
         identificacaoFilosofo + 1, GARFO_1 + 1, identificacaoFilosofo + 1);
  printf("O Filosofo %d está comendo\n", identificacaoFilosofo + 1);
  sleep(1);
}

void ComaPrimeiro(int identificacaoFilosofo) {
  if (tipodeGarfo[identificacaoFilosofo] == FAMINTO
      && tipodeGarfo[GARFO_1] != COMENDO
      && tipodeGarfo[GARFO_2] != COMENDO) {
    tipodeGarfo[identificacaoFilosofo] = COMENDO;
    comendo(identificacaoFilosofo);
    sem_post(&semF[identificacaoFilosofo]);
  }
}


void pegarEcomer(int identificacaoFilosofo) {
  pthread_mutex_lock(&lock);
  tipodeGarfo[identificacaoFilosofo] = FAMINTO;
  printf("O Filosofo %d está faminto\n", identificacaoFilosofo + 1);
  ComaPrimeiro(identificacaoFilosofo);
  pthread_mutex_unlock(&lock);
  sem_wait(&semF[identificacaoFilosofo]);
  sleep(1);
}


void garfoEpensar(int identificacaoFilosofo) {
  pthread_mutex_lock(&lock);
  tipodeGarfo[identificacaoFilosofo] = PENSANDO;
  printf("O Filosofo %d coloca o garfo %d e o garfo %d fica abaixado\n",
         identificacaoFilosofo + 1, GARFO_1 + 1, identificacaoFilosofo + 1);
  printf("O Filosofo %d está pensando\n", identificacaoFilosofo + 1);
  ComaPrimeiro(GARFO_1);
  ComaPrimeiro(GARFO_2);
  pthread_mutex_unlock(&lock);
  sleep(1);
}

_Noreturn void *novoCiclo(void *num) {
  int *i = num;
  printf("O Filosofo %d está pensando\n", *i + 1);
  while (1) {
    pegarEcomer(*i);
    garfoEpensar(*i);
  }
}

int main() {
  int i;

  pthread_t filosofos_threads[FILOSOFOS];

  
  pthread_mutex_init(&lock, NULL);

  
  for (i = 0; i < FILOSOFOS; i++) {
    sem_init(&semF[i], 0, 0);
    filosofos[i] = i;
  }

 
  for (i = 0; i < FILOSOFOS; i++) {
    pthread_create(&filosofos_threads[i], NULL,
                   novoCiclo, &filosofos[i]);
  }

  for (i = 0; i < FILOSOFOS; i++)
    pthread_join(filosofos_threads[i], NULL);
}