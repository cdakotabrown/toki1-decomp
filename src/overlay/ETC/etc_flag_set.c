/* @SymbolStart: 0x8014A2E0 */
/* @SymbolSize: 468 */
/* @PostProc: swap_instructions --offset1 0x1c0 --offset2 0x1c4 */
/* @PostProc: patch_insn_reg_field 0x10:rt:t6:a1 0x14:rs:t6:a1 0x14:rt:t6:a1 0x1c:rs:t6:a1 0x60:rt:v0:a1 0x8c:rt:v0:a1 0x94:rt:v0:a1 0xb0:rt:v0:a1 0xdc:rt:v0:a1 0xe4:rt:v0:a1 0x100:rt:v0:a1 0x12c:rt:v0:a1 0x134:rt:v0:a1 0x174:rt:v0:a1 0x17c:rt:v0:a1 0x198:rt:v0:a1 0x1a0:rt:v0:a1 0x1b4:rt:v0:a1 0x1bc:rt:v0:a1 0x1c8:rt:v0:a1 */
/* Address: 0x8014A2E8 */
/* Size: 460 bytes (115 instructions) */
#include "types.h"

extern void printf(s32 *);
extern s32 get_etc_index(void);
extern s32 etc_sum_check(void);
extern u8 sinro;
extern s32 etc_siken_sel;
extern s32 etc_sinro_set_fmt[];

void etc_flag_set(void)
{
    s32 a1 = sinro;

    s32 v0;

    if (a1 == 0) {
        goto zero_path;
    }
    v0 = etc_siken_sel;
    if (v0 == 0) {
        goto def_a;
    }
    if (v0 == 1) {
        goto case1_a;
    }
    if (v0 != 2) {
        goto def_a;
    }
    if (get_etc_index() < 2) {
        a1 = 32;
        goto store;
    }
    if (get_etc_index() == 2 && etc_sum_check() != 0) {
        a1 = 32;
        goto store;
    }
    a1 = 160;
    goto store;

case1_a:
    if (get_etc_index() < 2) {
        a1 = 16;
        goto store;
    }
    if (get_etc_index() == 2 && etc_sum_check() != 0) {
        a1 = 16;
        goto store;
    }
    a1 = 144;
    goto store;

def_a:
    if (get_etc_index() < 2) {
        a1 = 8;
        goto store;
    }
    if (get_etc_index() == 2 && etc_sum_check() != 0) {
        a1 = 8;
        goto store;
    }
    a1 = 136;
    goto store;

zero_path:
    v0 = etc_siken_sel;
    if (v0 == 0) {
        goto def_b;
    }
    if (v0 == 1) {
        goto case1_b;
    }
    if (v0 != 2) {
        goto def_b;
    }
    if (get_etc_index() < 2) {
        a1 = 4;
        goto store;
    }
    a1 = 132;
    goto store;

case1_b:
    if (get_etc_index() < 2) {
        a1 = 2;
        goto store;
    }
    a1 = 130;
    goto store;

def_b:
    if (get_etc_index() < 2) {
        a1 = 1;
        goto store;
    }
    a1 = 129;

store:
    sinro = (u8)a1;
    printf(etc_sinro_set_fmt);
}
