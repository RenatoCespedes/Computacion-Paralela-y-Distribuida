#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>  

const int MAX_THREADS = 1024;
const int MSG_MAX = 100;


int thread_count;
char** messages;
sem_t* semaphores;

void ModoUso(char* prog_name) {

   fprintf(stderr, "ModoUso: %s ||numero de threads\n", prog_name);
   exit(0);
} 

void *Send_msg(void* rank) {
   long my_rank = (long) rank;
   long dest = (my_rank + 1) % thread_count;
   char* my_msg = (char*) malloc(MSG_MAX*sizeof(char));

   sprintf(my_msg, "Envio de mensaje de %ld a %ld", dest, my_rank);
   messages[dest] = my_msg;
   sem_post(&semaphores[dest]);  

   sem_wait(&semaphores[my_rank]);  
   printf("Thread %ld > %s\n", my_rank, messages[my_rank]);

   return NULL;
} 


int main(int argc, char* argv[]) {
   long       thread;
   pthread_t* thread_handles; 

   if (argc != 2) ModoUso(argv[0]);
   thread_count = strtol(argv[1], NULL, 10);
   if (thread_count <= 0 || thread_count > MAX_THREADS) ModoUso(argv[0]);

   thread_handles = malloc (thread_count*sizeof(pthread_t));
   messages = malloc(thread_count*sizeof(char*));
   semaphores = malloc(thread_count*sizeof(sem_t));
   for (thread = 0; thread < thread_count; thread++) {
      messages[thread] = NULL;

      sem_init(&semaphores[thread], 0, 0);
   }

   for (thread = 0; thread < thread_count; thread++)
      pthread_create(&thread_handles[thread], (pthread_attr_t*) NULL,
          Send_msg, (void*) thread);

   for (thread = 0; thread < thread_count; thread++) {
      pthread_join(thread_handles[thread], NULL);
   }

   for (thread = 0; thread < thread_count; thread++) {
      free(messages[thread]);
      sem_destroy(&semaphores[thread]);
   }
   free(messages);
   free(semaphores);
   free(thread_handles);

   return 0;
} 




