#include <stdio.h>
#include <stdlib.h>
#include <time.h> 

const int N=100;

int rand_a_b(int a, int b){
    return rand()%(b-a) +a;
}


int main(){
	
	int *t = malloc(N*N*sizeof(int*));
	
	int nombre_aleatoire;
	int i,j,k;
	int min,temp;
	
	srand(time(NULL));
	for(i=0; i<N; i++){
		for(j = 0; j < N; j++){
			nombre_aleatoire = rand_a_b(1, 150*N);
			*(t + i*N + j) = nombre_aleatoire;
		}
	}
	
	for(i =0; i < N; i++){		
		for(j = 0; j < N; j++){
			min = -1;
		
			for(k = j; k < N; k++){		
				if(min == -1) min = k;
				if(*(t + i*N + k) < *(t + i*N + min)) min =k;
			}
			if(j != min){
				temp = *(t + i*N + j);
				*(t + i*N + j)  = *(t + i*N + min);
				*(t + i*N + min)  = temp;
			}
		} 
	}

	
	free(t);
	return 0;
}
