int main(int argc, char *argv[]) {
	int a;
	int b;

	a = 0;

	int *c = &a;

	*c=1;
	a=1;

	*c=2;
	a=2;

	return 0;
}
