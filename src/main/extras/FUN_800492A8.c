/* Address: 0x800492A8 */
/* Size: 20 bytes */
/* Compiler: IDO 5.2 */
/* Region: GCC (0x80040000-0x80080000) */
extern unsigned char screen_bri;
extern unsigned char bri_step;

void FUN_800492A8(unsigned char arg0) {
    screen_bri = arg0;
    bri_step = 0;
}
