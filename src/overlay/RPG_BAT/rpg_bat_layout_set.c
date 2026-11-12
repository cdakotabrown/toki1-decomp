/* @SymbolSize: 552 */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:6 */
/* @PostProc: swap_branch_operands --offsets 0x140,0x168,0x188 */
/* rpg_bat_layout_set - RPG_BAT overlay, Address: 0x801513C8, Size: 552 bytes (138 insns) */
/* `@CompileEnv: IDO52_PRECOLOR_WEB=v0:6` coalesces them into the target's */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
#include "tokimeki/class_w.h"

extern s32 palette_upload_enqueue();
extern s32 rpg_bat_anim_set();
extern s32 rpg_bat_anim_set2();
extern s32 rpg_bat_sprite_pos_set();

extern s32 rpg_bat_res_pal_4;
extern s32 rpg_bat_toriko_flag;
extern s32 rpg_bat_toriko_flag_1;
extern s32 rpg_bat_toriko_flag_2;

extern CLASS_W class_w[];
extern RPG_BAT_UNIT rpg_bat_unit1;

s32 rpg_bat_layout_set(void)
{
    palette_upload_enqueue(rpg_bat_res_pal_4, 27, 1, 3, 7);

    switch (rpg_bat_unit1.flag & 0xFFFF0000) {
    case 0x00100000:
        rpg_bat_sprite_pos_set(53, 120, 104);
        rpg_bat_sprite_pos_set(54, 64, 88);
        rpg_bat_sprite_pos_set(55, 56, 136);
        rpg_bat_anim_set2(53, 9, 1, 1);
        rpg_bat_anim_set2(54, 9, 1, 1);
        rpg_bat_anim_set2(55, 9, 1, 1);
        (*(s32 *)&class_w[149].attribute) = 0x51000000;
        (*(s32 *)&class_w[150].attribute) = 0x51000000;
        (*(s32 *)&class_w[151].attribute) = 0x51000000;
        class_w[149].bright = 0;
        class_w[150].bright = 0;
        class_w[151].bright = 0;
        class_w[149].clut = 27;
        class_w[150].clut = 27;
        class_w[151].clut = 27;
        if (rpg_bat_toriko_flag == 1) {
            class_w[149].disp_flag |= 1;
        }
        if (rpg_bat_toriko_flag_1 == 1) {
            class_w[150].disp_flag |= 1;
        }
        if (rpg_bat_toriko_flag_2 == 1) {
            class_w[151].disp_flag |= 1;
        }
        break;
    case 0x00200000:
        rpg_bat_sprite_pos_set(53, 0, 0);
        rpg_bat_anim_set(53, 10, 1, 1);
        (*(s32 *)&class_w[149].attribute) = 0x51000000;
        class_w[149].bright = 0;
        class_w[149].clut = 27;
        break;
    case 0x00400000:
        rpg_bat_sprite_pos_set(53, 0, 0);
        rpg_bat_anim_set(53, 11, 1, 1);
        (*(s32 *)&class_w[149].attribute) = 0x51000000;
        class_w[149].bright = 0;
        class_w[149].clut = 27;
        break;
    default:
        return rpg_bat_unit1.flag & 0xFFFF0000;
    }
}
