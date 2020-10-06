#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include "timer.h"

const int MAX_THREADS = 1024;

long thread_count;
long long n;
int flag;
double sum;

void* Thread_sum(void* rank) {
   long my_rank = (long) rank;
   double factor;
   long long i;
   long long my_n = n/thread_count;
   long long my_first_i = my_n*my_rank;
   long long my_last_i = my_first_i + my_n;

   if (my_first_i % 2 == 0)
      factor = 1.0;
   else
      factor = -1.0;

   for (i = my_first_i; i < my_last_i; i++, factor = -factor) {
      while (flag != my_rank);
      sum += factor/(2*i+1);  
      flag = (flag+1) % thread_count;
   }

   return NULL;
} 

void ModoUso(char* prog_name) {
   fprintf(stderr, "Formato para usar: %s ||n° de threads|| ||n>1||\n", prog_name);
   exit(0);
} 

void Get_args(int argc, char* argv[]) {
   if (argc != 3) ModoUso(argv[0]);
   thread_count = strtol(argv[1], NULL, 10);  
   if (thread_count <= 0 || thread_count > MAX_THREADS) ModoUso(argv[0]);
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
   long       thread;  
   pthread_t* Handthread;
   double start, finish, time;

   Get_args(argc, argv);

   Handthread = (pthread_t*) malloc (thread_count*sizeof(pthread_t)); 
   
   GET_TIME(start);
   sum = 0.0;
   flag = 0;
   for (thread = 0; thread < thread_count; thread++)  
      pthread_create(&Handthread[thread], NULL,
          Thread_sum, (void*)thread);  

   for (thread = 0; thread < thread_count; thread++) 
      pthread_join(Handthread[thread], NULL); 
   GET_TIME(finish);
   time = finish - start;

   sum = 4.0*sum;
   printf("El tiempo multiHilado es %f seconds\n", time);

   GET_TIME(start);
   sum = Serial_pi(n);
   GET_TIME(finish);
   time = finish - start;
   printf("El tiempo en serie es %f seconds\n", time);

   
   free(Handthread);
   return 0;
}

