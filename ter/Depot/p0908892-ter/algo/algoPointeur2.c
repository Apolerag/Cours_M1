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
	printf("initialisation");
	for(i=0; i<N; i++){
		//printf("i = %d\n",i);
		*(t+i) = malloc(N*sizeof(int));
		//printf("%d\n",(t+i));
		//printf("malloc\n");
		for(j = 0; j < N; j++){
		//printf("j = %d\n",j);
			nombre_aleatoire = rand_a_b(1, N);
			
			//printf("%d, j = %d",*(t+i)+j,j);
			*(*(t+i)+j) = nombre_aleatoire;
			
			//printf(" t[i][j] = %d\n",*(*(t+i)+j));
		}
	}
	
	
	printf("affichage\n");
	for(i=0; i<N; i++){
		for(j = 0; j < N; j++){
			printf("%d ",*(*(t+i)+j));
		//	printf("%d ",*(t[i/sizeof(int)]+j));		
		//	printf("%d\n",t[i/sizeof(int)][j/sizeof(int)]);
		}
		printf("\n");
	}
	
	printf("tri\n");
	for(i=0; i<N; i++){
		for(j = 0; j < N; j++){
		
			//printf("j = %d\n",k);
			min = j;
			for(k = j; k < N; k++){		
				//printf("i = %d, j = %d, k = %d\n",i,j,k);				
				//printf("t[i][k] = %d ",t[i][k]);		
				//printf("t[i][min] = %d\n",t[i][min]);
				if(*(*(t+i)+k) < *(*(t+i)+min)) min = k;
			}
			//printf("finFork\n");
			if(j != min){
				temp = *(*(t+i)+j) ;
				*(*(t+i)+j)  = *(*(t+i)+min) ;
				*(*(t+i)+min)  = temp;
			}
		} 
	}
	
	printf("affichage\n");
	for(i=0; i<N; i++){
		for(j = 0; j < N; j++){
			printf("%d ",*(*(t+i)+j));
		//	printf("%d ",*(t[i/sizeof(int)]+j));		
		//	printf("%d\n",t[i/sizeof(int)][j/sizeof(int)]);
		}
		printf("\n");
	}
	printf("free boucle\n");
	for(i=0; i<=N-1; i++){
		free(t[i]);
	}
	printf("free\n");
	free(t);
	return 0;
} 
