#define NULL 0

/* p0 -> T
 * p1 -> C
 */
void function_00()
{
  int x, y;

  int *p0 = &x;
  int *p1 = &y;

  p0 = &y;
}

/* p0 -> T
 * p1 -> C
 */
void function_01()
{
  int x, y;

  int *p0 = &x;
  int *p1 = p0;

  p0 = &y;
}

/* p0 -> Constant
 * p1 -> Constant
 */
void function_02()
{
  int x, y;

  int *p0 = &x;
  int *p1 = &x;

  p0 = &x;
}

/* p0 -> C
 * p1 -> C
 */
void function_03()
{
  int x, y;
  
  int *p0 = &x;
  int *p1 = &x;

  p0 = p1;
}

/* p0 -> T
 * p1 -> T
 * p2 -> C
 */
void function_04()
{
  int x, y;

  int *p0 = &x;
  int *p1 = &x;
  int *p2 = &y;

  p0 = p2;
  p1 = p0;
}

/* p0 -> T
 * p1 -> C
 * p2 -> C
 */
void function_05()
{
  int x, y;

  int *p0 = &x;
  int *p1 = p0;
  int *p2 = &y;

  p0 = p2;
}

/* p0 -> T
 * p1 -> C
 * p2 -> C
 */
void function_06()
{
  int x, y;

  int *p0 = &x;
  int *p1 = p0;
  int *p2 = p1;

  p0 = &y;
}

void function_07()
{
  int x, y;

  int *p0_0 = &x;
  int *p1_1 = p0_0;
  int *p1_2 = p1_1;
  int *p2_1 = p0_0;
  int *p2_2 = p2_1;

  p0_0 = &y;
  p2_1 = p2_2++;
}

void function_08(int *t, int sz)
{
  int *tmp = t;

  while (sz--)
    *tmp++ = 0;
}

void function_09(int *t, int sz)
{
  int i;

  for (i = 0; i < sz; ++i)
    t[i] = 0;
}


/* int *function(int limit, int t[], size_t sz) */
/* { */
/*   int size; */
/*   int *sizep = &size; */
/*   int *tp = t;   */
/*   int *result = (int*)malloc(sizeof(int) * sz); */

/*   while (tp < (t + sz)) { */
/*     if (*tp % 2 == 0) { */
/*       if (*tp < limit) { */
/* 	result[(*sizep)++] = *tp; */
/*       } */
/*     } */

/*     tp++; */
/*   } */

/*   result = (int*)realloc(result, (sizeof(int) * (*sizep))); */

/*   return result; */
/* } */



