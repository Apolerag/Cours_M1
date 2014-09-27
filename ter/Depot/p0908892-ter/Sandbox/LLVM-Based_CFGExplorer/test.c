void forstmt(int tab[]) {
	for (int i = 0; i < 0; ++i) {
		tab[i] = 0;
	}    
}

int f(int n, int *nptr) {
	return 1;
}

int main(int argc, char **argv) {
	int *nptr, *nptr2 = &argc;
	int tab[2] = { *nptr2, 1 };
	int (*fptr)(int, int*);

	fptr = &f;

	nptr = tab;

	tab[0] = 2;
	tab[*nptr2] = nptr[0];

	argc = *(nptr + (argc = **argv));

	f(*nptr, nptr + 1);

	fptr(0, &argc);

	return 0;
}
