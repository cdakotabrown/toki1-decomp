/* @PostProc: patch_insn_reg_field 0x3c:rt:t7:a0 0x44:rt:t7:a0 0x50:rt:t8:t7 0x58:rs:t8:t7 0x5c:rs:t8:t7 0x78:rt:t0:t1 0x84:rt:t0:t1 0x98:rt:t1:t3 0xa0:rt:t1:t3 */
/* geko_zyotai_serifu_offset -- GEKO overlay, Address: 0x80136FEC, Size: 172 bytes (43 insns) */
#include "types.h"

extern s32 get_g_zyotai_s(s32 a0);
extern void sub_sub_step_up(void);

extern u8 now_girl;
extern volatile s16 geko_now_serifu2;
extern u8 geko_girl_flag;

void geko_zyotai_serifu_offset(void)
{
    s32 mode;
    u8 m;
    s32 cur;

    mode = get_g_zyotai_s(now_girl);
    if (geko_girl_flag == 1) {
        geko_now_serifu2 = 6;
    }
    cur = geko_now_serifu2;
    m = mode & 0x7F;
    if (m == 2) {
        geko_now_serifu2 = cur + 2;
    } else if (m == 3) {
        geko_now_serifu2 = cur + 4;
    } else {
        geko_now_serifu2 = cur;
        if (m == 4) {
            geko_now_serifu2 = cur + 6;
        }
    }
    sub_sub_step_up();
}
