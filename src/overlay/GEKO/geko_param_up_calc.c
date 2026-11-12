/* @CompileEnv: IDO52_AS1_NO_FALLTHRU_FILL=5 IDO52_AS1_XBB_VOLATILE_OK=occ:4,6,0 */
/* @PostProc: patch_insn_reg_field 0x54:rt:v1:t7 0x58:rs:v1:t7 0x58:rt:v1:t7 0x60:rd:t7:t8 0x60:rt:v1:t7 0x64:rd:v1:t8 0x64:rs:t7:t8 0x64:rt:v1:t7 0x70:rt:v1:t8 0x78:rd:a0:v1 0x7c:rs:a0:v1 0xc0:rd:s1:s2 0xc4:rs:s1:s2 0xd0:rd:s1:s2 0xe4:rd:a1:a0 0xe8:rs:a1:a0 0xf0:rd:a1:a0 0xf4:rs:a0:v1 0x108:rs:a0:v1 0x10c:rs:a0:v1 0x114:rt:a1:a0 0x11c:rs:a0:v1 0x134:rt:v1:t5 0x138:rs:v1:t5 0x138:rt:v1:t5 0x140:rs:v1:t5 0x14c:rs:v1:t5 0x154:rs:v1:t5 0x15c:rs:v1:t5 0x164:rs:v1:t5 0x16c:rs:v1:t5 0x174:rt:t5:t6 0x178:rs:t5:t6 0x178:rt:t6:t7 0x17c:rs:t6:t7 0x1a0:rd:s2:s1 0x1a8:rs:s2:s1 0x1ac:rt:t1:t2 0x1b0:rd:t7:t8 0x1b4:rt:t7:t8 0x1b8:rt:t8:t9 0x1c0:rd:t9:t0 0x1c0:rt:t8:t9 0x1c4:rd:t0:t1 0x1c4:rt:t9:t0 0x1c8:rs:t0:t1 0x1d0:rs:t1:t2 0x1d0:rt:t1:t2 0x1d8:rs:s2:s1 0x1d8:rt:t1:t2 0x1e0:rt:t2:t3 0x1e8:rs:t2:t3 0x204:rs:t2:t3 0x210:rd:s1:s2 0x214:rs:s1:s2 0x214:rt:t3:t4 0x21c:rd:s1:s2 0x21c:rs:t3:t4 0x220:rs:s1:s2 0x220:rt:t6:t7 0x224:rt:t6:t7 0x228:rt:t4:t5 0x230:rd:t5:t6 0x230:rs:t4:t5 0x234:rs:t6:t7 0x240:rd:t7:t8 0x244:rs:t7:t8 0x244:rt:t8:t9 0x248:rd:t9:t0 0x248:rs:t5:t6 0x248:rt:t8:t9 0x24c:rt:t9:t0 0x250:rs:s2:s1 0x250:rt:s2:s1 0x254:rd:t0:t1 0x254:rt:s2:s1 0x258:rd:s2:s1 0x258:rt:t0:t1 0x25c:rs:s2:s1 */
/* geko_param_up_calc - GEKO overlay, Address: 0x80133308, Size: 656 bytes */
#include "types.h"

extern void sub_sub_step_up(void);
extern u32 get_g_zyotai_s(s32 a0);
extern void param_clamp(void);
extern u32 rand(void);

extern volatile u8 now_girl;
extern s32 frame_counter;
extern s32 serifu_no;
extern volatile s16 geko_now_serifu;
extern volatile u8 geko_param_up_flag;
extern u8 screen_w[];

void geko_param_up_calc(void)
{
    s16 i;
    s32 q;
    u32 m;
    u32 rnd;
    u32 r;
    u32 v0;
    s32 t;
    s32 bonus;
    s32 sel;
    u8 *rec;

    v0 = get_g_zyotai_s(now_girl);

    if (serifu_no != 0) {
        t = now_girl;
        t = (t << 3) - t;
        rec = (u8 *)screen_w + (t << 3);
        geko_param_up_flag = 0;
        q = *(s16 *)(rec + 446) / 20;
        q = q & 0xFF;
        m = (u32)frame_counter % (q + 1);
        m = m & 0xFF;
        r = v0 & 0x7F;
        r = r & 0xFF;
        if (r < 2) {
            *(s16 *)(rec + 454) = *(s16 *)(rec + 454) + m + 10;
        } else {
            *(s16 *)(rec + 454) = *(s16 *)(rec + 454) + m + 6;
        }
        geko_now_serifu = geko_now_serifu + 2;
    } else {
        sel = now_girl;
        geko_param_up_flag = 1;
        if (sel == 0 || sel == 7 || sel == 1 || sel == 2 || sel == 8 ||
            sel == 9) {
            r = v0 & 0x7F;
            r = r & 0xFF;
            bonus = 0;
            if (r >= 3) {
                geko_param_up_flag = 0;
                goto run_loop;
            }
        }
        bonus = 4;
run_loop:
        for (i = 0; i < 11; i++) {
            rec = (u8 *)screen_w + i * 56;
            if (((u32)(*(s32 *)(rec + 456) << 30) >> 31) == 0) {
                continue;
            }
            if (i == now_girl) {
                continue;
            }
            q = *(s16 *)(rec + 446) / 20;
            q = q & 0xFF;
            rnd = rand();
            m = rnd % (q + 1);
            m = m & 0xFF;
            *(s16 *)(rec + 454) = *(s16 *)(rec + 454) + bonus + m;
        }
    }
    param_clamp();
    sub_sub_step_up();
}
