#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define MAX 1000000000
#define ARRAYBUFFER 10

pthread_mutex_t the_mutex;
pthread_cond_t condc, condp;
int buffer[ARRAYBUFFER];

const clock_t MAXDELAY = 2000000;

void delay(clock_t ticks) { /* a "busy" delay */
  clock_t start = clock();

  do
    ;
  while (clock() < start + ticks);
}

int bufferIsEmpty(void) {
  for (int i = 0; i < ARRAYBUFFER; i++)
    if (buffer[i] != 0)
      return 0;
  
  return 1;
}

int bufferIsFull(void) {
  for (int i = 0; i < ARRAYBUFFER; i++)
    if (buffer[i] == 0)
      return 0;

  return 1;
}

void insertValueToBuffer(int value) {
  for (int i = 0; i < ARRAYBUFFER; i++) {
    if (buffer[i] == 0) {
      buffer[i] = value;
      return;
    }
  }

  return;
}

int deleteBufferValue(void) {
  for (int i = ARRAYBUFFER; i >= 0; i--) {
    if (buffer[i] != 0) {
      int x = buffer[i];
      buffer[i] = 0;
      return x;
    }
  }

  return 0;
}

void *producer(void *ptr) {
  /* usado para sinalizacao */
  /* buffer usado entre produtor e consumidor */
  /* dados do produtor */
  for (int i = 1; i <= MAX; i++) {
    pthread_mutex_lock(&the_mutex); /* obtem acesso exclusivo ao buffer */
    
    while (bufferIsFull())
      pthread_cond_wait(&condp, &the_mutex);
    
    printf("Colocando item %d no buffer\n", i);
    
    insertValueToBuffer(i); /*coloca item no buffer */
    delay(MAXDELAY);
    pthread_cond_signal(&condc); /* acorda consumidor */
    pthread_mutex_unlock(&the_mutex); /* libera acesso ao buffer */
  }

  pthread_exit(0);
  return;
}

void *consumer(void *ptr) {
  /* quantos numeros produzir */
  /* dados do consumidor */
  for (int i = 1; i <= MAX; i++) {
    pthread_mutex_lock(&the_mutex); /* obtem acesso exclusivo ao buffer */
    
    while (bufferIsEmpty())
      pthread_cond_wait(&condc, &the_mutex);
    
    int x = deleteBufferValue();
    printf("Retirando item %d no buffer\n", x);
    
    delay(MAXDELAY);
    pthread_cond_signal(&condp);  /* acorda o produtor */
    pthread_mutex_unlock(&the_mutex); /* libera acesso ao buffer */
  }

  pthread_exit(0);
  return;
}

int main(int argc, char **argv) {
  pthread_t pro, con;
  
  pthread_mutex_init(&the_mutex, 0);
  pthread_cond_init(&condc, 0);
  pthread_cond_init(&condp, 0);
  
  pthread_create(&con, 0, consumer, 0);
  pthread_create(&pro, 0, producer, 0);
  
  pthread_join(pro, 0);
  pthread_join(con, 0);
  
  pthread_cond_destroy(&condc);
  pthread_cond_destroy(&condp);
  pthread_mutex_destroy(&the_mutex);

  return 0;
}