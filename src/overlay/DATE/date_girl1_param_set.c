/* @SymbolStart: 0x8013D254 */
/* Address: 0x8013D25C */
/* @SymbolSize: 232 */
/* @PostProc: patch_insn_reg_field 0x1c:rt:a0:v1 0x20:rs:a0:v1 0x28:rt:v1:v0 0x2c:rs:v1:v0 0x40:rs:a0:v0 0x40:rt:v0:v1 */
#include "types.h"

extern void date_trans_set(void);
extern void sub_sub_step_up(void);

extern u8  now_girl;
extern u8  date_pattern;
extern s16 now_serifu;
extern u8  now_month;
extern s16 imp[];
extern s16 kao[];

void date_girl1_param_set(void) {
    u8 state;
    u8 v;

    if (now_girl == 1) {
        state = date_pattern;
        if (state == 0) {
            now_serifu = 4;
        }
        if (state == 1) {
            now_serifu = 7;
        }
        if (state == 2) {
            now_serifu = 10;
        }
        v = now_month;
        if ((u32)v >= 6u && (u32)v < 9u) {
            now_serifu = 0;
            imp[0] = 1;
            imp[1] = 4;
            imp[2] = 2;
            kao[0] = 2;
            kao[1] = 3;
            kao[2] = 3;
        }
        date_trans_set();
    } else {
        sub_sub_step_up();
    }
}
