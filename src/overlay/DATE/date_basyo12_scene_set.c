/* @SymbolSize: 288 */
/* @PostProc: scheduler_normalizer --target-bin assets/overlays/DATE.EXN --address 0x801399b4 --block-aware --reloc-symbol-fp */
/* @PostProc: patch_insn_reg_field 0x34:rt:a1:t9 0x38:rs:a1:t9 0x38:rt:a1:t9 0x3c:rt:a0:v1 0x40:rs:a1:t9 0x40:rt:a0:v1 0x44:rt:t0:t1 0x48:rs:t0:t1 0x48:rt:t0:t1 0x64:rd:t9:t0 0x68:rd:t1:t2 0x68:rs:t9:t0 0x68:rt:t0:t1 0x6c:rd:t2:t3 0x6c:rs:t1:t2 0x70:rs:a1:t9 0x70:rt:a0:v1 0x74:rs:t2:t3 0x74:rt:t3:t4 0x7c:rt:t3:t4 0x84:rd:t4:t5 0x88:rd:t5:t6 0x88:rs:t4:t5 0x88:rt:t0:t1 0x8c:rd:t6:t7 0x8c:rs:t5:t6 0x90:rs:a1:t9 0x90:rt:a0:v1 0x94:rs:t6:t7 0x94:rt:t7:t8 0x98:rt:t4:t6 0x9c:rs:t4:t6 0x9c:rt:t4:t6 0xa0:rt:t7:t8 0xa8:rd:t8:t0 0xac:rd:t9:t2 0xac:rs:t8:t0 0xac:rt:t0:t1 0xb0:rd:t1:t3 0xb0:rs:t9:t2 0xb4:rs:a1:t9 0xb4:rt:a0:v1 0xb8:rs:t1:t3 0xb8:rt:t2:t4 0xc0:rt:t2:t4 0xc8:rd:t3:t5 0xcc:rd:t5:t7 0xcc:rs:t3:t5 0xcc:rt:t4:t6 0xd0:rd:t6:t8 0xd0:rs:t5:t7 0xd4:rs:a1:t9 0xd4:rt:a0:v1 0xd8:rs:t6:t8 0xd8:rt:t7:t0 0xe0:rt:t7:t0 0xe8:rd:t8:t1 0xec:rd:t0:t2 0xec:rs:t8:t1 0xec:rt:t4:t6 0xf0:rd:t9:t3 0xf0:rs:t0:t2 0xf4:rs:a1:t9 0xf4:rt:a0:v1 0xf8:rs:t9:t3 0xf8:rt:t1:t4 0x100:rt:t1:t4 0x108:rd:t2:t5 0x10c:rd:t3:t7 0x10c:rs:t2:t5 0x10c:rt:t4:t6 0x110:rd:t5:t8 0x110:rs:t3:t7 0x114:rs:t5:t8 0x114:rt:t6:t0 0x11c:rt:t6:t0 */
/* Function: date_basyo12_scene_set */
/* Overlay: DATE */
/* Address: 0x801399B4 */
/* Size: 288 bytes (72 instructions) */
#include "types.h"

extern volatile u8 now_sub_sub_step;
extern volatile u8 now_girl;
extern u8 date_pattern;
extern s16 imp[];
extern s16 kao[];
extern u8 *date_impkao_tbl_18_imp;
extern u8 *date_impkao_tbl_18_kao;

extern void sub_sub_step_up(void);
extern void date_trans_set(void);

void date_basyo12_scene_set(void)
{
    u8 *p1;
    u8 *p2;
    u8 g;
    s32 c3;

    sub_sub_step_up();
    now_sub_sub_step = (u8)(now_sub_sub_step + 22);
    g = now_girl;
    p1 = date_impkao_tbl_18_imp;
    c3 = date_pattern * 3;
    imp[0] = (p1 + g * 36U)[c3 + 30];
    imp[1] = (p1 + (u32)g * 36U)[c3 + 31];
    p2 = date_impkao_tbl_18_kao;
    imp[2] = (p1 + (g | 0U) * 36U)[c3 + 32];
    kao[0] = (p2 + (g ^ 0U) * 36U)[c3 + 30];
    kao[1] = (p2 + (g * 1U) * 36U)[c3 + 31];
    kao[2] = (p2 + (g >> 0) * 36U)[c3 + 32];
    date_trans_set();
}
