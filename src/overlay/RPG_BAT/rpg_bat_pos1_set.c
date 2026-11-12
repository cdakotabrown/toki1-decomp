/* @RodataBase: 0x8015BE50 */
/* @SymbolSize: 292 */
/* @PostProc: scheduler_normalizer --block-aware */
/* @PostProc: swap_instructions --offset1 0xa4 --offset2 0xb4 */
/* @PostProc: patch_insn_reg_field 0x38:rt:t8:at 0x3c:rs:t8:at 0x40:rt:t0:at 0x44:rt:t9:t8 0x4c:rs:t0:at 0x4c:rt:t9:t8 0x50:rt:t1:t9 0x54:rt:t2:at 0x58:rs:t2:at 0x58:rt:t1:t9 0x5c:rt:t4:at 0x60:rt:t3:t0 0x68:rs:t4:at 0x68:rt:t3:t0 0x6c:rt:t5:t1 0x70:rt:t6:at 0x74:rs:t6:at 0x74:rt:t5:t1 0x78:rt:t8:at 0x7c:rt:t7:t2 0x84:rs:t8:at 0x84:rt:t7:t2 0x88:rt:t9:t3 0x8c:rt:t0:at 0x90:rs:t0:at 0x90:rt:t9:t3 0x94:rt:t2:at 0x98:rt:t1:t4 0xa0:rs:t2:at 0xa0:rt:t1:t4 0xa4:rt:t3:t5 0xa8:rt:t4:at 0xac:rs:t4:at 0xac:rt:t3:t5 0xb0:rt:t6:at 0xb4:rt:t5:t6 0xbc:rs:t6:at 0xbc:rt:t5:t6 0xc4:rt:t8:at 0xc8:rs:t8:at 0xcc:rt:t0:at 0xd0:rt:t9:t8 0xd8:rs:t0:at 0xd8:rt:t9:t8 0xdc:rt:t1:t9 0xe0:rt:t2:at 0xe4:rs:t2:at 0xe4:rt:t1:t9 0xe8:rt:t4:at 0xec:rt:t3:t0 0xf4:rs:t4:at 0xf4:rt:t3:t0 0xf8:rt:t5:t1 0xfc:rt:t6:at 0x100:rs:t6:at 0x100:rt:t5:t1 0x104:rt:t8:at 0x108:rt:t7:t2 0x110:rs:t8:at 0x110:rt:t7:t2 0x114:rt:t9:t3 0x118:rt:t0:at 0x11c:rs:t0:at 0x11c:rt:t9:t3 0x120:rt:t2:at 0x124:rt:t1:t4 0x128:rs:t2:at 0x128:rt:t1:t4 */
/* rpg_bat_pos1_set - RPG_BAT overlay, Address: 0x8013EEAC, Size: 292 bytes (73 insns) */
#include "types.h"

#define rpg_bat_target_x (*(volatile s32 *)0x8015EC28)
#define rpg_bat_target_y (*(volatile s32 *)0x8015EC2C)

void rpg_bat_pos1_set(s32 a0)
{
    switch ((u32)a0) {
    case 0:
        rpg_bat_target_x = 120;
        rpg_bat_target_y = 104;
        break;
    case 1:
        rpg_bat_target_x = 64;
        rpg_bat_target_y = 88;
        break;
    case 2:
        rpg_bat_target_x = 56;
        rpg_bat_target_y = 136;
        break;
    case 3:
        rpg_bat_target_x = 72;
        rpg_bat_target_y = 96;
        break;
    case 4:
        rpg_bat_target_x = 72;
        rpg_bat_target_y = 96;
        break;
    case 5:
        rpg_bat_target_x = 72;
        rpg_bat_target_y = 96;
        break;
    case 16:
        rpg_bat_target_x = 72;
        rpg_bat_target_y = 96;
        break;
    case 17:
        rpg_bat_target_x = 72;
        rpg_bat_target_y = 96;
        break;
    case 18:
        rpg_bat_target_x = 72;
        rpg_bat_target_y = 96;
        break;
    }
}
