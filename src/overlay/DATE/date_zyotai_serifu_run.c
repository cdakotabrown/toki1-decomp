/* @SymbolSize: 212 */
/* @PostProc: swap_instructions --offset1 0x34 --offset2 0x38 */
/* @PostProc: patch_insn_reg_field 0x28:rt:v1:a0 0x2c:rs:v1:a0 0x2c:rt:v1:a0 0x34:rs:v1:a0 0x34:rt:t7:v1 0x38:rs:v1:a0 0x3c:rs:t7:v1 0x44:rt:v1:t7 0x48:rs:v1:t7 0x4c:rs:v1:t7 0x4c:rt:t0:t9 0x50:rs:t0:t9 0x50:rt:t1:t9 0x54:rt:t9:t8 0x58:rs:v1:t7 0x58:rt:t3:t1 0x5c:rs:t3:t1 0x5c:rt:t4:t1 0x60:rd:t2:t0 0x60:rs:t9:t8 0x60:rt:t1:t9 0x64:rd:t5:t2 0x64:rs:t2:t0 0x64:rt:t4:t1 0x6c:rt:t5:t2 0x70:rt:t6:t3 0x74:rs:t6:t3 0x74:rt:t6:t3 0x7c:rt:t6:t3 0x80:rt:t7:t4 0x84:rs:t7:t4 0x84:rt:t7:t4 0x8c:rt:t8:t5 0x90:rs:t8:t5 0x90:rt:t8:t5 0x94:rt:t7:t4 0x9c:rt:t0:t6 0xa0:rs:t0:t6 0xa0:rt:t0:t6 0xa4:rt:t8:t5 0xac:rt:t9:t7 0xb0:rs:t9:t7 0xb0:rt:t9:t7 0xb8:rt:t0:t6 0xd0:rt:t9:t7 */
/* Function: date_zyotai_serifu_run */
/* Overlay: DATE */
/* Address: 0x8013B74C */
/* Size: 212 bytes (53 instructions) */
#include "types.h"

extern s32 get_g_zyotai_s(s32);
extern s32 serifu_main(s32, s32, s32);

extern volatile u8 now_girl;
extern volatile u8 step_mode;

extern volatile s16 date_now_serifu_zyotai;
extern volatile s16 date_now_serifu_zyotai_in;


extern volatile u8 serifu_no;

extern s32 now_s;
extern s32 now_s_in;
extern s32 ss_s;
extern s32 ss_s_kind;
extern s32 sss_so;
extern s32 date_serifu_tbl[];
extern s32 date_serifu_kind_tbl[];
extern s32 date_sound_tbl[];

void date_zyotai_serifu_run(void) {
    s32 a0;
    s32 v0;
    s32 t7;
    s32 cond;

    v0 = get_g_zyotai_s(now_girl);

    a0 = step_mode;
    step_mode = (u8)(a0 + 1);
    cond = (u32)a0 < 1u;
    if (cond != 0) {
        t7 = v0 & 0x7F;
        date_now_serifu_zyotai = (s16)(((((u32)t7 < 2u) ^ 1) +
                            (((u32)t7 < 3u) ^ 1)) +
                           (((u32)t7 < 4u) ^ 1));
    }

    now_s = (s32)&date_now_serifu_zyotai;
    now_s_in = (s32)&date_now_serifu_zyotai_in;
    ss_s = date_serifu_tbl[26];
    ss_s_kind = date_serifu_kind_tbl[26];
    sss_so = date_sound_tbl[26];

    serifu_main(serifu_no, 1, 0);
}
