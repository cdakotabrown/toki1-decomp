/* @CompileFlags: -Wc,-pic0 */
/* Function: date_rank_grade_get */
/* Overlay: DATE */
/* Address: 0x8013C3AC */
/* Size: 532 bytes (133 instructions) */
/* @CompileEnv: IDO52_CONST_NOINREG_VALUES=2,3 */
/* @PostProc: patch_insn_reg_field 0x68:rd:v1:v0 0x60:rt:v0:v1 0x64:rs:v0:v1 0x64:rt:v0:v1 0x74:rs:v1:v0 0x80:rs:v0:v1 0x8c:rt:v1:v0 0x94:rt:v0:v1 0x98:rs:v0:v1 0x98:rt:v0:v1 0xa4:rs:v1:v0 0xa4:rt:v1:v0 0xa8:rt:v1:v0 0xac:rd:v1:v0 0xb0:rs:v1:v0 0xbc:rs:v0:v1 0xc0:rt:t2:v0 0xc8:rs:t2:v0 0xc8:rt:t2:v0 0xcc:rt:t4:t3 0xd0:rd:t8:t2 0xd0:rt:t2:v0 0xd4:rd:t8:t2 0xd4:rs:t8:t2 0xd4:rt:t2:v0 0xd8:rd:t8:t2 0xd8:rt:t8:t2 0xdc:rs:t4:t3 0xdc:rt:t4:t3 0xe0:rd:t8:t2 0xe0:rs:t8:t2 0xe0:rt:t2:v0 0xe4:rd:t3:t8 0xe4:rt:t8:t2 0xe8:rd:t5:t4 0xe8:rs:t3:t8 0xe8:rt:t4:t3 0xec:rs:t5:t4 0xec:rt:v0:v1 0xf0:rt:t6:t5 0xf4:rs:v0:v1 0x100:rt:t6:t5 0x104:rs:v0:v1 0x108:rt:t7:t6 0x110:rs:v0:v1 0x11c:rt:t7:t6 0x124:rs:v0:v1 0x128:rt:t9:t1 0x12c:rt:t1:t7 0x134:rt:t1:t7 0x13c:rt:t9:t1 0x140:rt:v0:v1 0x144:rs:v0:v1 0x144:rt:v0:v1 0x148:rd:v1:v0 0x154:rs:v1:v0 0x158:rd:t2:t9 0x15c:rd:t8:t2 0x15c:rt:t2:t9 0x160:rs:v0:v1 0x160:rt:t8:t2 0x168:rs:v1:v0 0x168:rt:t3:t8 0x16c:rt:t3:t8 0x170:rt:v0:v1 0x174:rs:v0:v1 0x174:rt:v0:v1 0x180:rs:v1:v0 0x180:rt:v1:v0 0x184:rd:t4:t3 0x184:rt:v1:v0 0x188:rd:v1:v0 0x188:rt:t4:t3 0x18c:rs:v1:v0 0x198:rs:v0:v1 0x19c:rt:t7:v0 0x1a0:rt:t6:t5 0x1a4:rt:t6:t5 0x1a8:rs:t7:v0 0x1a8:rt:t7:v0 0x1ac:rt:t2:t1 0x1b0:rd:t1:t6 0x1b0:rt:t7:v0 0x1b4:rd:t1:t6 0x1b4:rs:t1:t6 0x1b4:rt:t7:v0 0x1b8:rd:t1:t6 0x1b8:rt:t1:t6 0x1bc:rs:t2:t1 0x1bc:rt:t2:t1 0x1c0:rd:t1:t6 0x1c0:rs:t1:t6 0x1c0:rt:t7:v0 0x1c4:rd:t9:t7 0x1c4:rt:t1:t6 0x1c8:rd:t8:t9 0x1c8:rs:t9:t7 0x1c8:rt:t2:t1 0x1cc:rs:t8:t9 0x1cc:rt:v0:v1 0x1d0:rt:t3:t2 0x1d4:rs:v0:v1 0x1e0:rt:t3:t2 0x1e4:rs:v0:v1 0x1e8:rt:t4:t8 0x1f0:rs:v0:v1 0x1fc:rt:t4:t8 0x204:rs:v0:v1 0x208:rt:t6:t4 0x20c:rt:t5:t3 0x214:rt:t5:t3 0x218:rt:t6:t4 */
#include "types.h"

typedef struct {
    u8 cell[11][21];
} DateTable;

extern DateTable date_rank_grade_get_tbl;
extern s16 para_zat;
extern u8 now_girl;
extern u8 now_basyo;

void date_rank_grade_get(s16 mode, s32 *outA, s32 *outB) {
    s32 spareHigh[2];
    DateTable tbl;
    s32 spareLow[2];
    s16 i;
    s32 pct;
    s32 cell;

    tbl = date_rank_grade_get_tbl;

    if (mode > 0) {
        pct = para_zat;
        for (i = 0; i < 5; i++) {
            if (pct >= 100 - i * 25) {
                *outA = 5 - i;
                pct = para_zat;
                break;
            }
        }
        if (pct == 0) {
            *outA = 0;
        }

        cell = tbl.cell[now_girl][now_basyo];
        if (cell == 0) {
            *outB = 5;
        } else if (cell == 1 || cell == 2) {
            *outB = 4;
        } else if (cell == 3) {
            *outB = 3;
        } else {
            *outB = 2;
        }
        return;
    }

    pct = para_zat;
    for (i = 0; i < 5; i++) {
        if (pct >= 100 - i * 25) {
            *outA = i + 3;
            pct = para_zat;
            break;
        }
    }
    if (pct == 0) {
        *outA = 8;
    }

    cell = tbl.cell[now_girl][now_basyo];
    if (cell == 0) {
        *outB = 3;
    } else if (cell == 1 || cell == 2) {
        *outB = 4;
    } else if (cell == 3) {
        *outB = 5;
    } else {
        *outB = 6;
    }
}
