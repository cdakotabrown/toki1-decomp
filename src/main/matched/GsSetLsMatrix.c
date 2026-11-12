/* Function: GsSetLsMatrix */
/* Address: 0x8009AE48 */
/* Size: 44 bytes (11 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 --fill-earliest */
/* @PostProc: strip_leading_instructions --count auto */
extern void SetRotMatrix(void);
extern void SetTransMatrix(int arg);

void GsSetLsMatrix(int arg0)
{
    SetRotMatrix();
    SetTransMatrix(arg0);
}
