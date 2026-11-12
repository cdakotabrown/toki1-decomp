/* @SymbolStart: 0x80136A9C */
/* @SymbolSize: 316 */
/* @PostProc: swap_instructions --offset1 0x2c --offset2 0x100 */
/* @PostProc: swap_instructions --offset1 0x100 --offset2 0x104 */
/* @PostProc: swap_instructions --offset1 0x108 --offset2 0x10c */
/* @PostProc: swap_instructions --offset1 0x10c --offset2 0x110 */
/* @PostProc: swap_instructions --offset1 0x128 --offset2 0x130 */
/* @PostProc: swap_instructions --offset1 0x12c --offset2 0x138 */
/* @PostProc: patch_insn_reg_field 0x2c:rt:v0:v1 0x40:rt:v0:v1 0x50:rt:a1:v0 0x54:rs:a1:v0 0x54:rt:a1:v0 0x5c:rs:a1:v0 0x5c:rt:a1:v0 0x64:rt:a1:v0 0x68:rt:a1:v0 0x6c:rs:a1:v0 0x6c:rt:a1:v0 0x74:rs:a1:v0 0x74:rt:a1:v0 0x78:rt:a1:v0 0x7c:rs:v0:v1 0x7c:rt:v0:v1 0x84:rs:v0:v1 0x90:rs:v0:v1 0x100:rs:v0:v1 0x100:rt:v0:v1 0x104:rt:a1:v0 0x108:rs:v0:v1 0x108:rt:v0:v1 0x10c:rt:v0:v1 0x110:rs:a1:v0 0x110:rt:a1:v0 0x120:rs:a1:v0 0x12c:rt:a1:v0 0x138:rt:a1:v0 */
/* Address: 0x80136AA4 (true start 0x80136A9C: the rpg_bat_unit0_sub selector */
/* Size: 308 bytes (77 insns) + 8 hoisted = 316 */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"

extern void rpg_bat_sprite_pos_set(s32, s32, s32);
extern void rpg_bat_anim_set2(s32, s32, s32, s32);
extern void rpg_bat_se_set2(s32, s32, s32);
extern void rpg_bat_state_next(void);
extern s32 rpg_bat_step_next(void);

extern s32 rpg_bat_target_y;
extern s32 rpg_bat_tgt_y;
extern s32 rpg_bat_target_no;
extern s32 rpg_bat_target_x;
extern u8 cw157_prio;
extern u8 cw155_prio;
extern s16 cw139_y;
extern RPG_BAT_UNIT rpg_bat_unit0;
extern RPG_BAT_UNIT rpg_bat_unit1;

void rpgbat_slide_step(void)
{
    s32 t;
    s32 h;

    switch (rpg_bat_unit0.sub) {
    case 0:
        if ((rpg_bat_unit1.flag << 11) < 0) {
            rpg_bat_tgt_y = rpg_bat_target_y - 160;
        } else {
            rpg_bat_tgt_y = rpg_bat_target_y - 120;
        }
        switch (rpg_bat_target_no) {
        case 0:
            cw157_prio = 4;
            break;
        case 1:
            cw155_prio = 4;
            cw157_prio = 4;
            break;
        }
        rpg_bat_sprite_pos_set(43, rpg_bat_target_x, -196);
        rpg_bat_anim_set2(43, 4, 1, 1);
        rpg_bat_se_set2(1283, 0, 3);
        rpg_bat_state_next();
        break;
    case 1:
        t = rpg_bat_tgt_y;
        h = (s16)(cw139_y + 4);
        cw139_y = h;
        if (t < h) {
            cw139_y = (s16)t;
            rpg_bat_tgt_y = t;
            rpg_bat_step_next();
        }
        break;
    }
}
