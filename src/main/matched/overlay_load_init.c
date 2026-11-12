/* Function: overlay_load_init */
/* Address: 0x80078CD8  (registered address corrected 2026-09-17; see below) */
/* Size: 916 bytes (229 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* @CompileFlags: -Wc,-pic1 */
/* @SymbolStart: 0x80078CD8 */
/* @SymbolSize: 916 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: swap_instructions --offset1 0x344 --offset2 0x34C */
/* @PostProc: swap_instructions --offset1 0x34C --offset2 0x368 */
/* @PostProc: swap_instructions --offset1 0x368 --offset2 0x36C */
/* @PostProc: patch_insn_reg_field 0x0:rt:t6:a2 0x4:rs:t6:a2 0x4:rt:t6:a2 0x18:rt:t6:a2 0x1c:rt:t7:t6 0x20:rs:t7:t6 0x20:rt:t7:t6 0x2c:rt:t7:t6 0x50:rt:t8:t7 0x54:rs:t8:t7 */
/* @PostProc: patch_insn_reg_field 0x5c:rt:t8:t7 0x5c:rd:t8:t7 0x64:rt:t8:t7 0x68:rt:t8:t7 0x70:rs:t8:t7 0xb4:rt:t9:t8 0xc0:rt:t9:t8 0xc4:rt:t0:t9 0xc8:rs:t0:t9 0xc8:rt:t0:t9 */
/* @PostProc: patch_insn_reg_field 0xd0:rs:t0:t9 0xd0:rt:t1:t0 0xd4:rs:t1:t0 0xd8:rt:t2:t1 0xe0:rt:t2:t1 0xec:rt:t3:t2 0x100:rt:t6:t5 0x108:rt:t7:t6 0x11c:rt:t7:t6 0x124:rt:t7:t6 */
/* @PostProc: patch_insn_reg_field 0x138:rt:t7:t6 0x140:rt:t7:t6 0x144:rs:t3:t2 0x14c:rt:t3:t2 0x14c:rd:t3:t2 0x154:rt:t3:t2 0x158:rt:t3:t2 0x160:rs:t3:t2 0x168:rt:t4:t3 0x170:rt:t4:t3 */
/* @PostProc: patch_insn_reg_field 0x17c:rt:t5:t4 0x184:rt:t5:t4 0x190:rt:t6:t5 0x198:rt:t6:t5 0x1a4:rt:t7:t6 0x1ac:rt:t7:t6 0x1b8:rt:t8:t7 0x1c0:rt:t8:t7 0x1cc:rt:t9:t8 0x1d4:rt:t9:t8 */
/* @PostProc: patch_insn_reg_field 0x1e0:rt:t0:t9 0x1e8:rt:t0:t9 0x1f4:rt:t1:t0 0x1fc:rt:t1:t0 0x208:rt:t2:t1 0x210:rt:t2:t1 0x21c:rt:t3:t2 0x224:rt:t3:t2 0x230:rt:t4:t3 0x238:rt:t4:t3 */
/* @PostProc: patch_insn_reg_field 0x244:rt:t5:t4 0x24c:rt:t5:t4 0x258:rt:t6:t5 0x260:rt:t6:t5 0x26c:rt:t7:t6 0x274:rt:t7:t6 0x280:rt:t8:t7 0x288:rt:t8:t7 0x294:rt:t9:t8 0x29c:rt:t9:t8 */
/* @PostProc: patch_insn_reg_field 0x2a8:rt:t0:t9 0x2b0:rt:t0:t9 0x2bc:rt:t1:t0 0x2c4:rt:t1:t0 0x2d0:rt:t2:t1 0x2d8:rt:t2:t1 0x2e4:rt:t3:t2 0x2ec:rt:t3:t2 0x2f8:rt:t4:t3 0x300:rt:t4:t3 */
/* @PostProc: patch_insn_reg_field 0x30c:rt:t5:t4 0x314:rt:t5:t4 0x324:rt:t6:t5 0x330:rt:t7:t6 0x338:rt:t7:t6 0x340:rt:t8:t7 0x348:rs:t8:t7 0x35c:rt:t9:t8 0x364:rt:t9:t8 0x378:rt:t0:t9 */
/* @PostProc: patch_insn_reg_field 0x380:rt:t0:t9 */
#include "common.h"

extern u8 prev_step;
extern u8 overlay_load_disp_tbl0;
extern u32 ending_main_tbl1;
extern u8 ending_main_tbl0;
extern s32 overlay_load_init_tbl0;
extern u8 overlay_load_main_tbl0;
extern u8 byouki_kind;
extern u32 Sprites;

extern void back_clear_switch(s32);
extern void vram_full_move(s32);
extern void cd_read_start2(s32, s32, s32);

void overlay_load_init(void) {
    u8 charId;
    s32 savedA2;
    s32 newA2;

    savedA2 = overlay_load_init_tbl0;
    back_clear_switch(1);
    ending_main_tbl1 = 0x80132000;
    charId = prev_step;
    overlay_load_disp_tbl0 = 0;

    switch (charId + 0U) {
    case 8:
    case 19:
    case 53:
        break;
    case 80: case 81: case 82: case 83: case 86: case 87: case 88:
    case 92: case 94: case 95: case 96: case 97: case 98: case 99:
    case 100: case 101: case 102: case 103: case 104: case 105:
    case 106: case 107: case 108: case 109: case 110: case 111:
    case 112: case 113: case 115: case 116: case 117: case 118:
    case 119: case 120: case 121: case 122: case 123: case 124:
    case 125: case 126: case 127: case 128: case 129:
        break;
    case 84: case 85: case 89: case 90: case 91: case 93: case 114:
        vram_full_move(160);
        charId = prev_step;
        overlay_load_disp_tbl0 = 1;
        break;
    default:
        break;
    }

    if ((byouki_kind & 0xF) == 3) {
        Sprites = 0x80000000;
    }

    switch (charId + 0U) {
    case 58:
        ending_main_tbl0 = 96; newA2 = 0x96AA; break;
    case 130:
        ending_main_tbl0 = 96; newA2 = 0x934A; break;
    case 116: case 117: case 118:
        ending_main_tbl0 = 96; newA2 = 0x958A; break;
    case 129:
        ending_main_tbl0 = 96; newA2 = 0x982A; break;
    case 128:
        ending_main_tbl0 = 96; newA2 = 0x91CA; break;
    case 114:
        ending_main_tbl0 = 96; newA2 = 0x99AA; break;
    case 95: case 96:
        ending_main_tbl0 = 96; newA2 = 0x952A; break;
    case 115:
        ending_main_tbl0 = 96; newA2 = 0x97CA; break;
    case 112:
        ending_main_tbl0 = 96; newA2 = 0x916A; break;
    case 113:
        ending_main_tbl0 = 96; newA2 = 0x910A; break;
    case 17: case 19: case 20: case 21:
        ending_main_tbl0 = 96; newA2 = 0x95EA; break;
    case 67: case 69:
        ending_main_tbl0 = 96; newA2 = 0x9A0A; break;
    case 99:
        ending_main_tbl0 = 96; newA2 = 0x92EA; break;
    case 53: case 54: case 55: case 145: case 194: case 195: case 196:
        ending_main_tbl0 = 96; newA2 = 0x93AA; break;
    case 80: case 94:
        ending_main_tbl0 = 96; newA2 = 0x922A; break;
    case 88: case 92:
        ending_main_tbl0 = 96; newA2 = 0x928A; break;
    case 97: case 98:
        ending_main_tbl0 = 96; newA2 = 0x970A; break;
    case 86: case 87: case 89: case 90: case 93:
        ending_main_tbl0 = 96; newA2 = 0x946A; break;
    case 81:
        ending_main_tbl0 = 96; newA2 = 0x98EA; break;
    case 84:
        ending_main_tbl0 = 96; newA2 = 0x9ACA; break;
    case 82: case 83: case 85: case 91:
        ending_main_tbl0 = 96; newA2 = 0x988A; break;
    case 32: case 33: case 34:
        ending_main_tbl0 = 96; newA2 = 0x976A; break;
    case 208:
        ending_main_tbl0 = 96; newA2 = 0x9A6A; break;
    default:
        ending_main_tbl0 = 96; newA2 = 0x994A; break;
    }

    if (savedA2 == newA2) {
        overlay_load_init_tbl0 = newA2;
        overlay_load_main_tbl0 = 3;
    } else {
        overlay_load_init_tbl0 = newA2;
        cd_read_start2(96, ending_main_tbl1, newA2);
        overlay_load_main_tbl0 = 1;
    }
}
