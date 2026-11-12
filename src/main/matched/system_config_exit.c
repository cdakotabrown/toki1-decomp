/* Address: 0x80060388 */
/* Size: 40 bytes */
/* Compiler: IDO 5.2 */
/* Region: GCC (0x80040000-0x80080000) */
extern void goto_sub_step(int arg0);
extern void goto_sub_sub_step(int arg0);

void system_config_exit(void) {
    goto_sub_step(0);
    goto_sub_sub_step(2);
}
