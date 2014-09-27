int main() {
	int N = 5;
	int T[N][N];
	int i,j;

	for(i = 1 ; i < N ; ++i)
		for(j = 1 ; j < N ; ++j)
			T[i][j] = T[i - 1 ][ j ] + T [ i ] [ j - 1] ;
	return 0;
}
