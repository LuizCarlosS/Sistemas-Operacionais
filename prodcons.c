#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <time.h>

int Buffer_Size = 10;//tamanho arbitr�rio para o buffer
sem_t mutex, empty, full;
int ret0, ret1, count = 0;
pthread_t produtor, consumidor;

    
void produzir(){
    int i = 0;
    while(i < 100){ //while(1) para emular uma execu��o sem limite
        sem_wait(&empty); //inicialmente com o tamanho do buffer, verifica os espa�os vazios
        sem_wait(&mutex);//controle da regi�o cr�tica
        count++;//quantidade de itens e posi��o para inser��o de itens no buffer
        printf("Produziu na posicao %d \n", count);//abstra��o do produtor
        sleep(2);//para simular o tempo de produ��o
        sem_post(&mutex);//saida da regi�o cr�tica
        sem_post(&full);//inicialmente com tamanho 0, agora � incrementado, representando a adi��o de um item ao buffer

        i++;//apenar para terminar o loop
    }
}

void consumir(){
    int i = 0;
    while(i < 100){ //while(1) para emular uma execu��o sem limite
        sem_wait(&full);//verifica se o tamanho do buffer ainda � 0
        sem_wait(&mutex);//controle da regi�o cr�tica
        count--;//remove um item do buffer, liberando um espa�o
        printf("Consumiu na posicao %d \n", count); //abstra��o de um consumo
        sleep(4);//simular tempo de consumo
        sem_post(&mutex);//sa�da da regi�ao cr�tica
        sem_post(&empty);//incrementa o empty, representando a remo��o de um item do buffer
        i++;//apenas para terminar o loop
    }
}

int main(void) {
    
    sem_init(&mutex, 0, 1);//cria��o do sem�foro para regi�o cr�tica
    sem_init(&empty, 0, Buffer_Size);//cria��o do sem�foro para verificar se � poss�vel adicionar itens ao buffer
    sem_init(&full, 0, 0);//cria��o do sem�foro para verificar se � poss�vel remover itens do buffer
    
    ret0 = pthread_create(&produtor, NULL, produzir, NULL);//cria a thread para um produtor
    ret1 = pthread_create(&consumidor, NULL, consumir, NULL);//cria a threa para um consumidor
    
    pthread_join(produtor, NULL);//faz o main esperar a execu��o da thread do produtor
    pthread_join(consumidor, NULL);//faz o main esperar a execu��o da thread do produtor
    
	return 0;
}
