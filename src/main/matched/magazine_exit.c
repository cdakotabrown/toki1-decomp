/* Likely Symbol: holiday_tel_init (from o.bin, offset +152 bytes from 0x80078050) */
/* Address: 0x80077FB8 - 0x80077FE8 (48 bytes) */
/* Region: GCC (< 0x80081000) */
/* Compiler: IDO 5.2 (via static-recomp) */
typedef signed int s32;

extern void basyo_flag_set(void);
extern void hizuke_disp_switch(s32 arg);
extern void goto_sub_step(s32 arg);

void magazine_exit(void) {
    basyo_flag_set();
    hizuke_disp_switch(1);
    goto_sub_step(0);
}
