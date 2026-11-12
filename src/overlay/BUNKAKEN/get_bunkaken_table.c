/* Address: 0x80152F90 */
/* Size: 248 bytes (62 insns) */
/* @SymbolSize: 248 */
/* @PostProc: swap_instructions --offset1 0xc8 --offset2 0xcc */
/* @PostProc: swap_instructions --offset1 0xd0 --offset2 0xd4 */
/* @PostProc: patch_insn_reg_field 0xb0:rt:v1:t8 0xc0:rd:v1:t8 0xc0:rs:v1:t8 0xc4:rt:t6:t1 0xc8:rs:t6:t1 0xc8:rt:t6:t1 0xcc:rs:v1:t8 0xcc:rt:v1:t8 0xd0:rd:t1:t2 0xd0:rt:t6:t1 0xd4:rd:t8:t0 0xd4:rt:v1:t8 0xd8:rd:t0:t7 0xd8:rt:t8:t0 0xdc:rd:t2:t3 0xdc:rt:t1:t2 0xe0:rd:t3:t4 0xe0:rt:t2:t3 0xe4:rd:t7:t6 0xe4:rt:t0:t7 0xe8:rd:t4:t5 0xe8:rs:t7:t6 0xe8:rt:t3:t4 0xec:rd:v0:v1 0xec:rt:t4:t5 0xf0:rs:v0:v1 */
#include "types.h"

typedef struct {
    s32 t[4][4];
} Tbl4x4;

typedef struct {
    s32 flags;
    char pad[52];
} Ent56;

extern Tbl4x4 get_bunkaken_table_tbl;
extern s32 bunka_girl;
extern u8  girl2_basyo;
extern s32 bunka_girl2;
extern s32 p_birthday;
extern Ent56 girl0_birth[];

extern s32 get_g_zyotai_low(s32);

s32 get_bunkaken_table(void) {
    s32 row;
    s32 col;
    Tbl4x4 tbl;

    tbl = get_bunkaken_table_tbl;
    if (bunka_girl == 2 && ((u32)girl2_basyo >> 4) == 3 &&
        (u32)get_g_zyotai_low(2) < 4 && bunka_girl2 != 2) {
        return 3;
    }
    col = (u32)(p_birthday << 21) >> 30;
    row = (u32)(girl0_birth[bunka_girl].flags << 21) >> 30;
    return tbl.t[row][col];
}
