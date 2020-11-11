#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#define random rand
#define srandom srand

const int RMAX = 10000000;

int thread_count;

void Argumentos(char* prog_name) {
   fprintf(stderr, "Argumentos:   %s <thread count> <n> <g|i>\n", prog_name);
   fprintf(stderr, "   n:   numero de elementos en la lista\n");
   fprintf(stderr, "  'g':  generador de lista aleaotoria\n");
   fprintf(stderr, "  'i':  ingreso manual de los elementos\n");
}

//Obtencion de Argumentos
void Get_args(int argc, char* argv[], int* n_p, char* g_i_p) {
   if (argc != 4 ) {
      Argumentos(argv[0]);
      exit(0);
   }
   thread_count = strtol(argv[1], NULL, 10);
   *n_p = strtol(argv[2], NULL, 10);
   *g_i_p = argv[3][0];

   if (*n_p <= 0 || (*g_i_p != 'g' && *g_i_p != 'i') ) {
      Argumentos(argv[0]);
      exit(0);
   }
}

//Generar lista aleaotoria
void GenerarLista(int a[], int n) {
   int i;

   srandom(1);
   for (i = 0; i < n; i++)
      a[i] = random() % RMAX;
} 

//Mostrar lista
void Show(int a[], int n, char* title) {
   int i;

   printf("%s:\n", title);
   for (i = 0; i < n; i++)
      printf("%d ", a[i]);
   printf("\n\n");
}

//Leer lista insertada por teclado
void Read_list(int a[], int n) {
   int i;

   printf("Please enter the elements of the list\n");
   for (i = 0; i < n; i++)
      scanf("%d", &a[i]);
}


void Odd_even(int a[], int n) {
   int phase, i, tmp;

#  pragma omp parallel num_threads(thread_count) \
      default(none) shared(a, n) private(i, tmp, phase)
   for (phase = 0; phase < n; phase++) {
      if (phase % 2 == 0)
#        pragma omp for 
         for (i = 1; i < n; i += 2) {
            if (a[i-1] > a[i]) {
               tmp = a[i-1];
               a[i-1] = a[i];
               a[i] = tmp;
            }
         }
      else
#        pragma omp for 
         for (i = 1; i < n-1; i += 2) {
            if (a[i] > a[i+1]) {
               tmp = a[i+1];
               a[i+1] = a[i];
               a[i] = tmp;
            }
         }
   }
} 


int main(int argc, char* argv[]) {
   int  n;
   char g_i;
   int* a;
   double start, finish;

   Get_args(argc, argv, &n, &g_i);
   a = malloc(n*sizeof(int));
   if (g_i == 'g') {
      GenerarLista(a, n);

   } else {
      Read_list(a, n);
   }

   start = omp_get_wtime();
   Odd_even(a, n);
   finish = omp_get_wtime();
   
   printf("tiempo = %f seconds\n", finish - start);
   free(a);
   return 0;
}  


 

