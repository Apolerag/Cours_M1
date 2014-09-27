int main() {
int N = 5;
int **T = malloc(N*sizeof(int*));
int i,j;
for(i=0; i< N; i++)
  T[i] = malloc(N*sizeof(int));

for(i = 1 ; i < N ; ++i)
  for(j = 1 ; j < N ; ++j)
    T[i][j] = T[i - 1][j] + T [i] [j - 1] ;
return 0;
}
