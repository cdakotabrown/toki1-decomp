/* @SymbolStart: 0x8013395C */
/* @SymbolSize: 268 */
/* @PostProc: scheduler_normalizer */
/* @PostProc: patch_insn_reg_field 0x68:rt:s3:s4 0x6c:rs:s3:s4 0x6c:rt:s3:s4 0x7c:rt:s4:s3 0xc8:rt:s4:s3 0xf0:rt:s3:s4 */
/* en_nichi_near_set - EN_NICHI overlay, Address: 0x8013395C, Size: 268 bytes (67 insns) */
#include "types.h"

extern s32 en_nichi_sq_dist(s32, s32, s32, s32);
extern s32 en_nichi_get_cnt;
extern u8 en_nichi_obj_wk[];
extern u8 en_nichi_near_tbl[10];
extern s16 cursor_cw_x;
extern s16 cursor_cw_y;

void en_nichi_near_set(void)
{
    s32 s2;
    s32 ret;
    u8 *s1;
    u8 *s0;
    u8 *s4;

    s2 = (en_nichi_get_cnt / 2) + 5;
    if (s2 >= 11) {
        s2 = 10;
    }
    s2 = s2 * s2;
    s1 = en_nichi_obj_wk;
    s0 = &en_nichi_near_tbl[0];
    s4 = &en_nichi_near_tbl[10];
    do {
        ret = en_nichi_sq_dist(*(s16 *)(s1 + 22), *(s16 *)(s1 + 26),
                            cursor_cw_x, cursor_cw_y);
        if (*(s32 *)s1 == 0) {
            goto store_zero;
        }
        if (ret < s2) {
            *s0 = 2;
        } else if (ret < 256) {
            *s0 = 1;
        } else {
            *s0 = 0;
        }
        goto next;
    store_zero:
        *s0 = 0;
    next:
        s0 += 1;
        s1 += 48;
    } while (s0 != s4);
}
