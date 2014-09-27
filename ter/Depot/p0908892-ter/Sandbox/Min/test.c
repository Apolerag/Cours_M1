static int global_var1;
static int global_var2;

int f(int x) {
	return (x + x);
}

int *funcWithoutBody();

int main(int argc, char *argv[]) {
	int a;
	int b;

	a = 0;
	b = a + 1;

	while (a < b) {
		if (b % 2 == 0) {
			b++;
		}
		else {
			a = f(a);
		}
	}

	return 0;
}
