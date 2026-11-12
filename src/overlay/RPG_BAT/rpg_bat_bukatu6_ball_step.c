/* @CompileEnv: IDO52_FORCE_WEB_NOSPILL=20 IDO52_FORCE_WEB_COLOR_NOSPLIT=20 IDO52_FORCE_WEB_COLOR=28:a0 */
/* rpg_bat_bukatu6_ball_step - RPG_BAT overlay, Address: 0x8015A744, Size: 368 bytes (92 insns) */
#include "types.h"

extern s32 rpg_bat_anim_set2();
extern s32 rpg_bat_sprite_pos_set();
extern s32 get_rand_mod();
extern s32 rpg_bat_se_set2();

extern s32 rpg_bat_move_dx;
extern s32 rpg_bat_move_dy;
extern s32 rpg_bat_tgt_x;

typedef struct {
    u8  pad0[3];
    u8  f3;
    u8  pad1[22];
    s16 f26;
    s16 f28;
    u8  pad2[8];
    s16 f38;
    u8  pad3[2];
    s16 f42;
    u8  pad4[2];
    s16 f46;
    u8  pad5[2];
    s16 f50;
    u8  pad6[16];
} BatRec;

extern BatRec class_w[];

void rpg_bat_bukatu6_ball_step(void)
{
    s32 i;
    s32 x;
    s32 result;

    for (i = 0; i < 8; i++) {
        if (class_w[i + 136].f38 < -180 || class_w[i + 136].f42 >= 141) {
            if (get_rand_mod(5) == 0) {
                rpg_bat_se_set2(1281, 2, 2);
                rpg_bat_sprite_pos_set(i + 40, 240, 40);
                rpg_bat_move_dx = get_rand_mod(8) + 5;
                rpg_bat_move_dy = get_rand_mod(rpg_bat_move_dx);
                class_w[i + 136].f3 = 148;
                rpg_bat_move_dx = -rpg_bat_move_dx;
                class_w[i + 136].f46 = rpg_bat_move_dx;
                class_w[i + 136].f50 = rpg_bat_move_dy;
            } else {
                rpg_bat_anim_set2(i + 40, 7, 1, 0);
            }
        }
        x = class_w[i + 136].f38;
        result = ((112 - x) << 12) / 240;
        class_w[i + 136].f26 = result;
        class_w[i + 136].f28 = result;
        rpg_bat_tgt_x = x;
    }
}
