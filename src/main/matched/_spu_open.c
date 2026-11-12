/* _spu_open - Address: 0x80095A10, Size: 44 bytes */
extern void _SpuDataCallback(char *arg);
extern char D_80095D00[];
int _spu_open(int arg0, int arg1)
{
  volatile int *new_var;
  volatile int *p0 = &arg0;
  volatile int *p1 = &arg1;
  volatile int *new_var2;
  if (!D_80095D00)
  {
  }
  new_var = p0;
  new_var2 = new_var;
  (void) new_var2;
  (void) p1;
  _SpuDataCallback(D_80095D00);
  return 3;
}
