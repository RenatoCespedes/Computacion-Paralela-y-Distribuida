#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "timer.h"
//variables  ausar para el generador aleatorio
#define multiplicador 279470273 
#define modulo 4294967291U
#define divisor ((double) 4294967291U)


const int MAX_KEY = 100000000;


struct Node {
   int    data;
   struct Node* next;
};

//GENERADOR DE ALEATORIOS
unsigned randU(unsigned* seed_p) {
   long long z = *seed_p;
   z *= multiplicador; 
   z %= modulo;
   *seed_p = z;
   return *seed_p;
}


double randD(unsigned* seed_p) {
   unsigned x = randU(seed_p);
   double y = x/divisor;
   return y;
}

//VARIABLES A USAR
struct      Node* head = NULL;  
int         cont_thread;
int         total_ops;
double      cont_insertar;
double      cont_search;
double      cont_borrar;
pthread_mutex_t mutex;
pthread_mutex_t count_mutex;
int         total_miembros=0;
int total_insertar=0;
int total_borrar=0;


int insertar(int value) {
   struct Node* actual = head;
   struct Node* anterior = NULL;
   struct Node* temporal;
   int Fb = 1;
   
   while (actual != NULL && actual->data < value) {
      anterior = actual;
      actual = actual->next;
   }

   if (actual == NULL || actual->data > value) {
      temporal = malloc(sizeof(struct Node));
      temporal->data = value;
      temporal->next = actual;
      if (anterior == NULL)
         head = temporal;
      else
         anterior->next = temporal;
   } else {
      Fb = 0;
   }

   return Fb;
}
int  Member(int value) {
   struct Node* temporal;

   temporal = head;
   while (temporal != NULL && temporal->data < value)
      temporal = temporal->next;

   if (temporal == NULL || temporal->data > value) {
      return 0;
   } else {
      return 1;
   }
} 


void mostrar(void) {
   struct Node* temporal;
   temporal = head;
   while (temporal != (struct Node*) NULL) {
      printf("%d ", temporal->data);
      temporal = temporal->next;
   }
   printf("\n");
}  


int eliminar(int value) {
   struct Node* actual = head;
   struct Node* anterior = NULL;
   int Fb = 1;

   while (actual != NULL && actual->data < value) {
      anterior = actual;
      actual = actual->next;
   }
   
   if (actual != NULL && actual->data == value) {
      if (anterior == NULL) { 
         head = actual->next;
         free(actual);
      } else { 
         anterior->next = actual->next;
         free(actual);
      }
   } else { 
      Fb = 0;
   }

   return Fb;
} 


int  isEmpty(void) {
   if (head == NULL)
      return 1;
   else
      return 0;
}  
void liberarList(void) {
   struct Node* actual;
   struct Node* siguiente;

   if (isEmpty()) return;
   actual = head; 
   siguiente = actual->next;
   while (siguiente != NULL) {
      free(actual);
      actual = siguiente;
      siguiente = actual->next;
   }

   free(actual);
} 


void* Thread_work(void* rank) {
   long my_rank = (long) rank;
   int i, val;
   double which_op;
   unsigned seed = my_rank + 1;
   int my_member=0, my_insertar=0, my_eliminar=0;
   int ops_per_thread = total_ops/cont_thread;

   for (i = 0; i < ops_per_thread; i++) {
      which_op = randD(&seed);
      val = randU(&seed) % MAX_KEY;
      if (which_op < cont_search) {
         pthread_mutex_lock(&mutex);
         Member(val);
         pthread_mutex_unlock(&mutex);
         my_member++;
      } else if (which_op < cont_search + cont_insertar) {
         pthread_mutex_lock(&mutex);
         insertar(val);
         pthread_mutex_unlock(&mutex);
         my_insertar++;
      } else { 
         pthread_mutex_lock(&mutex);
         eliminar(val);
         pthread_mutex_unlock(&mutex);
         my_eliminar++;
      }
   }  

   pthread_mutex_lock(&count_mutex);
   total_miembros += my_member;
   total_insertar += my_insertar;
   total_borrar += my_eliminar;
   pthread_mutex_unlock(&count_mutex);

   return NULL;
}  


void input(int* inserts_in_main_p) {

   printf("Numero de keys a insertar\n");
   scanf("%d", inserts_in_main_p);
   printf("Numero de operaciones\n");
   scanf("%d", &total_ops);
   printf("Porcentaje de miembros (ingresar entre 0 y 1)\n");
   scanf("%lf", &cont_search);
   printf("Porcentaje de insertadas (ingresar entre 0 y 1)\n");
   scanf("%lf", &cont_insertar);
   printf("Porcentaje de eliminacion (ingresar entre o y 1)\n");
   scanf("%lf", &cont_borrar);

} 

int main(int argc, char* argv[]) {
   long i; 
   int key, success, attemporalts;
   pthread_t* thread_handles;
   int x;
   unsigned seed = 1;
   double start, finish;


   cont_thread = strtol(argv[1],NULL,10);

   input(&x);

   i = attemporalts = 0;
   while ( i < x && attemporalts < 2*x ) {
      key = randU(&seed) % MAX_KEY;
      success = insertar(key);
      attemporalts++;
      if (success) i++;
   }
   



   thread_handles = malloc(cont_thread*sizeof(pthread_t));
   pthread_mutex_init(&mutex, NULL);
   pthread_mutex_init(&count_mutex, NULL);

   GET_TIME(start);
   for (i = 0; i < cont_thread; i++)
      pthread_create(&thread_handles[i], NULL, Thread_work, (void*) i);

   for (i = 0; i < cont_thread; i++)
      pthread_join(thread_handles[i], NULL);
   GET_TIME(finish);
   printf("Elapsed time = %f seconds\n", finish - start);
//   printf("Total ops = %d\n", total_ops);
//   printf("member ops = %d\n", total_miembros);
//   printf("insertar ops = %d\n", total_insertar);
//   printf("eliminar ops = %d\n", total_borrar);


   liberarList();
   pthread_mutex_destroy(&mutex);
   pthread_mutex_destroy(&count_mutex);
   free(thread_handles);

   return 0;
}  
