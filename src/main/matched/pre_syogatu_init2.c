/* Debug Symbol: pre_syogatu_init2 (from o.bin, high match) */
/* Address: 0x800613D0 - 0x80061420 (80 bytes) */
/* Region: GCC (0x80041000 - 0x80080FFF) */
/* Compiler: IDO 5.2 */
typedef signed int s32;

extern s32 get_CD_State2(void);
extern s32 dec_bg_init(s32 param);
extern void indexed_byte_lsb_set(s32 a, s32 b);
extern void set_dec_bri(s32 param);
extern void sub_sub_step_up(void);

void pre_syogatu_init2(void) {
    s32 result;

    result = get_CD_State2();
    if ((result & 1) != 0) {
        result = dec_bg_init(1);
        indexed_byte_lsb_set(1, result);
        set_dec_bri(128);
        sub_sub_step_up();
    }
}
