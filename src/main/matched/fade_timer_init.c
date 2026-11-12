/* Likely Symbol: Fade_In_Main (from o.bin, offset +24 bytes from 0x80086588) */
/* Address: 0x80086570 */
/* Size: 32 bytes (8 instructions) */
/* Compiler: IDO 5.2 */
/* Region: IDO (0x80081000-0x800BFFFF) */
/* @PostProc: convert_or_to_addu */
/* @CompileFlags: -Wc,-pic1 */
extern void Vblnk_Timer_Init(void);

void fade_timer_init(void)
{
    Vblnk_Timer_Init();
}
