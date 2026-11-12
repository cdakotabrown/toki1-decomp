/* @CompileEnv: IDO52_COPYPROP_VAR=-24 IDO52_FORCE_WEB_COLOR=50:s2,32:s1 IDO52_AS1_TIE_TAKE=168,174 IDO52_AS1_TIE_REJECT=172 */
/* @CompileFlags: -cckr */
/* @SymbolSize: 812 */
/* rpg_bat_sprite_spread - RPG_BAT overlay, Address: 0x80146578, Size: 812 bytes */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"

typedef struct {
    u8 pad0[7];
    u8 f7;
    u8 pad8[4];
    s32 f12;
    s32 f16;
    u8 pad20[32];
    s32 f52;
    s32 f56;
    u8 pad60[7];
    u8 f67;
} Rec;

extern Rec class_w[];
extern s32 rpg_bat_res_hdr[];
extern s32 rpg_bat_res_img[];
extern s32 rpg_bat_res_anim[];
extern RPG_BAT_UNIT rpg_bat_unit1;
extern RPG_BAT_UNIT rpg_bat_unit3;

rpg_bat_sprite_spread()
{
    int i;
    int n;
    int hv;
    int pad0;
    int pad1;
    int z;
    int a;
    int b;
    int d;

    rpg_bat_se_set(1288, 0, 1);
    switch (rpg_bat_unit1.sub) {
    case 0:
        for (i = 0; i < 12; i++) {
            class_w[i + 148].f12 = rpg_bat_res_img[1];
            class_w[i + 148].f16 = rpg_bat_res_anim[1];
            class_w[i + 148].f52 = rpg_bat_res_hdr[1];
            class_w[i + 148].f56 = 0x41000000;
            class_w[i + 148].f7 = 128;
            class_w[i + 148].f67 = 19;
            rpg_bat_sprite_pos_set(i + 52, 0, 0);
            rpg_bat_anim_set(i + 52, i + 5, 1, 1);
        }
        rpg_bat_substep_next();
        break;
    case 1:
        n = ++rpg_bat_unit1.cnt;
        hv = rsin((n << 11) / 100) >> 8;
        a = (n * -48) / 100;
        rpg_bat_sprite_pos_set(52, a, a - hv);
        d = a - hv;
        z = 0;
        rpg_bat_sprite_pos_set(53, z, d);
        b = (n * 48) / 100;
        rpg_bat_sprite_pos_set(54, b, d);
        d = (n * -16) / 100 - hv;
        rpg_bat_sprite_pos_set(55, a, d);
        rpg_bat_sprite_pos_set(56, z, d);
        rpg_bat_sprite_pos_set(57, b, d);
        d = (n * 16) / 100 - hv;
        rpg_bat_sprite_pos_set(58, a, d);
        rpg_bat_sprite_pos_set(59, z, d);
        rpg_bat_sprite_pos_set(60, b, d);
        rpg_bat_sprite_pos_set(61, (n * 96) / 100, d);
        d = b - hv;
        rpg_bat_sprite_pos_set(62, z, d);
        rpg_bat_sprite_pos_set(63, b, d);
        if (rpg_bat_unit1.cnt >= 81) {
            rpg_bat_unit1.cnt = rpg_bat_unit1.cnt;
            if ((rpg_bat_unit1.cnt & 1) == 0) {
                for (i = 0; i < 12; i++) {
                    rpg_bat_anim_set(i + 52, i + 5, 1, 1);
                }
            } else {
                for (i = 0; i < 12; i++) {
                    rpg_bat_anim_set(i + 52, i + 5, 1, 0);
                }
            }
        }
        if (rpg_bat_unit1.cnt >= 101) {
            rpg_bat_unit3.hp ^= 0x10000000;
            rpg_bat_seq_step_next();
        }
        break;
    }
}
