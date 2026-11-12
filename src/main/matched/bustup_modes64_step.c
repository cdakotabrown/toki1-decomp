/* Likely Symbol: bustup_modes31_step (from o.bin, offset +132 bytes from 0x80085690) */
/* Address: 0x80085714 - 0x80085748 (52 bytes) */
/* Region: IDO (>= 0x80081000) */
/* Compiler: IDO 5.2 */
typedef signed short s16;
typedef signed int s32;

extern s16 cal_cw_anim;
extern s16 cw97_anim;

extern void sub_sub_step_up(void);

void bustup_modes64_step(void) {
    volatile s32 pad[4];
    cal_cw_anim = 6;
    cw97_anim = 4;
    sub_sub_step_up();
}
