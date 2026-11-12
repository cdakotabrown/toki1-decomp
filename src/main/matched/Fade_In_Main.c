/* Debug Symbol: Fade_In_Main (from o.bin, high match) */
/* Address: 0x80086590 */
/* Size: 100 bytes (25 instructions) */
/* @PostProc: convert_or_to_addu --exclude 0x4c,0x50 */
/* @PostProc: patch_stack 24 40 */
/* Compiler: IDO 5.2 */
/* Region: IDO (0x80086000-0x800BFFFF) */
extern int Vblnk_Timer(void);
extern void Vblnk_Timer_Init(void);
extern unsigned char screen_bri;

int Fade_In_Main(void)
{
    unsigned int temp;

    if (Vblnk_Timer() != 0)
    {
        temp = screen_bri;
        if (temp >= 126u)
        {
            screen_bri = 128;
            return 1;
        }
        temp = screen_bri;
        screen_bri = temp + 4;
        Vblnk_Timer_Init();
        return 0;
    }
    return 0;
}
