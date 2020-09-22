#include <bits/stdc++.h>
#include "generator.h"
#include<math.h>
using namespace std;



//Matrices 
void LLenarCon0(int **A,int n)
{
	for(int i=0;i<n;i++){
    	for(int j=0;j<n;j++){
			A[i][j]=0;
		}
	}
}

void crearMatriz2d(int **&M,int f)
{
	int c=f;
	M=new int *[f];
	for(int i=0;i<f;i++)
		*(M+i)=new int[c];
}
void llenar(int **M,int f)
{
	int c=f;
	for(int i=0;i<f;i++)
		for(int j=0;j<c;j++)
			*(*(M+i)+j)=rand()%100;
}
void mostrar(int **M,int f)
{
	cout<<"Matriz generada"<<endl;
	int c=f;
	for(int i=0;i<f;i++){
		for(int j=0;j<c;j++){
			cout<<*(*(M+i)+j)<<" ";	
		}
			cout<<endl;
	}
}



int** multplicacion(int **A, int **B, int n)
{
	int **C;
	crearMatriz2d(C,n);
	LLenarCon0(C,n);
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
		{
			for(int k=0;k<n;k++){
				*(*(C+i)+j) += *(*(A+i)+k) *  *(*(B+k)+j);

			}
		}
	}
	return C;
}



int main(){

	int n=100;
	clock_t t0;
	srand(time(NULL));
	int **A, **B, **D;
	int l=0;
	ofstream fs("Clasica.txt");
	
	for(int i=n;i<=3000;i=i+300){
		Generar(i,l);
		crearMatriz2d(A,i);
		leer(A,i,l);
//		llenar(A,i);
		crearMatriz2d(B,i);
		leer(B,i,l);
//		llenar(B,i);
		crearMatriz2d(D,i);
		LLenarCon0(D,i);
		l=l+1;
		
		t0=clock();
		D=multplicacion(A,B,i);
		t0=clock()-t0;
		cout<<((float)t0)/CLOCKS_PER_SEC<<endl;
//		mostrar(D,n);
		fs<<((float)t0)/CLOCKS_PER_SEC<<endl;
	}
	return 0;
}
