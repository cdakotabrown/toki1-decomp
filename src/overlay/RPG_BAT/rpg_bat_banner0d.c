/* rpg_bat_banner0d - RPG_BAT overlay, Address: 0x8013A98C, Size: 792 bytes (198 insns) */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
#include "tokimeki/class_w.h"

extern s32 rpg_bat_anim_set();
extern s32 rpg_bat_se_set2();
extern s32 palette_upload_enqueue();
extern s32 rpg_bat_step_next();

extern CLASS_W class_w[];
extern s32 rpg_bat_res_pal[];
extern RPG_BAT_UNIT rpg_bat_unit0;

void rpg_bat_banner0d(void)
{
    rpg_bat_anim_set(46, 8, 1, 1);
    class_w[142].clut = 27;
    class_w[157].prio = 4;
    if (rpg_bat_unit0.cnt < 60) {
        s32 v0 = rpg_bat_unit0.cnt;
        rpg_bat_unit0.cnt = v0;
        palette_upload_enqueue(rpg_bat_res_pal[5], 27, 1, 4, v0 / 10);
    }
    if (rpg_bat_unit0.cnt >= 60) {
        s32 v0;
        rpg_bat_se_set2(1286, 0, 1);
        v0 = rpg_bat_unit0.cnt;
        palette_upload_enqueue(rpg_bat_res_pal[5], 27, 1, 4, (v0 - 60) % 8 / 2 + 6);
    }
    if (rpg_bat_unit0.cnt >= 68) {
        s32 v0 = rpg_bat_unit0.cnt;
        rpg_bat_unit0.cnt = v0;
        palette_upload_enqueue(rpg_bat_res_pal[5], 27, 1, 4, (v0 - 68) % 8 / 2 + 6);
    }
    if (rpg_bat_unit0.cnt >= 76) {
        s32 v0 = rpg_bat_unit0.cnt;
        rpg_bat_unit0.cnt = v0;
        palette_upload_enqueue(rpg_bat_res_pal[5], 27, 1, 4, (v0 - 76) % 8 / 2 + 6);
    }
    if (rpg_bat_unit0.cnt >= 84) {
        s32 v0 = rpg_bat_unit0.cnt;
        rpg_bat_unit0.cnt = v0;
        palette_upload_enqueue(rpg_bat_res_pal[5], 27, 1, 4, (v0 - 84) % 8 / 2 + 6);
    }
    if (rpg_bat_unit0.cnt >= 92) {
        s32 v0 = rpg_bat_unit0.cnt;
        rpg_bat_unit0.cnt = v0;
        palette_upload_enqueue(rpg_bat_res_pal[5], 27, 1, 4, (v0 - 92) % 12 / 2 + 10);
    }
    rpg_bat_unit0.cnt = rpg_bat_unit0.cnt + 1;
    if (rpg_bat_unit0.cnt < 105) {
        return;
    }
    class_w[142].clut = 29;
    rpg_bat_anim_set(46, 9, 1, 1);
    rpg_bat_anim_set(47, 255, 1, 0);
    rpg_bat_anim_set(44, 255, 1, 0);
    rpg_bat_anim_set(45, 255, 1, 0);
    (*(s32 *)&class_w[140].attribute) = 0x41000000;
    (*(s32 *)&class_w[141].attribute) = 0x41000000;
    rpg_bat_step_next();
}
