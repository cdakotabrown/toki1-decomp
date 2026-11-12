/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=75:s2 */
/* @SymbolSize: 1012 */
/* rpg_bat_gauge_step - RPG_BAT overlay, Address: 0x80148714, Size: 1012 bytes */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
#include "tokimeki/class_w.h"

typedef struct {
    s32 w00;
    s32 pad04[30];
    s32 x;
    s32 y;
    s32 pad84[8];
    s32 sx;
    s32 sy;
    s32 padAC[4];
    s32 px;
    s32 py;
    s32 padC4;
    s32 tbl[12];
} Bat;
extern Bat rpg_bat_target_no;
extern s32 rpg_bat_pair_sel[];
extern s32 rpg_bat_seq_work[];
extern CLASS_W class_w[];
extern RPG_BAT_UNIT rpg_bat_unit0;

rpg_bat_gauge_step()
{
    s32 i;
    s32 x;
    s32 y;
    s32 v;

    switch (rpg_bat_unit0.sub) {
    case 0:
        rpg_bat_target_no.sx = rpg_bat_target_no.x - 176;
        rpg_bat_target_no.sy = rpg_bat_target_no.y - 88;
        if (rpg_bat_target_no.w00 == 1) {
            class_w[155].prio = 4;
            class_w[157].prio = 4;
        }
        if (rpg_bat_target_no.w00 == 0) {
            class_w[157].prio = 4;
        }
        rpg_bat_se_set2(1287, 1, 0);
        rpg_bat_state_next();
        break;
    case 1:
        if (rpg_bat_unit0.cnt >= 1000) {
            v = 1;
        } else {
            v = 0;
        }
        i = v;
        if (v < 6) do {
            rpg_bat_target_no.px = rpg_bat_target_no.sx * rpg_bat_target_no.tbl[i * 2] / 1000;
            rpg_bat_target_no.py = rpg_bat_target_no.sy * rpg_bat_target_no.tbl[i * 2] / 1000;
            rpg_bat_sprite_pos_set(i + 50, rpg_bat_target_no.px, rpg_bat_target_no.py);
            rpg_bat_anim_set(i + 50, 6, 1, 1);
        } while (++i != 6);
        if (rpg_bat_unit0.cnt < 1000) {
            if (rpg_bat_target_no.w00 == 0) {
                rpg_bat_unit0.cnt += 60;
            } else {
                rpg_bat_unit0.cnt += 40;
            }
            rpg_bat_seq_work[0] = rpg_bat_unit0.cnt;
        }
        x = 0;
        for (i = 49; i > x; i--) {
            rpg_bat_seq_work[i] = rpg_bat_pair_sel[i];
        }
        if (rpg_bat_seq_work[20] == rpg_bat_seq_work[0]) {
            for (i = 1; i < 6; i++) {
                rpg_bat_anim_set(i + 50, 6, 1, 0);
            }
        }
        if (rpg_bat_unit0.cnt >= 1000 && rpg_bat_unit0.cnt < 2000) {
            rpg_bat_sprite_pos_set(50, rpg_bat_target_no.x, rpg_bat_target_no.y);
            rpg_bat_anim_set2(50, 7, 5, 1);
            rpg_bat_unit0.cnt = 2001;
        } else if ((class_w[146].anim_flag & 1) == 0) {
            rpg_bat_unit0.wait++;
            rpg_bat_unit0.cnt = 1000;
        }
        if (rpg_bat_unit0.cnt >= 1000) {
            rpg_bat_se_set2(1288, 10, 1);
            for (i = 0; i < 4; i++) {
                if (get_rand_mod(20) == 0) {
                    x = get_rand_mod(32);
                    y = get_rand_mod(32);
                    rpg_bat_sprite_pos_set(i + 40, x + rpg_bat_target_no.x - 16, y + rpg_bat_target_no.y - 16);
                    rpg_bat_anim_set2(i + 40, 8, 1, 1);
                }
            }
        }
        if (rpg_bat_unit0.wait >= 8) {
            for (i = 0; i < 4; i++) {
                rpg_bat_anim_set2(i + 40, 255, 1, 0);
            }
            rpg_bat_step_next();
        }
        break;
    }
}
