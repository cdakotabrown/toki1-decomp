/* @SymbolSize: 660 */
/* @PostProc: swap_instructions --offset1 0x228 --offset2 0x230 */
/* @PostProc: swap_instructions --offset1 0x1b0 --offset2 0x1b4 */
/* @PostProc: swap_instructions --offset1 0x1b4 --offset2 0x1b8 */
/* @PostProc: patch_insn_reg_field 0x1dc:rs:t2:v0 0x1dc:rt:v0:t2 0x228:rt:t9:t8 0x230:rt:t8:t9 0x234:rs:t8:t9 0x234:rt:t8:t9 0x23c:rs:t8:t9 0x244:rs:t9:t8 0x244:rt:t9:t8 0x254:rs:t9:t8 0x25c:rs:t4:v0 0x25c:rt:v0:t4 0x270:rs:t8:t9 0x274:rt:t9:t8 0x278:rs:t9:t8 0x278:rt:t9:t8 0x280:rs:t9:t8 */
/* Address: 0x80132B14, Size: 660 bytes (165 instructions) */
#include "types.h"

extern s32  check_end_k(void);
extern s32  get_CD_l_com(void);
extern void k_reset(s32);
extern void ending_serifu_cmd(s32, s32, s32, s32, s32, s32, s32);

extern s32  serifu_wait;
extern s32  serifu_voice;
extern s32  msg_wait;
extern s32  serifu_state;
extern s32  pad_data;
extern s32  step_counter;
extern s16 *now_s;
extern s16 *now_s_in;
extern s32 **ss_s;
extern u8  **ss_s_kind;
extern s32  sss_so;
extern u8   sound_seek_wait;
extern u8   ending_wait_cnt;

s32 ending_serifu_main(u8 p0, u8 p1, u8 p2)
{
    s32 v;
    s32 i;
    s32 j;

    if (serifu_wait != 0) {
        if (check_end_k() == 0) {
            goto ret0;
        }
        v = serifu_voice;
        if (v != 0) {
            if (v != 1) {
                goto ret0;
            }
            if (get_CD_l_com() != 9) {
                goto ret0;
            }
        }
        v = step_counter + 1;
        step_counter = v;
        if ((u32)v < 60) {
            goto ret0;
        }
        serifu_wait = 0;
        k_reset(1);
        return 1;
    }

    if (msg_wait != 0) {
        if (check_end_k() == 0) {
            if (check_end_k() != 0) {
                goto ret0;
            }
            if (pad_data != 0) {
                goto ret0;
            }
        }
        v = serifu_voice;
        if (v != 0) {
            if (v != 1) {
                goto ret0;
            }
            if (get_CD_l_com() != 9) {
                goto ret0;
            }
        }
        msg_wait = 0;
        return 1;
    }

    if (serifu_state != 0) {
        if (check_end_k() == 0) {
            if (check_end_k() != 0) {
                goto ret0;
            }
            if (pad_data != 0) {
                goto ret0;
            }
        }
        v = serifu_voice;
        if (v != 0) {
            if (v != 1) {
                goto ret0;
            }
            if (get_CD_l_com() != 9) {
                goto ret0;
            }
        }
        serifu_state = 0;
        k_reset(1);
        return 1;
    }

    step_counter = 0;
    i = *now_s;
    j = *now_s_in;
    ending_serifu_cmd(ss_s[i][j], ss_s_kind[i][j], p0, sss_so,
                  i, j, p2);

    if (ending_wait_cnt != 0) {
        goto ret0;
    }
    if (p1 == 0) {
        goto ret0;
    }
    if (sound_seek_wait != 0) {
        goto ret0;
    }

    i = *now_s;
    j = *now_s_in;
    if (ss_s_kind[i][j] == 255) {
        *now_s = (s16)(i + 1);
        *now_s_in = 0;
    } else {
        *now_s_in = (s16)(j + 1);
    }
ret0:
    return 0;
}
