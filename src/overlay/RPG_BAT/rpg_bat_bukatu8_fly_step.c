/* @CompileEnv: IDO52_FORCE_WEB_COLOR=14:v1 */
/* rpg_bat_bukatu8_fly_step - RPG_BAT overlay, Address: 0x80155E80, Size: 484 bytes (121 insns) */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
#include "tokimeki/class_w.h"

extern s32 rpg_bat_target_no;
typedef struct {
    s32 x;
    s32 y;
    s32 pad[8];
    s32 dx;
    s32 dy;
} CamPos;
extern CamPos rpg_bat_target_x;
extern CLASS_W class_w[];
extern RPG_BAT_UNIT rpg_bat_unit0;
extern RPG_BAT_UNIT rpg_bat_unit1;

extern s32 rpg_bat_se_set2();
extern s32 rpg_bat_sprite_pos_set();
extern s32 rpg_bat_anim_set2();
extern s32 rpg_bat_state_next();
extern s32 rpg_bat_step_next();

void rpg_bat_bukatu8_fly_step(void)
{
    s32 sel;
    s32 sub;
    s32 a1v;
    s32 a2v;

    rpg_bat_se_set2(1294, 0, 3);
    sel = rpg_bat_unit0.sub;
    switch (sel) {
    case 0:
        sub = rpg_bat_target_no;
        switch (sub) {
        case 0:
            class_w[157].prio = 4;
            break;
        case 1:
            class_w[155].prio = 4;
            class_w[157].prio = 4;
            break;
        }
        if ((rpg_bat_unit1.flag << 11) < 0) {
            rpg_bat_target_x.dx = rpg_bat_target_x.x - 236;
            rpg_bat_target_x.dy = rpg_bat_target_x.y - 23;
        } else {
            rpg_bat_target_x.dx = -164;
            rpg_bat_target_x.dy = 116;
        }
        rpg_bat_sprite_pos_set(51, 236, 44);
        rpg_bat_anim_set2(51, 5, 1, 1);
        rpg_bat_state_next();
        break;
    case 1:
        a1v = (s32)((u32)rpg_bat_target_x.dx * (u32)rpg_bat_unit0.cnt) / 1000 + 236;
        a2v = (s32)((u32)rpg_bat_target_x.dy * (u32)rpg_bat_unit0.cnt) / 1000 + 44;
        rpg_bat_sprite_pos_set(51, a1v, a2v);
        rpg_bat_unit0.cnt += 50;
        if (rpg_bat_unit0.cnt >= 1001) {
            rpg_bat_step_next();
        }
        break;
    }
}
