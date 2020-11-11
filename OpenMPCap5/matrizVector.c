#include <stdio.h>
#include <stdlib.h>
#include <omp.h>



void ModoUso (char* prog_name) {
   fprintf(stderr, "ModoUso: %s <thread_count> <m> <n>\n", prog_name);
   exit(0);
}  

void Get_args(int argc, char* argv[], int* thread_count_p, 
      int* m_p, int* n_p)  {

   if (argc != 4) ModoUso(argv[0]);
   *thread_count_p = strtol(argv[1], NULL, 10);
   *m_p = strtol(argv[2], NULL, 10);
   *n_p = strtol(argv[3], NULL, 10);
   if (*thread_count_p <= 0 || *m_p <= 0 || *n_p <= 0) ModoUso(argv[0]);

} 

void Gen_matrix(double A[], int m, int n) {
   int i, j;
   for (i = 0; i < m; i++)
      for (j = 0; j < n; j++)
         A[i*n+j] = random()/((double) RAND_MAX);
}  


void Gen_vector(double x[], int n) {
   int i;
   for (i = 0; i < n; i++)
      x[i] = random()/((double) RAND_MAX);
}  


void Omp_mat_vect(double A[], double x[], double y[],
      int m, int n, int thread_count) {
   int i, j;
   double inicio, final, tiempo;

   inicio = omp_get_wtime();
#  pragma omp parallel for num_threads(thread_count) \
      default(none) private(i, j)  shared(A, x, y, m, n)
   for (i = 0; i < m; i++) {
      y[i] = 0.0;
      for (j = 0; j < n; j++)
         y[i] += A[i*n+j]*x[j];
   }
   final = omp_get_wtime();
   tiempo = final - inicio;
   printf("tiempo time = %f seconds\n", tiempo);

}

void Print_matrix( char* title, double A[], int m, int n) {
   int   i, j;

   printf("%s\n", title);
   for (i = 0; i < m; i++) {
      for (j = 0; j < n; j++)
         printf("%4.1f ", A[i*n + j]);
      printf("\n");
   }
}  


void Print_vector(char* title, double y[], double m) {
   int   i;

   printf("%s\n", title);
   for (i = 0; i < m; i++)
      printf("%4.1f ", y[i]);
   printf("\n");
}
int main(int argc, char* argv[]) {
   int     thread_count;
   int     m, n;
   double* A;
   double* x;
   double* y;

   Get_args(argc, argv, &thread_count, &m, &n);

   A = malloc(m*n*sizeof(double));
   x = malloc(n*sizeof(double));
   y = malloc(m*sizeof(double));
   
   Omp_mat_vect(A, x, y, m, n, thread_count);

   free(A);
   free(x);
   free(y);

   return 0;
} 