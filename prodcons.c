#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <time.h>

int Buffer_Size = 10;
sem_t mutex, empty, full;
int ret0, ret1, count = 0;
pthread_t produtor, consumidor;

    
void produzir(){
    int i = 0;
    while(i < 100){ //while(1) para emular uma execução sem limite
        sem_wait(&empty);
        sem_wait(&mutex);
        printf("Produziu na posicao %d \n", count);
        count++;
        sem_post(&mutex);
        sem_post(&full);
        sleep(2);
        i++;
    }
}

void consumir(){
    int i = 0;
    while(i < 100){ //while(1) para emular uma execução sem limite
        sem_wait(&full);
        sem_wait(&mutex);
        printf("Consumiu na posicao %d \n", count);
        count--;
        sem_post(&mutex);
        sem_post(&empty);
        sleep(4);
        i++;
    }
}

int main(void) {
    
    sem_init(&mutex, 0, 1);
    sem_init(&empty, 0, Buffer_Size);
    sem_init(&full, 0, 0);
    
    ret0 = pthread_create(&produtor, NULL, produzir, NULL);
    ret1 = pthread_create(&consumidor, NULL, consumir, NULL);
    
    pthread_join(produtor, NULL);
    pthread_join(consumidor, NULL);
    
	return 0;
}
