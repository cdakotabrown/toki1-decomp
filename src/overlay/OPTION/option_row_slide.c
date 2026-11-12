/* @CompileFlags: -Wc,-pic0 */
/* @CompileEnv: IDO52_CONST_NOINREG_VALUES=20 */
/* @PostProc: remap_stack_slots --from 52 --to 80 */
/* @PostProc: swap_registers s3:s4 --bidirectional */
/* @PostProc: patch_insn_reg_field 0x14:rt:v1:v0 0x18:rs:v1:v0 0x18:rt:v1:v0 0x20:rs:v1:v0 0x24:rt:s3:s4 0x28:rt:s4:s3 0x3c:rs:v1:v0 0x4c:rt:v1:v0 0x54:rt:v1:v0 0x7c:rs:s1:t8 0x7c:rt:t8:s1 0x164:rt:s4:s3 0x168:rt:s3:s4 */
/* @PostProc: swap_instructions --offset1 0x68 --offset2 0x6c */
#include "types.h"

extern s16 cal_cw_x;
extern s16 cw97_y;
extern u8 now_sub_step;
extern s8 now_icon;

extern s16 *get_k_work(s32 idx);
extern void option_row_pos_set();

void option_row_slide(void)
{
    s32 i;
    s32 next;
    s32 lim;
    s32 x;
    s16 *p;
    s32 a;
    s32 v;
    s16 buf[40];

    a = cal_cw_x;
    if (a < -160) {
        cal_cw_x = a + 12;
    } else {
        cal_cw_x = -160;
    }

    option_row_pos_set(buf);

    i = 0;
    lim = 8;
    do {
        next = i + 1;
        x = i * 20;
        if (now_sub_step != next) {
            p = get_k_work(i);
            v = p[0];
            if (v < -120) {
                p[0] = v + 12;
            } else {
                p[0] = -120;
            }
        } else {
            x = x - 88;
            p = get_k_work(i);
            v = p[1];
            if (v < x) {
                p[1] = v + 12;
                cw97_y = cw97_y + 12;
                if (p[1] >= x) {
                    p[1] = x;
                    cw97_y = buf[now_icon + 8] + 6;
                }
            } else {
                p[1] = x;
                cw97_y = buf[now_icon + 8] + 6;
            }
        }
        i = next;
    } while (next != lim);
}
