#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <locale.h> //para lingua portuguesa

#include <unistd.h>

#define LEITOR 2
#define ESCRITOR 5

pthread_mutex_t db;
pthread_mutex_t mutex;
int leitor_lendo;


void lerBaseDeDados(void);
void usarBancoDeDados(void);
void pensandoDados(void);
void escreverBancoDados(void);

void * leitor(void*  quant)
{

    int * id = (int*) (quant);
    printf("O leitor %d foi criado com sucesso\n", *(id) );

    while(1)
    {
        pthread_mutex_lock(&mutex);
        leitor_lendo=leitor_lendo +1;

        if(leitor_lendo == 1 )
        {
            pthread_mutex_lock(&db);
        }

        pthread_mutex_unlock(&mutex);

        lerBaseDeDados();
        pthread_mutex_lock(&mutex);
        leitor_lendo=leitor_lendo -1;

        if(leitor_lendo == 0)
        {
            pthread_mutex_unlock(&db);
        }

        pthread_mutex_unlock(&mutex);

        usarBancoDeDados();

    }
}

void * escritor(void*)
{
    while (1)
    {
        pensandoDados();
        pthread_mutex_lock(&db);
        escreverBancoDados();

        pthread_mutex_unlock(&db);

    }
    
}







int main()
{
    pthread_t threadsdosescritores[ESCRITOR], threadsdosleitores[LEITOR];
    int i;


    pthread_mutex_init(&db,NULL);
    pthread_mutex_init(&mutex,NULL);

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