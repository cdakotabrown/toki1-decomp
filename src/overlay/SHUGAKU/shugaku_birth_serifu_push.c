/* Address: 0x801330B8 (true start 0x80133038, .sym late by 32 insns) */
/* @SymbolStart: 0x80133038 */
/* @SymbolSize: 176 */
/* @PostProc: patch_insn_reg_field 0x10:rt:t6:v0 0x14:rs:t6:v0 0x14:rt:t6:v0 0x1c:rt:t7:t6 0x20:rs:t7:t6 0x20:rt:t7:t6 0x24:rt:t6:v0 0x2c:rt:t8:t7 0x30:rs:t8:t7 0x30:rt:t8:t7 0x34:rt:t7:t6 0x3c:rt:t9:t8 0x40:rs:t9:t8 0x40:rt:t9:t8 0x44:rt:t8:t7 0x4c:rt:t0:t5 0x50:rs:t0:t5 0x50:rt:t0:t5 0x54:rt:t9:t8 0x5c:rt:t1:t6 0x60:rs:t1:t6 0x60:rt:t1:t6 0x64:rt:t3:v0 0x68:rt:t0:t5 0x6c:rs:t3:v0 0x6c:rt:t3:v0 0x74:rt:t2:t7 0x78:rs:t2:t7 0x78:rt:t2:t7 0x7c:rt:t5:t2 0x80:rt:t1:t6 0x84:rs:t5:t2 0x84:rt:t5:t2 0x8c:rd:t4:t9 0x8c:rt:t3:v0 0x94:rd:t4:t1 0x94:rs:t4:t9 0x94:rt:t3:v0 0x98:rt:t2:t7 0xa4:rd:t6:t4 0xa4:rs:t4:t1 0xa4:rt:t5:t2 0xac:rt:t6:t4 */
#include "types.h"

extern void sub_sub_step_up(void);

extern s16 shugaku_birth_now_serifu;
extern s16 shugaku_birth_last_serifu;
extern s32 shugaku_birth_ss_serifu;
extern s32 shugaku_birth_last_ss_serifu;
extern s32 shugaku_birth_ss_serifu_kind;
extern s32 shugaku_birth_last_ss_serifu_kind;
extern s32 shugaku_birth_sss_sound;
extern s32 shugaku_birth_last_sss_sound;
extern u8 now_girl;
extern u8 shugaku_gakunen;
extern s32 shugaku_birth_serifu_tbl[];
extern s32 shugaku_birth_serifu_kind_tbl[];
extern s32 shugaku_birth_sound_tbl[];

void shugaku_birth_serifu_push(void)
{
    shugaku_birth_last_serifu = shugaku_birth_now_serifu;
    shugaku_birth_last_ss_serifu = shugaku_birth_ss_serifu;
    shugaku_birth_last_ss_serifu_kind = shugaku_birth_ss_serifu_kind;
    shugaku_birth_last_sss_sound = shugaku_birth_sss_sound;
    shugaku_birth_ss_serifu = shugaku_birth_serifu_tbl[11];
    shugaku_birth_ss_serifu_kind = shugaku_birth_serifu_kind_tbl[11];
    shugaku_birth_sss_sound = shugaku_birth_sound_tbl[11];
    shugaku_birth_now_serifu = (s16)(now_girl * 3 + shugaku_gakunen);
    sub_sub_step_up();
}
