/* Function: SD_ReverbReset */
/* Address: 0x8007B870 */
/* Size: 100 bytes (25 instructions) */
/* Region: GCC (0x80041000-0x80080FFF) */
/* Compiler: IDO 5.2 via ido-static-recomp */
/* Post-processing required:
 *   - Patch frame size: 24 -> 40 bytes
 */
extern int tt_step_check(void);
extern void SsUtFlush(void);
extern void SsUtAllKeyOff(int);
extern void SsUtReverbOff(void);
extern void SpuClearReverbWorkArea(int);
extern void SsUtReverbOn(void);

extern int gSDInfo;

void SD_ReverbReset(void)
{
    if (tt_step_check() == 0 || (gSDInfo & 0x100) == 0) {
        SsUtFlush();
        SsUtAllKeyOff(0);
        SsUtReverbOff();
        SpuClearReverbWorkArea(3);
        SsUtReverbOn();
    }
}
