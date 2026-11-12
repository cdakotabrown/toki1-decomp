/* Function: voice_restart */
/* Address: 0x8007B5F8 */
/* Size: 100 bytes (25 instructions) */
/* Region: GCC (0x80041000-0x80080FFF) */
/* Compiler: IDO 5.2 via ido-static-recomp */
/* @CompileFlags: -Wc,-pic1 */
/* Post-processing required:
 *   - Patch frame size: 24 -> 40 bytes
 *   - Patch argument offsets: sp+24/28 -> sp+40/44
 * @CompileFlags: -Wc,-pic1
 */
extern void SsVoKeyOff(void);
extern void SsSetMVol(int, int);
extern void SsVoKeyOn(int, int, int, int);

extern int gSDInfo;
extern unsigned short voice_vol_l;

void voice_restart(int arg0, int arg1)
{
    SsVoKeyOff();
    gSDInfo = gSDInfo & 0xFFEFFFFF;
    SsSetMVol(127, 127);
    SsVoKeyOn(arg0, arg1, voice_vol_l, voice_vol_l);
}
