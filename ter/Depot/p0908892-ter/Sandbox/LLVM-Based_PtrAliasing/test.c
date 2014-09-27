int main()
{
  int *p1, *p2, *p3 = p2, *p4 = p2;
  int n;
  p0_1 = &n;

  return 0;
}

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
