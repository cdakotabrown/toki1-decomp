/* @PostProc: patch_stack_complete --old 48 --new 56 */
/* @PostProc: swap_instructions --offset1 0x90 --offset2 0x94 */
/* @PostProc: remap_stack_slots --from 40 --to 44 --from 42 --to 46 --from 44 --to 48 --from 46 --to 50 */
/* @PostProc: patch_insn_reg_field 0x4c:rt:a2:t0 0x50:rs:a2:t0 0x50:rt:a2:t0 0x54:rt:t3:t4 0x58:rs:t3:t4 0x58:rt:t3:t4 0x5c:rt:a2:t0 0x60:rd:t2:t3 0x64:rs:t3:t4 0x64:rt:t4:t5 0x68:rd:t5:t6 0x68:rs:t2:t3 0x68:rt:t4:t5 0x70:rt:t5:t6 0x74:rt:v1:t7 0x78:rs:v1:t7 0x78:rt:v1:t7 0x7c:rt:a3:a2 0x80:rs:v1:t7 0x80:rt:a3:a2 0x84:rt:t0:v1 0x88:rs:t0:v1 0x88:rt:t0:v1 0x8c:rt:v1:t2 0x90:rt:t2:t7 0xa8:rd:t6:t8 0xac:rs:t6:v1 0xac:rt:t0:t8 0xb0:rt:t7:t0 0xb8:rs:t7:t0 0xb8:rt:t8:t1 0xbc:rt:t8:t1 0xc0:rs:v1:t2 0xc0:rt:v1:t2 0xc8:rs:v1:t2 0xc8:rt:a3:a2 0xcc:rd:t9:t4 0xd0:rs:t9:v1 0xd0:rt:t0:t4 0xd4:rt:t1:t5 0xdc:rs:t1:t5 0xdc:rt:t3:t6 0xe0:rt:t3:t6 0xe4:rt:t2:t7 */
/* Address: 0x8013876C */
/* Size: 272 bytes (68 instructions) */
#include "types.h"

extern void ClearImage(void *, s32, s32, s32);
extern void strcpy(void *, void *);
extern void sub_sub_step_up(void);

extern volatile u8 byouki_kind;
extern volatile u8 girl2_basyo;
extern volatile u8 now_girl;
extern u8 screen_w[];
extern s16 shougatu_date_now_serifu;
extern s16 shougatu_date_now_serifu_in;
extern u8 ba_string[];
extern u8 shougatu_girl2_basyo_set3_str0[];
extern u8 ba_string2[];
extern u8 shougatu_girl2_basyo_set3_str1[];

void shougatu_girl2_byouki_serifu6_set(void)
{
    s16 rect[4];
    u8 *p;
    u32 hi;

    rect[0] = 1152;
    rect[1] = 352;
    rect[2] = 16;
    rect[3] = 16;
    ClearImage(rect, 0, 0, 0);
    hi = (u32)girl2_basyo >> 4;
    byouki_kind = (u8)((hi << 4) | (byouki_kind & 0xFF0F));
    p = screen_w;
    p[now_girl * 56U + 456] |= 4;
    p[now_girl * 56U + 458] |= 8;
    shougatu_date_now_serifu = 6;
    shougatu_date_now_serifu_in = 0;
    strcpy(ba_string, shougatu_girl2_basyo_set3_str0);
    strcpy(ba_string2, shougatu_girl2_basyo_set3_str1);
    sub_sub_step_up();
}
