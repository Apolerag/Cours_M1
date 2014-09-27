#include <stdio.h>
#include <stdlib.h>
#include <time.h> 

int main(){	
	int *t = malloc(100*sizeof(int));
	int i,j;
	int min,temp;
	
	srand(time(NULL));
	for(i=0; i < 100; i++){
		nombre_aleatoire = rand_a_b(1, N);
		t[i] = rand()%(b-a) +a;
	}
	
	for(i =0; i < 100; i++){
		min = i;		
		for(j = i; j < 100; j++){
			if(t[j] < t[min]) min = j;	
		} 
		temp = t[i] ;
		t[i]  = t[min] ;
		t[min]  = temp;
	}
	free(t);
	return 0;
}
