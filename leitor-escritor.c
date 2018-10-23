#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <time.h>

int Buffer_Size = 10;
sem_t mutex, escritex;
int ret0, ret1, readerCount = 0, READERS = 2;
pthread_t leitores[2], escritor;
//srand(time(NULL));
    
void reader(){
    int i = 0;
    while(i < 100){ //while(1) para emular uma execuï¿½ï¿½o sem limite
        sem_wait(&mutex);
        readerCount++;
        printf("Há %d leitores\n", readerCount);
        if(readerCount == 1) sem_wait(&escritex);
        sem_post(&mutex);
        
        printf("Lendo...\n");
        sleep(1+ (rand()%3));
        
        sem_wait(&mutex);
        readerCount--;
        
        if(readerCount == 0) sem_post(&escritex);
        sem_post(&mutex);
        
        i++;
    }
}

void writer(){
    int i = 0;
    while(i < 100){ //while(1) para emular uma execuï¿½ï¿½o sem limite
        sem_wait(&escritex);
        printf("Escrevendo...\n");
        sem_post(&escritex);
        sleep(3+(rand()%5));
        
        i++;
    }
}

int main(void) {
    
    sem_init(&mutex, 0, 1);
    sem_init(&escritex, 0, 1);
    
    for(int i=0; i<READERS; i++){
    	ret0 = pthread_create( &leitores[i], NULL,reader, NULL);
	}
    ret1 = pthread_create(&escritor, NULL, writer, NULL);
    
    for(int i=0; i<READERS; i++){
    	pthread_join(leitores[i], NULL);
	}
    pthread_join(escritor, NULL);
    
	return 0;
}
