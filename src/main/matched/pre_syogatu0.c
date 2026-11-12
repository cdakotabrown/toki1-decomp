/* Address: 0x80061518 */
/* Size: 432 bytes */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern u8 step_mode;
extern u8 p_sei;
extern u8 pre_syogatu0_str0;
extern u8 pre_syogatu0_str1;
extern u8 pre_syogatu0_str2;

extern void k_sub_reset(void);
extern void gnsx(void *arg0);
extern void sndisp(void *arg0, s32 arg1, s32 arg2);
extern void xa_wait(void);
extern void xa_wait_m(void);
extern void k_sub_disp_start(s32 arg0);
extern void xa_set_m(u32 arg0, u32 arg1, s32 arg2, s32 arg3);
extern s32 value_cache_find_or_load(s32 arg0, s32 arg1);
extern void indexed_byte_lsb_set(s32 arg0, s32 arg1);
extern void set_dec_bri(s32 arg0);
extern s32 get_CD_State2(void);
extern s32 dec_bg_init(s32 arg0);
extern void sub_sub_step_up(void);

void pre_syogatu0(void) {
    u8 mode = step_mode;

    switch (mode) {
    case 0:
        k_sub_reset();
        gnsx(&p_sei);
        sndisp(&pre_syogatu0_str0, 0, 31);
        step_mode = step_mode + 1;
        break;
    case 1:
        xa_wait();
        return;
    case 2:
        k_sub_reset();
        gnsx(&pre_syogatu0_str1);
        sndisp(&pre_syogatu0_str2, 0, 14);
        k_sub_disp_start(1);
        xa_set_m(0x08000005, 0x03000009, -1, 14);
        step_mode = step_mode + 1;
        break;
    case 3:
        xa_wait_m();
        return;
    case 4: {
        s32 result = value_cache_find_or_load(16708, 1);

        if (result != -1) {
            if (result != 0) {
                if (result != 1) {
                    return;
                }
            }
            indexed_byte_lsb_set(1, result);
            set_dec_bri(128);
            step_mode = 6;
        } else {
            step_mode = 5;
        }
        break;
    }
    case 5: {
        s32 result = get_CD_State2();

        if ((result & 1) == 0) {
            return;
        }
        result = dec_bg_init(1);
        indexed_byte_lsb_set(1, result);
        set_dec_bri(128);
        step_mode = step_mode + 1;
        break;
    }
    case 6:
        sub_sub_step_up();
        break;
    }
}
