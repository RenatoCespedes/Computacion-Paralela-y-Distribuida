#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "timer.h"
//variables  ausar para el generador aleatorio
#define multiplicador 279470273 
#define modulo 4294967291U
#define divisor ((double) 4294967291U)

const int MAX_KEY = 100000000;


const int IN_LIST = 1;
const int EMPTY_LIST = -1;
const int END_OF_LIST = 0;

struct Node {
	int data;
	pthread_mutex_t mutex;
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
struct Node* head=NULL;
pthread_mutex_t head_mutex;
int cont_thread;
int total_ops;
double cont_insert;
double cont_search;
double cont_borrar;
pthread_mutex_t cont_mutex;
int total_miembros=0;
int total_insert=0;
int total_borrar=0;

void inicializar(struct Node** actualPp,struct Node** antesPp){
	*antesPp=NULL;
	pthread_mutex_lock(&head_mutex);
	*actualPp=head;
	if (*actualPp!=NULL)
	{
		pthread_mutex_lock(&((*actualPp)->mutex));
		
	}
}
int recorrer(struct Node** actual1, struct Node** anterior1){
	int Fb=IN_LIST;
	struct Node* actual=*actual1;
	struct Node* anterior=*anterior1; 
	
	if(actual==NULL){
		if(anterior==NULL)
		{
			pthread_mutex_unlock(&head_mutex);
			return EMPTY_LIST;
		}
		else{
			return END_OF_LIST;
		}
	}
	else{
		if(actual->next!=NULL){
			pthread_mutex_lock(&(actual->next->mutex));
		}
		else{
			Fb=END_OF_LIST;
		}
		if(anterior!=NULL){
			pthread_mutex_unlock(&(anterior->mutex));
		}
		else{
			pthread_mutex_unlock(&head_mutex);
		}
		*anterior1 = actual;
		*actual1=actual->next;
		return Fb;
	}
}
int insertar(int valor){
struct Node* actual;
struct Node* anterior;
struct Node* temporal;
int ind=1;

inicializar(&actual,&anterior);

while(actual!=NULL && actual->data < valor){
	recorrer(&actual,&anterior);
}

if(actual==NULL || actual->data > valor){
	temporal= malloc(sizeof(struct Node));
	pthread_mutex_init(&(temporal->mutex),NULL);
	temporal->data=valor;
	temporal->next=actual;
	if(actual!=NULL){
		pthread_mutex_unlock(&(actual->mutex));
	}
	if(anterior==NULL){
		head=temporal;
		pthread_mutex_unlock(&head_mutex);
	}
	else{
		anterior->next=temporal;
		pthread_mutex_unlock(&(anterior->mutex));
	}
}
else{
	if(actual!=NULL){
		pthread_mutex_unlock(&(actual->mutex));
	}
	if(anterior!=NULL){
		pthread_mutex_unlock(&(anterior->mutex));
	}
	else{
		pthread_mutex_unlock(&head_mutex);
	}
	ind=0;
}
return ind;
}

int Member(int dato){
	struct Node *temp, *temp0;
	
	pthread_mutex_lock(&head_mutex);
	temp=head;
	if(temp!=NULL){
		pthread_mutex_lock(&(temp->mutex));
	}
	pthread_mutex_unlock(&head_mutex);
	while(temp!=NULL && temp->data < dato){
		if(temp->next!=NULL){
			pthread_mutex_lock(&(temp->next->mutex));
		}
		temp0=temp;
		temp=temp->next;
		pthread_mutex_unlock(&(temp0->mutex));
	}
	
	if(temp==NULL || temp->data > dato){
		if(temp!=NULL){
			pthread_mutex_unlock(&(temp->mutex));
		}
		return 0;
	}
	else{
		if(temp==head){
			pthread_mutex_unlock(&head_mutex);
		}
		pthread_mutex_unlock(&(temp->mutex));
		return 1;
	}
}

void mostrar(void){
	struct Node *temporal;
	
	temporal=head;
	
	while(temporal!=(struct Node*)NULL){
		printf("%d \t",temporal->data);
		temporal=temporal->next;
	}
	printf("\n");
}

int eliminar(int valor){
	struct Node* actual;
	struct Node* anterior;
	
	int Fb=1;
	
	inicializar(&actual,&anterior);
	
	while(actual!=NULL && actual->data < valor){
		recorrer(&actual,&anterior);
	}
	
	if(actual!=NULL && actual->data < valor){
		if(anterior ==NULL){
			head= actual->next;
			pthread_mutex_unlock(&head_mutex);
			pthread_mutex_unlock(&(actual->mutex ));
			pthread_mutex_destroy(&(actual->mutex));
			free(actual);
		}
		else{
			anterior->next=actual->next;
			pthread_mutex_unlock(&(anterior->mutex));
			
			pthread_mutex_unlock(&(actual->mutex));
			pthread_mutex_destroy(&(actual->mutex));
			free(actual);
		}
	}
	else{
		if(anterior!=NULL){
			pthread_mutex_unlock(&(anterior->mutex));
		}
		if(actual!=NULL){
			pthread_mutex_unlock(&(actual->mutex));
		}
		if(actual==head){
			pthread_mutex_unlock(&head_mutex);
		}
		Fb=0;
	}
	return Fb;


}
int isEmpty(){
	if(head==NULL){
		return 1;
	}
	else{
		return 0;
	}
}

void liberarList(void){
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
   int my_member=0, my_insert=0, my_delete=0;
   int ops_per_thread = total_ops/cont_thread;

   for (i = 0; i < ops_per_thread; i++) {
      which_op = randD(&seed);
      val = randU(&seed) % MAX_KEY;

      if (which_op < cont_search) {

         Member(val);
         my_member++;
      } else if (which_op < cont_search + cont_insert) {

         insertar(val);
         my_insert++;
      } else {

         eliminar(val);
         my_delete++;
      }
   }
   pthread_mutex_lock(&cont_mutex);
   total_miembros += my_member;
   total_insert += my_insert;
   total_borrar += my_delete;
   pthread_mutex_unlock(&cont_mutex);
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
   scanf("%lf", &cont_insert);
   printf("Porcentaje de eliminacion (ingresar entre o y 1)\n");
   scanf("%lf", &cont_borrar);

} 

int main(int argc, char* argv[]){
   long i; 
   int key, success, attempts;
   pthread_t* thread_handles;
   int x;
   unsigned seed = 1;
   double start, finish;


   cont_thread = strtol(argv[1], NULL, 10);

   input(&x);

                       
   i = attempts = 0;
   pthread_mutex_init(&head_mutex, NULL);
   while ( i < x && attempts < 2*x ) {
      key = randU(&seed) % MAX_KEY;
      success = insertar(key);
      attempts++;
      if (success) i++;
   }

   thread_handles = malloc(cont_thread*sizeof(pthread_t));
   pthread_mutex_init(&cont_mutex, NULL);

   GET_TIME(start);
   for (i = 0; i < cont_thread; i++)
      pthread_create(&thread_handles[i], NULL, Thread_work, (void*) i);

   for (i = 0; i < cont_thread; i++)
      pthread_join(thread_handles[i], NULL);
   GET_TIME(finish);
//   mostrar();
   printf("tiempo obtenido = %f seconds\n", finish - start);
//   printf("Total ops = %d\n", total_ops);
//   printf("member ops = %d\n", total_miembros);
//   printf("insert ops = %d\n", total_insert);
//   printf("delete ops = %d\n", total_borrar);



   liberarList();
   pthread_mutex_destroy(&head_mutex);
   pthread_mutex_destroy(&cont_mutex);
   free(thread_handles);

   return 0;
}
