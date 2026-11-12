/* @SymbolStart: 0x801324EC */
/* Address: 0x801324F4, Size: 176 bytes (44 instructions) */
/* @SymbolSize: 184 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v1:v0 0x14:rs:v1:v0 0x14:rt:v1:v0 0x1c:rs:v1:v0 0x30:rs:v1:v0 0x3c:rd:v1:a0 0x4c:rs:v1:a0 0x50:rs:s0:t8 0x50:rt:t8:s0 0xb0:rd:v1:a0 */
#include "types.h"

extern s16 *get_k_work();

extern s16 cal_cw_x;
extern u8 now_sub_step;
extern s16 cw97_y;

void option_panels_scroll(void)
{
    s32 v0;
    s32 a0;
    s32 s0;
    s32 s1;
    s16 *p;
    s32 v1;

    v0 = cal_cw_x;

    if (v0 >= -287) {
        cal_cw_x = (s16)(v0 - 8);
    }

    a0 = 0;
    s1 = 8;
    do {
        s0 = a0 + 1;
        if (now_sub_step != s0) {
            p = get_k_work();
            v1 = p[0];
            if (v1 >= -247) {
                p[0] = (s16)(v1 - 8);
            }
        } else {
            p = get_k_work();
            v1 = p[1];
            if (v1 >= -87) {
                p[1] = (s16)(v1 - 8);
                cw97_y = (s16)(cw97_y - 8);
            }
        }
        a0 = s0;
    } while (s0 != s1);
}
