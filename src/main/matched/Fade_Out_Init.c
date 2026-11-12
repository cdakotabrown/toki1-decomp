/* Likely Symbol: bri_fade_out (from o.bin, offset +40 bytes from 0x80086518) */
/* Address: 0x800864F0 */
/* Size: 32 bytes (8 instructions) */
/* Compiler: IDO 5.2 */
/* Region: IDO (0x80081000-0x800BFFFF) */
/* @PostProc: convert_or_to_addu */
/* @CompileFlags: -Wc,-pic1 */
extern void Vblnk_Timer_Init(void);

void Fade_Out_Init(void)
{
    Vblnk_Timer_Init();
}
