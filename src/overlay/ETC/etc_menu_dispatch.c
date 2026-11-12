/* @SymbolSize: 832 */
/* @CompileEnv: IDO52_LDA_NOINREG_CLASSES=4 */
/* @RodataBase: 0x8014C3A0 */
/* @PostProc: patch_insn_reg_field 0x90:rt:v0:t1 0x94:rt:t1:t2 0x98:rd:v0:t1 0x98:rs:v0:t1 0x9c:rs:v0:t1 0x9c:rt:v0:t1 0xa0:rd:t2:t3 0xa0:rt:t1:t2 0xa4:rd:t3:t4 0xa4:rs:v0:t1 0xa4:rt:t2:t3 0xa8:rs:t3:t4 0xb4:rt:t4:t5 0xc0:rt:t4:t5 0xc4:rt:t5:t6 0xc8:rs:t5:t6 0xc8:rt:t5:t6 0xd0:rs:t5:t6 0xd4:rd:t7:t8 0xe4:rt:t6:t7 0xf0:rt:t6:t7 0xf4:rt:v0:t9 0xf8:rt:t8:t0 0xfc:rd:v0:t9 0xfc:rs:v0:t9 0xfc:rt:t7:t8 0x100:rs:v0:t9 0x100:rt:v0:t9 0x104:rd:t9:t2 0x104:rt:t8:t0 0x108:rd:t0:t1 0x108:rs:v0:t9 0x108:rt:t9:t2 0x10c:rs:t0:t1 0x118:rt:t1:t3 0x124:rt:t1:t3 0x128:rt:t2:t4 0x12c:rs:t2:t4 0x12c:rt:t2:t4 0x134:rs:t2:t4 0x138:rd:t4:t6 0x148:rt:t3:t5 0x154:rt:t3:t5 0x158:rt:v0:t7 0x15c:rt:t5:t8 0x160:rd:v0:t7 0x160:rs:v0:t7 0x160:rt:t4:t6 0x164:rs:v0:t7 0x164:rt:v0:t7 0x168:rd:t6:t0 0x168:rt:t5:t8 0x16c:rd:t7:t9 0x16c:rs:v0:t7 0x16c:rt:t6:t0 0x170:rs:t7:t9 0x17c:rt:t8:t2 0x188:rt:t8:t2 0x18c:rd:t9:t1 0x190:rt:v0:t3 0x194:rt:t0:t4 0x198:rd:v0:t3 0x198:rs:v0:t3 0x198:rt:t9:t1 0x19c:rs:v0:t3 0x19c:rt:v0:t3 0x1a0:rd:t1:t5 0x1a0:rt:t0:t4 0x1a4:rd:t2:t6 0x1a4:rs:v0:t3 0x1a4:rt:t1:t5 0x1a8:rs:t2:t6 0x1b4:rt:t3:t8 0x1c0:rt:t3:t8 0x1c4:rt:t5:t0 0x1c8:rs:t5:t0 0x1c8:rt:t5:t0 0x1cc:rd:t4:t7 0x1d0:rt:a1:v1 0x1d4:rd:v1:v0 0x1d4:rs:t4:t7 0x1d4:rt:t5:t0 0x1d8:rs:v1:v0 0x1d8:rt:v0:t2 0x1dc:rd:t6:t9 0x1dc:rt:a1:v1 0x1e0:rd:t7:t1 0x1e0:rs:v0:t2 0x1e0:rt:t6:t9 0x1e4:rs:t7:t1 0x1f0:rd:a1:v1 0x1f0:rs:t6:t9 0x21c:rt:t8:t4 0x228:rt:t8:t4 0x234:rt:t9:t3 0x240:rt:t9:t3 0x26c:rt:t0:t5 0x278:rt:t0:t5 0x284:rt:t1:t6 0x290:rt:t1:t6 0x29c:rt:t4:t0 0x2c0:rt:t2:t8 0x2cc:rt:t2:t8 0x2d8:rt:t3:t7 0x2e4:rt:t3:t7 0x2e8:rs:t4:t0 0x2e8:rt:t4:t0 0x2ec:rs:v1:v0 0x2ec:rt:t4:t0 0x2f0:rt:t5:t9 0x2f4:rs:t5:t9 0x2f4:rt:t5:t9 0x2f8:rs:v1:v0 0x2f8:rt:t5:t9 0x2fc:rt:t6:t2 0x300:rs:t6:t2 0x300:rt:t6:t2 0x304:rs:v1:v0 0x304:rt:t6:t2 0x308:rt:t7:t1 0x30c:rs:t7:t1 0x30c:rt:t7:t1 0x310:rs:v1:v0 0x310:rt:t7:t1 0x314:rt:t8:t4 0x318:rs:t8:t4 0x318:rt:t8:t4 0x31c:rs:v1:v0 0x31c:rt:t8:t4 0x324:rs:a1:v1 0x334:rt:t9:t3 0x33c:rt:t9:t3 */
/* Address: 0x80132FB0 */
/* Size: 832 bytes (208 instructions) */
#include "types.h"

extern s32 value_cache_find_or_load(s32, s32);
extern s32 etc_bg_req;
extern s32 *etc_menu_val_tbl_tbl[];
extern s32 etc_menu_val_tbl_tbl_w;
extern s32 etc_bg_tbl;
extern s32 etc_bg_tbl_1;
extern s32 etc_bg_tbl_10;
extern s32 etc_bg_tbl_11;
extern s32 etc_bg_tbl_13;
extern s32 etc_rec_year;

void etc_menu_dispatch(s32 a0, s32 a1, s32 a2_unused, s32 a3)
{
    s32 *table;
    s32 value;
    s32 a2;
    s32 **slot2;
    s32 idx4;

    etc_bg_req = 0;
    a2 = a0;

    if (a0 != 20) {
        goto general_path;
    }

    switch (a1) {
    case 1: case 2: case 3:
        if (etc_rec_year == 97) {
            value_cache_find_or_load(16628, 1);
            etc_bg_req = 1;
            return;
        }
        table = ((s32 **)((u8 *)etc_menu_val_tbl_tbl + 4))[a2];
        value = table[a1];
        value_cache_find_or_load(value, 1);
        etc_bg_req = 1;
        return;
    case 4:
        if (etc_rec_year == 97) {
            value_cache_find_or_load(18044, 1);
            etc_bg_req = 1;
            return;
        }
        table = ((s32 **)((u8 *)etc_menu_val_tbl_tbl + 4))[a2];
        value = table[a1];
        value_cache_find_or_load(value, 1);
        etc_bg_req = 1;
        return;
    case 17: case 18: case 19: case 20:
        if (etc_rec_year == 96) {
            value_cache_find_or_load(16664, 1);
            etc_bg_req = 1;
            return;
        }
        table = ((s32 **)((u8 *)etc_menu_val_tbl_tbl + 4))[a2];
        value = table[a1];
        value_cache_find_or_load(value, 1);
        etc_bg_req = 1;
        return;
    case 5: case 6: case 7: case 8: case 9:
    case 10: case 11: case 12: case 13: case 14:
    case 15: case 16:
    default:
        table = ((s32 **)((u8 *)etc_menu_val_tbl_tbl + 4))[a2];
        value = table[a1];
        value_cache_find_or_load(value, 1);
        etc_bg_req = 1;
        return;
    }

general_path:
    slot2 = (s32 **)&etc_bg_tbl + a2;
    table = *slot2;
    idx4 = a1 * 4;
    value = *(s32 *)((u8 *)table + idx4);
    if (value == -1) {
        return;
    }
    if (value == 17131) {
        if (a3 == 2 || a3 == 3) {
            value_cache_find_or_load(17140, 1);
            etc_bg_req = 1;
            return;
        }
        value_cache_find_or_load(value, 1);
        etc_bg_req = 1;
        return;
    }
    if (value == 17149) {
        if (a3 == 2 || a3 == 3) {
            value_cache_find_or_load(17158, 1);
            etc_bg_req = 1;
            return;
        }
        value_cache_find_or_load(value, 1);
        etc_bg_req = 1;
        return;
    }
    if (value == 17167) {
        if (a3 == 2 || a3 == 3) {
            value_cache_find_or_load(17176, 1);
            etc_bg_req = 1;
            return;
        }
        value_cache_find_or_load(value, 1);
        etc_bg_req = 1;
        return;
    }

    if (slot2 == (s32 **)&etc_bg_tbl || slot2 == (s32 **)&etc_bg_tbl_1 ||
        slot2 == (s32 **)&etc_bg_tbl_11 || slot2 == (s32 **)&etc_bg_tbl_10 ||
        slot2 == (s32 **)&etc_bg_tbl_13) {
        if (idx4 == 0) {
            return;
        }
    }
    value_cache_find_or_load(value, 1);
    etc_bg_req = 1;
}
