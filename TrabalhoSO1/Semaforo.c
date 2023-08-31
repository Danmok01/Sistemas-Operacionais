
#include <stdio.h>
#include <windows.h>

HANDLE s1 = 0;


void thread_01( void )
{
    printf( "thread_01 iniciando...\n" );

    DWORD rc = WaitForSingleObject( s1, 0 );

    switch( rc )
    {
        case WAIT_OBJECT_0:
            for( int i = 0; i < 5; ++i )
            {
                printf( "thread_01 %d...\n", i );
                Sleep( 1000 );

                if ( i == 3 )
                {
                    int anterior = -1;
                    ReleaseSemaphore( s1, 1, &anterior );
                    printf( "liberando semaforo para thread_02\n" );
                }
            }

            int anterior = -1;
            ReleaseSemaphore( s1, 1, &anterior );
            printf( "thread_01 release %d\n", anterior );
            break;

        case WAIT_TIMEOUT:
            printf( "thread_01 erro\n" );
            break;
    }

    printf( "thread_01 FIM\n" );
}

void thread_02( void )
{
    printf( "thread_02 iniciando...\n" );

    DWORD rc = WAIT_TIMEOUT;

    while( rc == WAIT_TIMEOUT )
    {
        rc = WaitForSingleObject( s1, 0 );
        printf( "tentando...\n" );
        Sleep( 500 );
    }

    switch( rc )
    {
        case WAIT_OBJECT_0:
            for( int i = 0; i < 5; ++i )
            {
                printf( "thread_02 %d...\n", i );
                Sleep( 1000 );
            }

            int anterior = -1;
            ReleaseSemaphore( s1, 1, &anterior );
            printf( "thread_02 release %d\n", anterior );
            break;

        case WAIT_TIMEOUT:
            printf( "thread_02 erro\n" );
            break;
    }

    printf( "thread_02 FIM\n" );
}

int main()
{
    s1 = CreateSemaphore( 0, 1, 2, 0 );

    HANDLE thread1 = CreateThread( NULL, 0, thread_01, 0, 0, 0 );

    Sleep( 500 );
    HANDLE thread2 = CreateThread( NULL, 0, thread_02, 0, 0, 0 );

    if ( !thread1 || !thread2 )
    {
        printf( "erro ao criar thread.\n" );
        return 1;
    }

    WaitForSingleObject( thread1, INFINITE );
    WaitForSingleObject( thread2, INFINITE );

    CloseHandle( s1 );

    printf("\n\n");
    return 0;
}
