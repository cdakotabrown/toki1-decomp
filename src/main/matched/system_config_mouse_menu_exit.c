/* Address: 0x8005EC2C */
/* Size: 76 bytes */
/* Compiler: IDO 5.2 */
/* Region: GCC (0x80040000-0x80080000) */
extern int pad_trig;
extern unsigned char now_icon;
extern void k_disp_inc(void);
extern void goto_sub_sub_step(int arg);

void system_config_mouse_menu_exit(void) {
    k_disp_inc();
    if (pad_trig & 0x20) {
        goto_sub_sub_step(0);
        now_icon = 2;
    }
}
