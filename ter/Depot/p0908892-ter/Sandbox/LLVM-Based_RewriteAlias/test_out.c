#define L_AND(a, b) a && b
#define L_OR(a, b) a || b
#define L_SUP(a, b) a > b
#define L_INF(a, b) a < b
#define L_SUPEQ(a, b) a >= b
#define L_INFEQ(a, b) a <= b
#define L_DIF(a, b) a != b
#define L_EQ(a, b) a == b

#define NULL 0

/* FUNCTION: main
 * returns: NULL TYPE
 */
int main()
{
	int *p0_1, *p0_2, *p0_3 = p0_2;
	int *p1_1, *p1_2, *p1_3 = p1_1;
	int n;

	/*Ptr*/p0_2 = p0_1/*ptr*/;
	/*not ptr*/p0_1 = &n/*notptr*/;

	/*Ptr*/p1_2 = p1_1/*ptr*/;
	/*not ptr*/p1_1 = &n/*notptr*/;

	return 0;
}
// END FUNCTION: main

/*


int main(int argc, char **argv)
{
	char **arg;

	int *p0_0, *p0_1 = p0_0;
	int *p1_0, *p1_1 = p1_0;

	p0_0 = &argc;
	p1_0 = p0_0 + sizeof(int);


	for (arg = argv; arg < argv + argc; ++arg)
	*arg = NULL; 


	switch (argc) {
		case 1:
		arg = argv;
		break;
	}

	if (argc == 1)
	arg = argv;
	else if (argc == 2)
	arg = argv + 1;
	else if (argc == 3)
	arg = argv + 2;

	return 0;
}
*/
