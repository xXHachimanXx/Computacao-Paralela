#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

int main()
{
  srand(time(NULL));

  #pragma omp parallel 
  {
    int id = omp_get_thread_num();
    int numero_secreto = rand() % 20; 

    // #pragma omp single // apenas uma thread executa barrando as outras até o fim
	// #pragma omp single nowait // mesma ideia porem sem a barreira
	// #pragma omp master // mesma ideia da acima mas apenas a thread 0 executa
    printf("Vamos revelar os números secretos!\n");   
	// #pragma omp barrier // a execução só continua quando todas as threads chegarem na barreira
	
    printf("Thread %d escolheu o número %d\n",id,numero_secreto);   

  }
}
