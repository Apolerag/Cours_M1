#include <stdio.h>
#include <stdlib.h>
#include <time.h> 

const int N=100;

int rand_a_b(int a, int b){
    return rand()%(b-a) +a;
}


int main(){
	
	int **t = malloc(N*sizeof(int*));
	
	int nombre_aleatoire;
	int i,j,k;
	int min,temp;
	
	srand(time(NULL));
	for(i=0; i<N; i++){
		t[i] = malloc(N*sizeof(int));
		for(j = 0; j < N; j++){
			nombre_aleatoire = rand_a_b(1, 150*N);
			t[i][j] = nombre_aleatoire;
		}
	}
	
	for(i =0; i < N; i++){		
		for(j = 0; j < N; j++){
			min = -1;
		
			for(k = j; k < N; k++){		
				if(min == -1) min = k;
				if(t[i][k] < t[i][min]) min = k;
			}
			if(j != min){
				temp = t[i][j] ;
				t[i][j]  = t[i][min] ;
				t[i][min]  = temp;
			}
		} 
	}
	
	for(i=0; i<N; i++){
		free(t[i]);
	}
	
	free(t);
	return 0;
}
