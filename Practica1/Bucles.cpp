#include<bits/stdc++.h>
#include<time.h>
#define MAX 1000
using namespace std;
double D[MAX][MAX], x[MAX], y[MAX];

void FirstLoop(){
	for (int i = 0; i < MAX; i++){
		for (int j = 0; j < MAX; j++){
			y[i] += D[i][j]*x[j];
		}
	}
		
}
void SecondLoop(){
	for (int j = 0; j < MAX; j++)
	{
		for (int i = 0; i < MAX; i++){
			y[i] += D[i][j]*x[j];
		}
	}
}


int main(){
	srand(time(NULL));
	clock_t t0,t1,t2,t3;
	double time,time1;

	for(int i=0;i<MAX;i++){
		x[i]=rand()%100;
	}

	for(int i=0;i<MAX;i++)
	{
		for(int j=0;j<MAX;j++)
		{
			D[i][j]=rand()%100;
		}
	}
	
    t0=clock();
	FirstLoop();
	t0=clock() - t0;
	cout<<((float)t0)/CLOCKS_PER_SEC<<endl;

	cout<<endl;
	
	for(int i=0;i<MAX;i++)
	{
		y[i]=0;
	}
	t2=clock();
	SecondLoop();
	t2=clock()-t2;
	time1=((float)t2) /CLOCKS_PER_SEC;
	cout<<time1<<endl;
}
