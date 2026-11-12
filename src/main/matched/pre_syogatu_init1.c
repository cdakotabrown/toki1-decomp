/* Function: pre_syogatu_init1 */
/* Address: 0x80061348 */
/* Size: 136 bytes (34 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s32 device_load_poll(void);
extern s32 value_cache_find_or_load(s32, s32);
extern void indexed_byte_lsb_set(s32, s32);
extern void set_dec_bri(s32);
extern void sub_sub_step_up(void);
extern void goto_step_bg_out(void);

void pre_syogatu_init1(void) {
    s32 ret;

    if (device_load_poll() == 1) {
        ret = value_cache_find_or_load(0x4167, 1);
        if (ret != -1) {
            if (ret == 0 || ret == 1) {
                indexed_byte_lsb_set(1, ret);
                set_dec_bri(128);
                sub_sub_step_up();
                sub_sub_step_up();
            }
        } else {
            sub_sub_step_up();
        }
    }
    goto_step_bg_out();
}
