/* @SymbolSize: 692 */
/* @PostProc: swap_instructions --offset1 0x30 --offset2 0x38 */
/* @PostProc: swap_instructions --offset1 0x34 --offset2 0x3C */
/* @PostProc: swap_instructions --offset1 0x38 --offset2 0x40 */
/* @PostProc: swap_instructions --offset1 0x3C --offset2 0x40 */
/* @PostProc: swap_instructions --offset1 0xD4 --offset2 0xD8 */
/* @PostProc: swap_instructions --offset1 0x1D8 --offset2 0x1DC */
/* rpg_bat_cam_orbit - RPG_BAT overlay, Address: 0x8014E300, Size: 692 bytes */
/* Windowed here with @SymbolSize: 692; splitting the blob would individuate */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"

extern void rpg_bat_cam_set(s32);
extern void rpg_bat_se_set(s32, s32, s32);
extern s32  rsin(s32);
extern void rpg_bat_sprite_pos_set(s32, s32, s32);
extern void rpg_bat_act_next(void);

extern s32 rpg_bat_target_no;
extern s32 rpg_bat_target_x;
extern s32 rpg_bat_target_y;
extern s32 rpg_bat_move_dx;
extern s32 rpg_bat_move_dy;
extern RPG_BAT_UNIT rpg_bat_unit2;

void rpg_bat_cam_orbit(void)
{
    s32 s;

    rpg_bat_cam_set(rpg_bat_target_no);
    rpg_bat_move_dx = rpg_bat_target_x - 224;
    rpg_bat_move_dy = rpg_bat_target_y - 142;
    rpg_bat_se_set(1295, 0, 1);
    if (rpg_bat_unit2.cnt >= 320) {
        rpg_bat_se_set(1300, 16, 3);
    }
    if (rpg_bat_target_no == 0) {
        rpg_bat_unit2.cnt = rpg_bat_unit2.cnt + 7;
        s = rsin(rpg_bat_unit2.cnt * 8192 / 1000);
        rpg_bat_sprite_pos_set(30, rpg_bat_move_dx * rpg_bat_unit2.cnt / 1000 + 224,
                      (s >> 8) + rpg_bat_move_dy * rpg_bat_unit2.cnt / 1000 + 128);
    } else {
        rpg_bat_unit2.cnt = rpg_bat_unit2.cnt + 5;
        s = rsin(rpg_bat_unit2.cnt * 12288 / 1000);
        rpg_bat_sprite_pos_set(30, rpg_bat_move_dx * rpg_bat_unit2.cnt / 1000 + 224,
                      (s >> 8) + rpg_bat_move_dy * rpg_bat_unit2.cnt / 1000 + 128);
    }
    if (rpg_bat_unit2.cnt >= 1001) {
        rpg_bat_sprite_pos_set(30, rpg_bat_target_x, rpg_bat_target_y - 14);
        rpg_bat_act_next();
    }
}
