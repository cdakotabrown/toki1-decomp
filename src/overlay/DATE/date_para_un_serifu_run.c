/* @SymbolSize: 648 */
/* @PostProc: patch_stack_complete --old 56 --new 48 */
/* @PostProc: remap_stack_slots --from 54 --to 42 --from 52 --to 40 */
/* Function: date_para_un_serifu_run */
/* Overlay: DATE */
/* Address: 0x8013B1AC */
/* Size: 648 bytes (162 instructions) */
/* @CompileEnv: IDO52_FORCE_WEB_NOSPILL=142 IDO52_FORCE_WEB_COLOR_NOSPLIT=142 IDO52_CONST_DEF_BB=142:29:+,142:39:- */
/* @PostProc: swap_instructions --offset1 0x6c --offset2 0x78 */
/* @PostProc: swap_instructions --offset1 0x150 --offset2 0x154 */
/* @PostProc: swap_instructions --offset1 0x154 --offset2 0x15c */
/* @PostProc: swap_instructions --offset1 0x158 --offset2 0x164 */
/* @PostProc: swap_instructions --offset1 0x160 --offset2 0x164 */
/* @PostProc: patch_insn_reg_field 0x6c:rt:a0:a1 0x7c:rs:a0:a1 0x7c:rt:a0:a1 0x80:rt:v1:a0 0x84:rs:a0:a1 0x84:rt:v1:a0 0xac:rs:v0:a0 0xac:rt:v1:v0 0x108:rt:a3:v1 0x10c:rs:a3:v1 0x10c:rt:a3:v1 0x130:rs:a3:v1 0x170:rt:a2:a0 0x180:rs:a0:a1 0x198:rs:v0:a0 0x198:rt:a2:v0 0x1c8:rs:a0:a1 0x23c:rt:a3:v1 0x240:rs:a3:v1 0x240:rt:a3:v1 0x264:rs:a3:v1 */
/* @PostProc: patch_insn_reg_field 0x38:rt:a3:v1 0x44:rs:a3:v1 0x44:rt:a3:v1 0x5c:rs:a3:v1 0x64:rt:v1:a3 0x68:rs:v1:a3 0x68:rt:v1:a3 0x70:rs:v1:a3 0x88:rt:a1:a2 0x8c:rs:a1:a2 0x8c:rt:a1:a2 0x94:rs:a1:a2 0x14c:rt:v1:v0 0x154:rt:a0:a2 0x158:rt:v0:a3 0x15c:rs:v1:a3 0x15c:rt:v1:a3 0x160:rs:a1:a2 0x160:rt:a1:a2 0x164:rs:a0:a1 0x164:rt:a0:a1 0x16c:rs:a3:v1 0x174:rs:v1:a3 0x188:rs:a1:a2 0x1b8:rs:a3:v1 0x1bc:rs:v1:a3 0x1d0:rs:a1:a2 */
#include "types.h"

extern s32 sub_sub_step_up(void);
extern s32 serifu_main(s32, s32, s32);
extern void bustup_face_change(s32);

extern s32 serifu_no;
extern volatile s16 para_un;
extern volatile u8 date_pattern;
extern volatile u8 now_basyo;
extern volatile u8 now_girl;
extern s32 now_s;
extern s32 ss_s;
extern s32 ss_s_kind;
extern s32 sss_so;
extern s32 now_s_in;
extern volatile s16 imp;
extern volatile s16 kao;
extern volatile s16 kao2;
extern volatile s16 ans;
extern s32 date_serifu0_tbl[];
extern s32 date_serifu0_kind_tbl[];
extern s32 date_sound0_tbl[];

void date_para_un_serifu_run(void)
{
    s16 buf0;
    s16 buf1;
    s32 v0;
    s32 v1;
    s32 a1;
    s32 a2;
    s32 a3;
    s32 one;

    now_s = (s32)&buf0;
    ss_s = date_serifu0_tbl[2];
    ss_s_kind = date_serifu0_kind_tbl[2];
    v1 = serifu_no;
    sss_so = date_sound0_tbl[2];
    buf0 = 0;

    if (v1 != 0) {
        goto L79;
    }
    a3 = para_un;
    if (a3 >= 40) {
        goto L79;
    }
    a1 = date_pattern;
    if ((u32)a1 != 1U) {
        goto L79;
    }
    a2 = now_basyo;
    if (a2 != 16) {
        goto L79;
    }
    v0 = now_girl;
    if (v0 == 0) {
        goto L50;
    }
    if ((u32)v0 == 1U) {
        goto L50;
    }
    if (v0 == 3) {
        goto L50;
    }
    if (v0 == 4) {
        goto L50;
    }
    if (v0 == 5) {
        goto L50;
    }
    if (v0 == 8) {
        goto L50;
    }
    if (v0 != 9) {
        goto L79;
    }
L50:
    imp = 3;
    kao = 3;
    kao2 = 3;
    ans = 2;
    bustup_face_change(3);
    v1 = serifu_no;
    buf1 = 5;
    now_s_in = (s32)&buf1;
    if (serifu_main(v1 & 0xFF, 0, 0) == 0) {
        return;
    }
    sub_sub_step_up();
    return;

L79:
    a3 = para_un;
    a2 = now_basyo;
    a1 = date_pattern;
    v0 = now_girl;
    one = 1;
    if (v1 != 0) {
        goto L106;
    }
    if (a3 >= 40) {
        goto L106;
    }
    if (a1 != 0) {
        goto L106;
    }
    if (a2 != 19) {
        goto L106;
    }
    if (v0 == 0) {
        goto L125;
    }
    if (v0 == one) {
        goto L125;
    }
    if (v0 == 3) {
        goto L125;
    }
    if (v0 == 6) {
        goto L125;
    }
    if (v0 == 7) {
        goto L125;
    }
L106:
    if (v1 != 0) {
        goto L156;
    }
    if (a3 >= 100) {
        goto L156;
    }
    if (a1 != 0) {
        goto L156;
    }
    if (a2 != 20) {
        goto L156;
    }
    if (v0 == 0) {
        goto L125;
    }
    if (v0 == 4) {
        goto L125;
    }
    if (v0 == 6) {
        goto L125;
    }
    if (v0 == 7) {
        goto L125;
    }
    if (v0 == 8) {
        goto L125;
    }
    if (v0 != 10) {
        goto L156;
    }
L125:
    imp = 3;
    kao = 3;
    kao2 = 3;
    ans = 2;
    bustup_face_change(3);
    v1 = serifu_no;
    buf1 = 6;
    now_s_in = (s32)&buf1;
    if (serifu_main(v1 & 0xFF, 0, 0) == 0) {
        return;
    }
    sub_sub_step_up();
    return;

L156:
    sub_sub_step_up();
}
