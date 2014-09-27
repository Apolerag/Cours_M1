#define L_AND(a, b) a && b
#define L_OR(a, b) a || b
#define L_SUP(a, b) a > b
#define L_INF(a, b) a < b
#define L_SUPEQ(a, b) a >= b
#define L_INFEQ(a, b) a <= b
#define L_DIF(a, b) a != b
#define L_EQ(a, b) a == b

/* FUNCTION: main
 * returns: NULL TYPE
 */
int main(int argc, char *argv[])
{
	int quarante_deux;

	int *i, *j = i;
	quarante_deux = 0;

	while (1)
	{
	if (L_SUPEQ(quarante_deux ,42))
		{
		quarante_deux++;
		}
	else
		{
		break;
		}
		}


	//free(i);
	return L_AND((L_DIF(quarante_deux , 0)) , (L_EQ(3 , 0)));
}
// END FUNCTION: main

