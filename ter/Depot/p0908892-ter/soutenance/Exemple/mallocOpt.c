#include <stdio.h>
#include <stdlib.h>
#include <time.h> 

int main(){	
int t[100];
int i,j;
int min,temp;
/*...*/
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
