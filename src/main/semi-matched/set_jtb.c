/* Likely Symbol: SeqPlay (from o.bin, offset +80 bytes from 0x8009F920) */
extern void GPU_cw(unsigned int arg);
void set_jtb(unsigned int arg0)
{
  int new_var2;
  int new_var3;
  unsigned long new_var;
  new_var3 = 1;
  new_var2 = 0;
  if (1)
  {
 if (1) { } if (new_var3) { }
    new_var = arg0;
  }
  new_var = new_var & (0x00FFFFFF ^ new_var2);
  GPU_cw(new_var);
}
