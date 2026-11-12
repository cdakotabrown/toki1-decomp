/* Address: 0x8005409C */
/* Size: 20 bytes */
/* Compiler: IDO 5.2 */
/* Region: GCC (0x80040000-0x80080000) */
extern unsigned char card_result;
extern unsigned char card_step;

void FUN_8005409C(unsigned char arg0) {
    card_result = 0;
    card_step = arg0;
}
