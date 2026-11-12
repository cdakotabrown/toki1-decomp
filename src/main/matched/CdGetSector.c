/* Function: CdGetSector */
/* Address: 0x80087DA8 */
/* Size: 32 bytes (8 instructions) */
/* Region: Library (0x80086000+), Psy-Q GCC 2.7.2 */
/* @Compiler: gcc272 */
/* @CompileFlags: --konami-lib-sched */
extern int CD_getsector(void);

int CdGetSector(void)
{
    return CD_getsector() == 0;
}
