/* Likely Symbol: normal_music_read_init0 (from o.bin, offset +56 bytes from 0x800789E8) */
/* Address: 0x800789B0 - 0x800789E0 (48 bytes) */
/* Region: GCC (< 0x80081000) */
/* Compiler: IDO 5.2 (via static-recomp) */
typedef signed int s32;

extern void class_clear_f(void);
extern void icon_disp_switch(s32 arg);
extern void goto_sub_step(s32 arg);

void holiday_club_join_exit(void) {
    class_clear_f();
    icon_disp_switch(1);
    goto_sub_step(5);
}
