/* Likely Symbol: bustup_modes31_step (from o.bin, offset +184 bytes from 0x80085690) */
/* Address: 0x80085748 - 0x8008577C (52 bytes) */
/* Region: IDO (>= 0x80081000) */
/* Compiler: IDO 5.2 */
typedef signed short s16;
typedef signed int s32;

extern s16 cal_cw_anim;
extern s16 cw97_anim;

extern void sub_sub_step_up(void);

void bustup_modes75_step(void) {
    volatile s32 pad[4];
    cal_cw_anim = 7;
    cw97_anim = 5;
    sub_sub_step_up();
}
