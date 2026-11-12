/* @PostProc: scheduler_normalizer --block-aware */
/* @RodataBase: 0x8015DC18 */
/* @PostProc: patch_insn_reg_field 0x13c:rs:v0:t5 0x13c:rt:t5:v0 0x158:rs:v0:t7 0x158:rt:t7:v0 0x17c:rs:v0:t9 0x17c:rt:t9:v0 0x194:rt:v1:v0 0x198:rs:v1:v0 0x1b8:rt:v1:v0 0x200:rt:v1:v0 */
/* @SymbolSize: 620 */
/* Address: 0x801564F0 */
/* Size: 620 bytes (155 insns)   BYTE-PERFECT (allowed PostProc only) */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"

extern void strcpy(u8 *dst, u8 *src);
extern void rpg_bat_anim_set(s32 a0, s32 a1, s32 a2, s32 a3);
extern s32 get_rand_mod(s32 a0);
extern void rpg_bat_koe_set(s32 a0, s32 a1, s32 a2);
extern void rpg_bat_talk_clear(void);
extern void rpg_bat_talk_wait(s32 a0);
extern void rpg_bat_msg_set(u8 *a0, s32 a1, s32 a2);
extern void rpg_bat_act_next(void);

extern s32 rpg_bat_talk_work_step;
extern u8 bat_blink_step_str0[];
extern u8 bat_blink_step_str1[];
extern u8 rpg_bat_msg_buf3[];
extern u8 rpg_bat_msg_tbl3[];
extern u8 cw155_disp;
extern u8 cw156_disp;
extern u8 cw157_disp;
extern RPG_BAT_UNIT rpg_bat_unit1;
extern RPG_BAT_UNIT rpg_bat_unit2;

void bat_blink_step(void)
{
    s32 tick;

    rpg_bat_anim_set(29, 8, 1, 1);

    switch ((u32)rpg_bat_talk_work_step) {
    case 0:
        if ((rpg_bat_unit1.flag << 11) < 0) {
            strcpy(rpg_bat_msg_buf3, bat_blink_step_str0);
        } else {
            strcpy(rpg_bat_msg_buf3, bat_blink_step_str1);
        }
        rpg_bat_msg_set(rpg_bat_msg_tbl3, 1, 130);
        rpg_bat_talk_work_step = rpg_bat_talk_work_step + 1;
        break;

    case 1:
        if ((rpg_bat_unit1.flag << 11) < 0) {
            rpg_bat_koe_set(0x1E00006F, 0x17000072, 0);
        } else {
            rpg_bat_koe_set(0x10000093, 0x06000098, 0);
        }
        rpg_bat_talk_work_step = rpg_bat_talk_work_step + 1;
        break;

    case 2:
        cw155_disp = get_rand_mod(2) | cw155_disp;
        cw156_disp = get_rand_mod(2) | cw156_disp;
        cw157_disp = get_rand_mod(2) | cw157_disp;
        rpg_bat_talk_work_step = rpg_bat_talk_work_step + 1;
        break;

    case 3:
        tick = rpg_bat_unit2.cnt;
        if (tick % 15 == 14) {
            cw155_disp = cw155_disp ^ 1;
            cw156_disp = cw156_disp ^ 1;
            cw157_disp = cw157_disp ^ 1;
        }
        rpg_bat_unit2.cnt = tick + 1;
        rpg_bat_talk_wait(0);
        return;

    case 4:
        cw155_disp = cw155_disp & 0xFE;
        cw156_disp = cw156_disp & 0xFE;
        cw157_disp = cw157_disp & 0xFE;
        rpg_bat_anim_set(29, 0, 1, 1);
        rpg_bat_talk_clear();
        rpg_bat_act_next();
        break;
    }
}
