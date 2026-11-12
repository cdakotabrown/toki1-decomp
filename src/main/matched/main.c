/* Address: 0x800422A0 */
/* Size: 100 bytes */
/* Compiler: IDO 5.2 */
/* Region: GCC (0x80040000-0x80080000) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s32 SetSp(void *addr);
extern void InitHeap(void *addr, s32 size);
extern void bzero(void *dst, s32 size);
extern s32 GetGp(void);
extern void _main(void);

extern s32 boot_sp;
extern s32 boot_gp;

void main(void)
{
    boot_sp = SetSp((void*)0x801FEFF0);
    InitHeap((void*)0x801FF800, 0x7F0);
    bzero((void*)0x800ED730, 0x44698);
    boot_gp = GetGp();
    _main();
}
