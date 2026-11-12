/* @SymbolSize: 204 */
/* @PostProc: scheduler_normalizer --fence 0-26,27-35,36-50 */
/* @PostProc: patch_insn_reg_field 0x28:rt:a3:t0 0x2c:rs:a3:t0 0x2c:rt:a3:t0 0x30:rt:v1:t6 0x34:rs:a3:t0 0x38:rt:t2:t1 0x3c:rs:v1:t6 0x3c:rt:t6:t7 0x40:rs:v1:t6 0x44:rt:t9:t8 0x48:rs:t6:t7 0x4c:rd:t3:t2 0x4c:rt:t2:t1 0x50:rd:t3:t2 0x50:rs:t3:t2 0x50:rt:t2:t1 0x54:rd:t0:t9 0x54:rt:t9:t8 0x58:rd:t4:t3 0x58:rs:t0:t9 0x58:rt:t3:t2 0x5c:rs:t4:t3 0x5c:rt:t5:t4 0x64:rt:t5:t4 0x68:rt:t8:t5 0x6c:rs:t6:t5 0x6c:rt:t6:t5 0x74:rt:t6:t5 0x7c:rs:t8:t6 0x7c:rt:t8:t6 0x8c:rt:t8:t6 0x94:rt:t9:t8 0x98:rs:t9:t8 0x98:rt:t9:t8 0xa4:rt:t1:t0 0xa8:rs:t1:t0 0xa8:rt:t1:t0 0xb0:rt:t9:t8 0xc8:rt:t1:t0 */
/* Function: date_year_serifu_run */
/* Overlay: DATE */
/* Address: 0x80140BF8 */
/* Size: 204 bytes (51 instructions) */
#include "types.h"

extern s32 get_g_zyotai_s(s32);
extern s32 serifu_main(s32, s32, s32);

extern volatile u8 now_girl;
extern volatile u8 now_year;

extern volatile s16 now_serifue;
extern volatile s16 now_serifue_in;


extern volatile u8 serifu_no;

extern volatile s32 now_s;
extern volatile s32 now_s_in;
extern volatile s32 ss_s;
extern volatile s32 ss_s_kind;
extern volatile s32 sss_so;
extern s32 date_serifu_tbl[];
extern s32 date_serifu_kind_tbl[];
extern s32 date_sound_tbl[];

void date_year_serifu_run(void) {
    s32 v0;
    s32 t0;
    s32 t6;

    v0 = get_g_zyotai_s(now_girl);
    t0 = now_year;
    t6 = v0 & 0x7F;

    now_serifue = (s16)(((((u32)t6 < 2u) ^ 1) + (((u32)t6 < 3u) ^ 1)) +
                       ((((u32)t0 < 96u) ^ 1) * 3) + 12);

    now_s = (s32)&now_serifue;
    now_s_in = (s32)&now_serifue_in;
    ss_s = date_serifu_tbl[22];
    ss_s_kind = date_serifu_kind_tbl[22];
    sss_so = date_sound_tbl[22];

    serifu_main(serifu_no, 1, 0);
}
