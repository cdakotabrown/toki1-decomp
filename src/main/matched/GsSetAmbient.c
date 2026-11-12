/* Function: GsSetAmbient */
/* Address: 0x8009D170 */
/* Size: 40 bytes (10 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc260 */
extern void SetBackColor(int, int, int);

void GsSetAmbient(int arg0, int arg1, int arg2)
{
    SetBackColor(arg0 >> 4, arg1 >> 4, arg2 >> 4);
}
