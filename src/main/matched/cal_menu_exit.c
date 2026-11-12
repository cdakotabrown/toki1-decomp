/* Likely Symbol: cal_menu_exit (from o.bin, offset +152 bytes from 0x8005BC00) */
/* Address: 0x8005BB68 */
/* Size: 40 bytes */
/* Compiler: IDO 5.2 */
/* Region: GCC (0x80040000-0x80080000) */
extern void goto_sub_step(int arg0);
extern void goto_sub_sub_step(int arg0);

void cal_menu_exit(void) {
    goto_sub_step(0);
    goto_sub_sub_step(2);
}
