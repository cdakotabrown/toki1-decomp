/* @RodataBase: 0x8015ADE4 */
/* @CompileEnv: IDO52_CONST_INREG_VALUES=768 IDO52_COPYPROP_SITE=lod:4,lod:5 IDO52_AS1_XBB_SKIP_CAND=0 */
/* rpg_bat_layout_init - RPG_BAT overlay, Address: 0x80134EBC, Size: 1180 bytes */
/* @SymbolStart: 0x80134E78 */
/* @SymbolSize: 1248 */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"

typedef struct {
    u8  pad0[1];
    u8  kind;
    u8  pad1[6];
    s16 f8;
    u8  pad2[14];
    s16 f24;
    u8  pad3[42];
} BatEntry;

extern void rpg_bat_anim_set2(s32, s32, s32, s32);
extern void rpg_bat_sprite_pos_set(s32, s32, s32);
extern s32  get_rand_mod(s32);
extern void rpg_bat_step_next(void);

extern BatEntry class_w[];
extern s32 rpg_bat_target_no[];
extern RPG_BAT_UNIT rpg_bat_unit1;


void rpg_bat_layout_init(void)
{
    BatEntry *q;
    s32 *p;
    s32 i;
    s32 id;
    s32 base;
    s32 span;
    s32 v;
    s32 w;

    rpg_bat_target_no[51] = -448;
    rpg_bat_target_no[52] = 640;
    rpg_bat_target_no[53] = -320;
    class_w[146].kind = 2;
    class_w[147].kind = 2;
    class_w[150].kind = 8;
    class_w[151].kind = 8;

    w = 768;
    rpg_bat_target_no[50] = w;
    rpg_bat_sprite_pos_set(50, w, 136);
    rpg_bat_sprite_pos_set(51, rpg_bat_target_no[50], 136);
    rpg_bat_sprite_pos_set(54, rpg_bat_target_no[52], 80);
    rpg_bat_sprite_pos_set(55, rpg_bat_target_no[52], 80);
    rpg_bat_anim_set2(50, 6, 1, 1);
    rpg_bat_anim_set2(51, 5, 1, 1);
    rpg_bat_anim_set2(54, 6, 1, 1);
    rpg_bat_anim_set2(55, 5, 1, 1);

    base = get_rand_mod(32);
    rpg_bat_target_no[60] = 752;
    rpg_bat_target_no[70] = -400;
    rpg_bat_target_no[80] = 125;
    v = base + 16;
    rpg_bat_target_no[61] = v + 784;
    rpg_bat_target_no[71] = v - 368;
    rpg_bat_target_no[81] = 122;

    span = get_rand_mod(16) + 32;
    base = get_rand_mod(16);
    rpg_bat_target_no[62] = 688;
    rpg_bat_target_no[72] = -400;
    rpg_bat_target_no[82] = 108;
    rpg_bat_target_no[63] = span + 704;
    rpg_bat_target_no[73] = span - 384;
    rpg_bat_target_no[83] = 105;
    v = base + span;
    rpg_bat_target_no[64] = v + 752;
    rpg_bat_target_no[74] = v - 336;
    rpg_bat_target_no[84] = 111;

    span = get_rand_mod(16) + 16;
    base = get_rand_mod(16);
    rpg_bat_target_no[65] = span + 672;
    rpg_bat_target_no[75] = span - 352;
    rpg_bat_target_no[85] = 97;
    v = base + span;
    rpg_bat_target_no[66] = v + 720;
    rpg_bat_target_no[76] = v - 304;
    rpg_bat_target_no[86] = 94;

    switch (rpg_bat_target_no[0]) {
    case 0:
        rpg_bat_target_no[67] = 760;
        rpg_bat_target_no[77] = -328;
        rpg_bat_target_no[87] = 100;
        rpg_bat_target_no[47] = 119;
        rpg_bat_target_no[48] = 62;
        break;
    case 1:
        rpg_bat_target_no[67] = 720;
        rpg_bat_target_no[77] = -304;
        rpg_bat_target_no[87] = 91;
        rpg_bat_target_no[47] = 63;
        rpg_bat_target_no[48] = 46;
        break;
    case 2:
        rpg_bat_target_no[67] = 784;
        rpg_bat_target_no[77] = -368;
        rpg_bat_target_no[87] = 133;
        rpg_bat_target_no[47] = 55;
        rpg_bat_target_no[48] = 94;
        break;
    case 3:
        rpg_bat_target_no[67] = 784;
        rpg_bat_target_no[77] = -368;
        rpg_bat_target_no[87] = 133;
        rpg_bat_target_no[47] = 76;
        rpg_bat_target_no[48] = 14;
        break;
    case 4:
        rpg_bat_target_no[67] = 784;
        rpg_bat_target_no[77] = -368;
        rpg_bat_target_no[87] = 133;
        rpg_bat_target_no[47] = 84;
        rpg_bat_target_no[48] = 42;
        break;
    }

    for (p = rpg_bat_target_no, q = class_w, i = 0; i < 8; i++) {
        id = i + 40;
        rpg_bat_anim_set2(id, 5, 1, 1);
        rpg_bat_sprite_pos_set(id, p[60], p[80]);
        q[136].f24 = (s16)get_rand_mod(4);
        w = get_rand_mod(7);
        v = p[80];
        q[136].f8 = w;
        if (v >= 133) {
            q[136].kind = 3;
        } else if (v >= 102) {
            q[136].kind = 4;
        } else if (v >= 100) {
            q[136].kind = 5;
        } else if (v >= 93) {
            q[136].kind = 6;
        } else {
            q[136].kind = 7;
        }
        p++;
        q++;
    }

    if ((rpg_bat_unit1.flag << 11) < 0) {
        class_w[155].kind = 5;
        class_w[156].kind = 7;
        class_w[157].kind = 3;
    } else {
        class_w[155].kind = 3;
    }
    rpg_bat_step_next();
}
