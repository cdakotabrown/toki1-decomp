/* Likely Symbol: basyo_init2 (from o.bin, offset +52 bytes from 0x80066200) */
/* Address: 0x80066234 */
/* Size: 40 bytes */
/* Compiler: IDO 5.2 */
/* Region: GCC (0x80040000-0x80080000) */
extern void basyo_init(int arg0);
extern unsigned short mes_base_y_h;

void basyo_init2(int arg0) {
    basyo_init(arg0);
    mes_base_y_h = 184;
}
