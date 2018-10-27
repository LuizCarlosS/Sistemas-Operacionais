#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <time.h>

int Buffer_Size = 10;//tamanho arbitrário para o buffer
sem_t mutex, empty, full;
int ret0, ret1, count = 0;
pthread_t produtor, consumidor;

    
void produzir(){
    int i = 0;
    while(i < 100){ //while(1) para emular uma execução sem limite
        sem_wait(&empty); //inicialmente com o tamanho do buffer, verifica os espaços vazios
        sem_wait(&mutex);//controle da região crítica
        count++;//quantidade de itens e posição para inserção de itens no buffer
        printf("Produziu na posicao %d \n", count);//abstração do produtor
        sleep(2);//para simular o tempo de produção
        sem_post(&mutex);//saida da região crítica
        sem_post(&full);//inicialmente com tamanho 0, agora é incrementado, representando a adição de um item ao buffer

        i++;//apenar para terminar o loop
    }
}

void consumir(){
    int i = 0;
    while(i < 100){ //while(1) para emular uma execução sem limite
        sem_wait(&full);//verifica se o tamanho do buffer ainda é 0
        sem_wait(&mutex);//controle da região crítica
        count--;//remove um item do buffer, liberando um espaço
        printf("Consumiu na posicao %d \n", count); //abstração de um consumo
        sleep(4);//simular tempo de consumo
        sem_post(&mutex);//saída da regiçao crítica
        sem_post(&empty);//incrementa o empty, representando a remoção de um item do buffer
        i++;//apenas para terminar o loop
    }
}

int main(void) {
    
    sem_init(&mutex, 0, 1);//criação do semáforo para região crítica
    sem_init(&empty, 0, Buffer_Size);//criação do semáforo para verificar se é possível adicionar itens ao buffer
    sem_init(&full, 0, 0);//criação do semáforo para verificar se é possível remover itens do buffer
    
    ret0 = pthread_create(&produtor, NULL, produzir, NULL);//cria a thread para um produtor
    ret1 = pthread_create(&consumidor, NULL, consumir, NULL);//cria a threa para um consumidor
    
    pthread_join(produtor, NULL);//faz o main esperar a execução da thread do produtor
    pthread_join(consumidor, NULL);//faz o main esperar a execução da thread do produtor
    
	return 0;
}
