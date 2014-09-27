/*Source Code From Laure Gonnord*/


int main(){

  void* toto[21];
  
  printf("size=%d\n",sizeof(toto));

  toto[12]=(void*) 70;

  mys s ;
  s.one = (double) 12345678901923927;
  s.two = (double) 12345678901;

  toto[16] = (void*) &s;
  printf("double = %d\n",sizeof(double));

  printf("size=%d\n",sizeof(toto[12]));
  printf("%f\n",((mys*) toto[16])->one);
  printf("%f\n",((mys*) toto[16])->two);

  return 0;
}
