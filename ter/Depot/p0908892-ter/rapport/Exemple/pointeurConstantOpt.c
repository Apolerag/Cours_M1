int main(){
	int a = 1, b = a + 1;

	while (a < b) {
		if (b % 2 == 0) 
			b++;
		else a  *= 2;
	}
	return 0;	
}

