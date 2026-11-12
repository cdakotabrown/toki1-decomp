/* Function: bri_fade_out */
/* Debug Symbol: bri_fade_out (from o.bin, high match) */
/* Address: 0x80086510 */
/* Size: 96 bytes (24 instructions) */
/* Region: IDO (0x80081000-0x800BFFFF) */
/* @PostProc: convert_or_to_addu --exclude 0x48,0x4c */
/* @PostProc: patch_stack 24 40 */
/* Compiler: IDO 5.2 via ido-static-recomp */
extern int Vblnk_Timer(void);
extern void Vblnk_Timer_Init(void);
extern unsigned char screen_bri;

int bri_fade_out(void)
{
    unsigned int val;

    if (Vblnk_Timer() != 0)
    {
        val = (screen_bri = screen_bri);
        if (val < 3u)
        {
            screen_bri = 0;
            return 1;
        }
        screen_bri = screen_bri - 4;
        Vblnk_Timer_Init();
        return 0;
    }
    return 0;
}
