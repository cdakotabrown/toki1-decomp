/* Address: 0x80135F3C */
/* Size: 324 bytes (81 insns) */
/* @CompileEnv: IDO52_CONST_NOINREG_VALUES=20 */
/* @PostProc: patch_stack_complete --old 304 --new 312 */
/* @PostProc: remap_stack_slots --map '84:76' */
/* @PostProc: swap_instructions --offset1 0x78 --offset2 0x7c */
/* @PostProc: patch_insn_reg_field 0xa0:rt:a1:v0 0xa4:rs:a1:v0 0xa4:rt:a1:v0 0xc4:rt:a1:v0 0xc8:rt:a1:v0 0xe8:rd:v0:t8 0xec:rd:t8:t7 0xec:rt:v0:t8 0xf0:rs:t8:t7 0xf0:rt:v0:t8 0xf8:rd:t9:t7 0xfc:rd:t6:t9 0xfc:rt:t9:t7 0x100:rd:t0:t6 0x104:rd:t1:t0 0x104:rs:t6:t9 0x104:rt:t0:t6 0x10c:rs:t1:t0 */
#include "types.h"

typedef struct {
    s32 w[55];
} Blk55;

extern s32 rand(void);
extern Blk55 taiiku_crowd_set_tbl;
extern s16 taiiku_kari_card;
extern u8 class_w[];

void taiiku_crowd_set(void)
{
    Blk55 buf;
    s32 outer;
    s32 mid;
    s32 skip;
    s32 idx;
    s32 sel;
    s32 row;

    buf = taiiku_crowd_set_tbl;

    idx = 0;
    outer = 0;
    do {
        skip = rand() % 3;
        sel = taiiku_kari_card;
        mid = 0;
        do {
            if (skip != mid) {
                row = outer * 3U;
                *(s16 *)(class_w + (((row << 4) + row) << 2) +
                         mid * 68U + 3150) =
                    ((s16 *)((u8 *)&buf + sel * 20))[idx];
                idx++;
                if (idx >= 10) {
                    idx = 0;
                }
            }
            mid++;
        } while (mid != 3);
        outer++;
    } while (outer != 10);
}
