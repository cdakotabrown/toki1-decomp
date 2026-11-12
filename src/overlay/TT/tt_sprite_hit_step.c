/* @SymbolSize: 688 */
/* @PostProc: swap_instructions --offset1 0x94 --offset2 0x98 */
/* @PostProc: swap_instructions --offset1 0x1e4 --offset2 0x1ec */
/* @PostProc: patch_insn_reg_field 0x18:rt:v1:a3 0x24:rs:v1:a3 0x28:rt:a3:t0 0x2c:rs:a3:t0 0x2c:rt:a3:t0 0x34:rs:v1:a3 0x6c:rt:t1:t7 0x98:rt:v1:a3 0xa0:rt:a3:t0 0xa4:rt:v1:a3 0xac:rs:v1:a3 0xac:rt:t0:t1 0xb0:rt:a3:t0 0xb4:rs:t0:t1 0xbc:rt:t1:t2 0xc0:rs:v1:a3 0xc0:rt:t1:t2 0xc4:rt:t2:t3 0xc8:rt:v0:t7 0xcc:rs:t2:t3 0xcc:rt:t3:t4 0xd0:rs:t3:t4 0xd0:rt:t4:t5 0xd4:rs:t4:t5 0xdc:rt:t3:t4 0xe0:rs:v1:a3 0xe4:rs:v0:t7 0xe4:rt:v0:t7 0xe8:rt:t5:t8 0xec:rt:t7:t6 0xf0:rt:t5:t8 0xf4:rt:t7:t6 0x110:rt:v0:t7 0x114:rs:v1:a3 0x114:rt:t8:t9 0x118:rt:t6:t1 0x11c:rd:t6:t1 0x11c:rs:t6:t1 0x11c:rt:t8:t9 0x120:rs:t6:t1 0x120:rt:t6:t1 0x128:rs:t6:t1 0x128:rt:a0:v1 0x12c:rs:a0:v1 0x130:rs:a0:v1 0x134:rt:t6:t1 0x158:rt:t4:t8 0x15c:rs:a3:t0 0x15c:rt:t9:t2 0x164:rs:t9:t2 0x174:rt:a0:v1 0x178:rs:a3:t0 0x178:rt:t0:t3 0x180:rs:t0:t3 0x194:rt:a0:v1 0x198:rs:a3:t0 0x198:rt:t1:t4 0x1a0:rs:t1:t4 0x1b0:rt:a0:v1 0x1b4:rs:a3:t0 0x1b4:rt:t2:t5 0x1bc:rs:t2:t5 0x1c8:rt:a0:v1 0x1cc:rt:t4:t8 0x1d0:rs:a0:v1 0x1d0:rt:t3:t7 0x1d4:rs:t4:t8 0x1d4:rt:t5:t6 0x1d8:rt:t3:t7 0x1e0:rt:t5:t6 0x1ec:rt:v1:a3 0x1f0:rt:v1:a3 0x1f8:rs:v1:a3 0x1f8:rt:t7:t9 0x1fc:rt:t8:t1 0x200:rs:t7:t9 0x208:rt:v0:t2 0x20c:rt:t8:t1 0x210:rs:v0:t2 0x210:rt:v0:t2 0x214:rt:t6:t3 0x218:rt:t9:t4 0x21c:rt:t0:t5 0x220:rt:t6:t3 0x228:rt:t9:t4 0x230:rt:t0:t5 0x250:rt:v0:t2 0x268:rt:t1:t7 0x270:rs:t1:t7 0x270:rt:t1:t7 0x278:rt:t1:t7 0x27c:rt:t2:t8 0x280:rt:t3:t6 0x284:rs:t2:t8 0x28c:rs:t3:t6 0x298:rs:t3:t6 0x2a0:rt:t4:t9 0x2a8:rt:t4:t9 */
/* Address: 0x8013C41C, Size: 688 bytes (172 insns) + 1 alignment nop */
#include "types.h"

extern void tt_cmd_once(s32 id);

extern u8 *tt_scroll_wk;
extern u8 *tt_player_wk;
extern u8 tt_bell_kind_tbl[];
extern s32 tt_obj_anim_tbl_1;
extern s32 tt_obj_anim_tbl_2;

void tt_sprite_hit_step(u8 *obj, s16 dir)
{
    u8 *rec;
    u8 *sys;
    s32 g;
    s32 st;

    rec = tt_scroll_wk;
    sys = tt_player_wk;
    rec[13]++;

    st = *(u16 *)(obj + 4);
    switch ((u32)st) {
    case 32:
        *(s16 *)(obj + 4) = 64;
        *(s16 *)(obj + 10) = 0;
        *(s16 *)(obj + 8) = 0;
        *(s16 *)(obj + 6) = 0;
        obj[15] = 0;
        obj[14] = 0;
        obj[13] = 0;

    case 64:
        tt_cmd_once(1547);
        if (rec[13] >= 21U) {
            rec[13] = 1;
            if (++obj[89] >= 2U) {
                rec[13] = 0;
                g = tt_obj_anim_tbl_1;
                obj[64] = 128;
                *(s16 *)(obj + 4) = 96;
                obj[13] = 0;
                obj[14] = 0;
                obj[15] = 0;
                *(s16 *)(obj + 6) = 0;
                *(s16 *)(obj + 8) = 0;
                *(s16 *)(obj + 10) = 0;
                *(s32 *)(obj + 56) = g;
            }
        }
        obj[88] = tt_bell_kind_tbl[rec[13]];
        switch (obj[88]) {
        case 1:
            if (sys[85] == 9) {
                obj[88] = 0;
            }
            break;
        case 2:
            if (sys[84] >= 10U) {
                obj[88] = 0;
            }
            break;
        case 3:
            if (sys[83] != 0) {
                obj[88] = 0;
            }
            break;
        case 6:
            if (sys[84] == 0) {
                obj[88] = 0;
            }
            break;
        }
        *(s16 *)(obj + 84) = (s16)(obj[88] + 15492);
        *(s16 *)(obj + 80) = (s16)(*(u16 *)(obj + 80) & 0xFFFE);
        break;
    case 96:
        tt_cmd_once(1547);
        if (rec[13] >= 4U) {
            *(s16 *)(obj + 84) = 15494;
            g = tt_obj_anim_tbl_2;
            obj[64] = 192;
            *(s32 *)(obj + 52) = 0;
            *(s32 *)(obj + 44) = 0x20000;
            *(s32 *)(obj + 40) = 0;
            *(s16 *)(obj + 4) = 112;
            obj[13] = 0;
            obj[14] = 0;
            obj[15] = 0;
            *(s16 *)(obj + 6) = 0;
            *(s16 *)(obj + 8) = 0;
            *(s16 *)(obj + 10) = 0;
            *(s32 *)(obj + 56) = g;
            return;
        }
        break;
    case 112:
        tt_cmd_once(1548);
        obj[0] = 0;
        break;
    }

    *(s32 *)(obj + 52) = 4096;
    *(s32 *)(obj + 44) = -98304;
    if (dir < -1) {
        *(s32 *)(obj + 40) = 4096;
    } else if (dir >= 2) {
        *(s32 *)(obj + 40) = -4096;
    } else {
        *(s32 *)(obj + 40) = 0;
    }
}
