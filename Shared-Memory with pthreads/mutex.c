#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

#include "timer.h"

const int MAX_THREADS = 1024;

long contThreads;
long long n;
double sum;
pthread_mutex_t mutex;

void* Thread_sum(void* rank) {
   long my_rank = (long) rank;
   double factor;
   long long i;
   long long my_n = n/contThreads;
   long long my_first_i = my_n*my_rank;
   long long my_last_i = my_first_i + my_n;
   double my_sum = 0.0;

   if (my_first_i % 2 == 0)
      factor = 1.0;
   else
      factor = -1.0;

   for (i = my_first_i; i < my_last_i; i++, factor = -factor) {
      my_sum += factor/(2*i+1);
   }
   pthread_mutex_lock(&mutex);
   sum += my_sum;
   pthread_mutex_unlock(&mutex);

   return NULL;
}


void ModoUso(char* prog_name) {
   fprintf(stderr, "Formato para usar: %s ||n° de threads|| ||n>1||\n", prog_name);

   exit(0);
}
void Get_args(int argc, char* argv[]) {
   if (argc != 3) ModoUso(argv[0]);
   contThreads = strtol(argv[1], NULL, 10);  
   if (contThreads <= 0 || contThreads > MAX_THREADS) ModoUso(argv[0]);
   n = strtoll(argv[2], NULL, 10);
   if (n <= 0) ModoUso(argv[0]);
}

double Serial_pi(long long n) {
   double sum = 0.0;
   long long i;
   double factor = 1.0;

   for (i = 0; i < n; i++, factor = -factor) {
      sum += factor/(2*i+1);
   }
   return 4.0*sum;

}

int main(int argc, char* argv[]) {
   long thread;  
   pthread_t* Handthread;
   double start, finish, time;

   Get_args(argc, argv);

   Handthread = (pthread_t*) malloc (contThreads*sizeof(pthread_t)); 
   pthread_mutex_init(&mutex, NULL);
   sum = 0.0;

   GET_TIME(start);
   for (thread = 0; thread < contThreads; thread++)  
      pthread_create(&Handthread[thread], NULL,
          Thread_sum, (void*)thread);  

   for (thread = 0; thread < contThreads; thread++) 
      pthread_join(Handthread[thread], NULL); 
   GET_TIME(finish);
   time = finish - start;

   sum = 4.0*sum;

   printf("El tiempo MultiHilado %f seconds\n", time);
   GET_TIME(start);
   sum = Serial_pi(n);
   GET_TIME(finish);
   time = finish - start;
   printf("El tiempo en serie es %f seconds\n", time);
   
   pthread_mutex_destroy(&mutex);
   free(Handthread);
   return 0;
}


