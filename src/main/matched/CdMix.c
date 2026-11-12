/* Address: 0x80087D88 */
/* Size: 32 bytes (8 instructions) */
/* Region: Library (>= 0x80086000) */
/* @Compiler: gcc272 */
/* @CompileFlags: --konami-lib-sched */
extern void CD_vol(void);

int CdMix(void)
{
    CD_vol();
    return 1;
}
