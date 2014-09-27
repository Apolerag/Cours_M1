int main(){
	
	int a = 1, b = a + 1;
	int *p = a;

	while (*p < b) {
		if (b % 2 == 0) 
			b++;
		else *p  *= 2;
	}
	return 0;	
}

