#include<bits/stdc++.h>
#include "generator.h"
using namespace std;
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

int minimo(int a,int b)
{
	if(a<b)
	{
		return a;
	}
	else{
		return b;
	}
}


int** multiplicacion(int **A,int **B,int n,int Blocktam){
	int **C;
	crearMatriz2d(C,n);
	LLenarCon0(C,n);
	for(int i=0;i<n;i+=Blocktam){
		for(int j=0;j<n;j+=Blocktam){
			for(int k=0;k<n;k+=Blocktam){
				for(int l=0;l<Blocktam;l++){
					for(int m=0;m<Blocktam;m++){
						for(int p=0;p<Blocktam;p++){
							C[i+l][j+m]+=A[i+l][k+p]*B[k+p][j+m];
//							*(*(C+(i+l))+(j+m))+=*(*(A+(i+l))+(k+p)) * (*(*(B+(k+p))+(j+m)));
						}
					}
				}
			}
		}
	}
	return C;
}

int main()
{
	ofstream fs("multiplicacion6blocks.txt");
	int n=100;
	clock_t t0;
	srand(time(NULL));
	int **A, **B, **D;
	int BlockTam=50,l=0;
		
	
	for(int i=n;i<=3000;i=i+300){
		crearMatriz2d(A,i);
		leer(A,i,l);
//		llenar(A,i);
		crearMatriz2d(B,i);
		leer(B,i,l);
//		llenar(B,i);
		crearMatriz2d(D,i);
		LLenarCon0(D,i);
		l++;
		t0=clock();
//		D=product6loop(A,B,i,BlockTam);
		D=multiplicacion(A,B,i,BlockTam);
		t0=clock()-t0;
		cout<<((float)t0)/CLOCKS_PER_SEC<<endl;
//		mostrar(D,n);

	fs<<((float)t0)/CLOCKS_PER_SEC<<endl;
	}
	
}
