/* @SymbolStart: 0x8013ADFC */
/* @SymbolSize: 192 */
/* @PostProc: patch_insn_reg_field 0x94:rt:t3:t4 0x98:rs:t3:t4 */
/* Function: date_machiawase_serifu_run (absorbs cross-prologue head func_8013ADFC) */
/* Overlay: DATE */
/* Address: 0x8013ADFC (head) / 0x8013AE40 (labelled entry) */
/* Size: 192 bytes (48 instructions: 17-insn hoisted head + 31-insn body) */
#include "types.h"

extern s32 get_g_zyotai_s(s32);
extern void bustup_face_change(s32);
extern void serifu_main(s32, s32, s32);

extern volatile u8 now_girl;

extern volatile s16 date_now_serifu_machi;
extern volatile s16 date_now_serifu_machi_in;


extern u8 date_machi_mode;

extern volatile u8 serifu_no;

extern s32 now_s;
extern s32 now_s_in;
extern s32 ss_s;
extern s32 ss_s_kind;
extern s32 sss_so;
extern s32 date_serifu_tbl[];
extern s32 date_serifu_kind_tbl[];
extern s32 date_sound_tbl[];

void date_machiawase_serifu_run(void) {
    s32 v0;
    s32 t4;

    now_s = (s32)&date_now_serifu_machi;
    now_s_in = (s32)&date_now_serifu_machi_in;
    ss_s = date_serifu_tbl[23];
    ss_s_kind = date_serifu_kind_tbl[23];
    sss_so = date_sound_tbl[23];

    v0 = get_g_zyotai_s(now_girl);

    if (date_machi_mode == 2u) {
        if (date_now_serifu_machi_in == 2) {
            t4 = v0 & 0x7F;
            if ((u32)t4 >= 2u) {
                bustup_face_change(3);
            }
        }
    }

    serifu_main(serifu_no, 1, 0);
}
