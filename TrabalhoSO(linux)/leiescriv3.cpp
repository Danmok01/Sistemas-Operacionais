#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <locale.h> //para lingua portuguesa

#include <unistd.h>

#define LEITOR 22
#define ESCRITOR 5

sem_t db;
sem_t mutex;
//pthread_mutex_t naosei;
int leitor_lendo;
int total_que_leram;


void lerBaseDeDados(void);
void usarBancoDeDados(void);
void pensandoDados(void);
void escreverBancoDados(void);

void * leitor(void*  quant)
{

    int * id = (int*) (quant) ;
    
    printf("O leitor %d foi criado com sucesso\n", *(id) );

    while(1)
    {
        sem_wait(&mutex);
        leitor_lendo=leitor_lendo +1;

        if(leitor_lendo == 1 )
        {
            sem_wait(&db);
        }

        sem_post(&mutex);

        lerBaseDeDados();
        sem_wait(&mutex);
        leitor_lendo=leitor_lendo -1;

        if(leitor_lendo == 0)
        {
            sem_post(&db);
        }

        sem_post(&mutex);

        usarBancoDeDados();

    }
}

void * escritor(void*)
{
    while (1)
    {
        pensandoDados();
        sem_wait(&db);
        escreverBancoDados();
        
        sem_post(&db);

    }
    
}







int main()
{
    pthread_t threadsdosescritores[ESCRITOR], threadsdosleitores[LEITOR];
    int i;

    sem_init (&mutex, 0,1);
    sem_init (&db, 0,1);

    //pthread_mutex_init(&db,NULL);
    //pthread_mutex_init(&mutex,NULL);
    //pthread_mutex_init(&naosei,NULL);

    for(i=0 ; i<ESCRITOR ; i++)
    {
        pthread_create(&threadsdosescritores[i],NULL,escritor ,NULL);
    }

    for(i=0 ; i<LEITOR ; i++)
    {
        int calma;
        calma = rand() % 2;
        pthread_create(&threadsdosleitores[i],NULL,leitor ,(void*)&(i));
        sleep(calma);
    }

    for(i=0 ; i<ESCRITOR ; i++)
    {
        pthread_join(threadsdosescritores[i],NULL);
    }

    for(i=0 ; i<LEITOR ; i++)
    {
        pthread_join(threadsdosleitores[i],NULL);
    }

    return(0);

}



void lerBaseDeDados()
{
    int tempo_de_leitura;
    tempo_de_leitura = rand() % 5;

    printf("Lendo o Banco de dados. Um total de %d.\n",leitor_lendo);
    sleep(tempo_de_leitura);
}

void usarBancoDeDados()
{
    int tempo_de_uso;
    tempo_de_uso= rand() % 5;

    printf("Usando o conhecimento.\n");
    sleep(tempo_de_uso);

}

void pensandoDados()
{
    int tempo_para_pensar;
    tempo_para_pensar = rand() % 10;

    printf("Pensando em que vai escrever.\n");
    sleep(tempo_para_pensar);
}

void escreverBancoDados()
{
    int tempo_de_escrita;
    tempo_de_escrita = rand() % 10;

    printf("Escrevendo no banco de dados.\n");
    sleep(tempo_de_escrita);
}