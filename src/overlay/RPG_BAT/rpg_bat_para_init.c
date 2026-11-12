/* @RodataBase: 0x8015C4A0 */
/* @SymbolSize: 1136 */
/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_FORCE_WEB_ADJSAVE=3:4.9 */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"

typedef union {
    u16 lo;
    struct {
        u32 unk0 : 25;
        u32 kind : 2;
        u32 unk27 : 5;
    } b;
} Ent;

typedef struct {
    u8 pad[1644];
    Ent ent[12];
} Save;

extern Save screen_w;
extern s16 para_tai;

extern s32 rpg_bat_mode;
extern s32 rpg_bat_girl;
extern s32 rpg_bat_bukatu_no;
extern s32 rpg_bat_bukatu_flag;
extern s32 rpg_bat_bukatu_ready;
extern s32 rpg_bat_enemy_hp[];
extern s32 rpg_bat_dmg_rate;
extern s32 rpg_bat_bukatu_gauge;
extern s32 rpg_bat_hit_cnt;
extern RPG_BAT_UNIT rpg_bat_unit0;
extern RPG_BAT_UNIT rpg_bat_unit1;
extern RPG_BAT_UNIT rpg_bat_unit2;
extern RPG_BAT_UNIT rpg_bat_unit3;

void rpg_bat_para_init()
{
    s32 i;
    s32 sum;
    s32 k;
    s32 n;

    sum = 0;
    k = 255;
    for (i = 0; i < 12; i++) {
        if (screen_w.ent[i].b.kind == 3) {
            k = i;
        }
        sum += screen_w.ent[i].lo;
    }
    if (k == 11) {
        if (sum == 0) {
            k = 11;
        } else {
            k = 255;
        }
    }
    rpg_bat_bukatu_no = k;
    if (k == 255) {
        rpg_bat_bukatu_flag = 0;
        rpg_bat_bukatu_ready = 0;
        rpg_bat_bukatu_gauge = 0;
    } else {
        rpg_bat_bukatu_flag = 1;
        rpg_bat_bukatu_ready = 1;
        rpg_bat_bukatu_gauge = 100;
    }
    rpg_bat_hit_cnt = 0;
    if (rpg_bat_mode & 0x800F0000) {
        rpg_bat_bukatu_flag = 0;
        rpg_bat_bukatu_ready = 0;
    }
    if (!(rpg_bat_mode & 0xF0000) && rpg_bat_girl == 13) {
        rpg_bat_girl = 9;
    }
    if (!(rpg_bat_mode & 0xF0000) && rpg_bat_girl == 14) {
        rpg_bat_girl = 9;
    }
    if ((rpg_bat_mode & 0xF0000) && rpg_bat_girl != 13) {
        rpg_bat_girl = 14;
    }
    if (rpg_bat_mode & 0x80000000) {
        rpg_bat_girl = 2;
    }
    if (rpg_bat_mode & 0x8000000) {
        rpg_bat_girl = 14;
    }
    if (rpg_bat_mode != 0x8000000) {
        rpg_bat_unit0.aux08 = 0xFFFF;
    } else {
        rpg_bat_unit0.aux08 = 0;
        rpg_bat_unit0.flag = 4096;
    }
    if (para_tai == 0) {
        n = 10;
    } else {
        n = para_tai * 10;
    }
    rpg_bat_unit0.hp_max = n;
    rpg_bat_unit0.hp = n;
    switch (rpg_bat_girl) {
    case 0:
        rpg_bat_unit2.hp = 256;
        break;
    case 1:
        rpg_bat_unit2.hp = 99;
        break;
    case 2:
        rpg_bat_unit2.hp = 312;
        break;
    case 3:
        rpg_bat_unit2.hp = 304;
        break;
    case 4:
        rpg_bat_unit2.hp = 189;
        break;
    case 5:
        rpg_bat_unit2.hp = 298;
        break;
    case 6:
        rpg_bat_unit2.hp = 1878;
        break;
    case 7:
        rpg_bat_unit2.hp = 277;
        break;
    case 8:
        rpg_bat_unit2.hp = 365;
        break;
    case 9:
        rpg_bat_unit2.hp = 173;
        break;
    case 10:
        rpg_bat_unit2.hp = 265;
        break;
    case 13:
        rpg_bat_unit2.hp = 573;
        break;
    case 14:
        rpg_bat_unit2.hp = 0x7FFFFFFF;
        break;
    }
    rpg_bat_unit2.hp_max = rpg_bat_unit2.hp;
    rpg_bat_unit1.flag = rpg_bat_mode;
    if (rpg_bat_mode == 0x8000000) {
        rpg_bat_unit1.flag = 0x8000002;
    }
    switch (rpg_bat_mode) {
    case 0x10000:
    case 0x20000:
    case 0x40000:
        rpg_bat_unit1.hp = 1;
        rpg_bat_unit1.hp_max = 1;
        break;
    case 0x8000000:
        rpg_bat_unit1.hp = 5000;
        rpg_bat_unit1.hp_max = 5000;
        break;
    case 0x80000000:
        rpg_bat_unit1.hp = 0x7FFFFFFF;
        rpg_bat_unit1.hp_max = 0x7FFFFFFF;
        rpg_bat_dmg_rate = 1000;
        break;
    case 0x100000:
        rpg_bat_unit1.hp = 0x10000;
        rpg_bat_unit1.hp_max = 0x10000;
        if (rpg_bat_flag_check()) {
            rpg_bat_dmg_rate = 1000;
            rpg_bat_enemy_hp[0] = 5000;
            rpg_bat_enemy_hp[1] = 5000;
            rpg_bat_enemy_hp[2] = 5000;
        } else {
            rpg_bat_dmg_rate = 1500;
            rpg_bat_enemy_hp[0] = 9999;
            rpg_bat_enemy_hp[1] = 9999;
            rpg_bat_enemy_hp[2] = 9999;
        }
        break;
    }
    switch (rpg_bat_mode) {
    case 0x10000:
        rpg_bat_unit3.hp = 0x10000075;
        break;
    case 0x20000:
        rpg_bat_unit3.hp = 0x10000232;
        break;
    case 0x40000:
        rpg_bat_unit3.hp = 0x1000012A;
        break;
    default:
        rpg_bat_unit3.hp = 0x7FFFFFFF;
        break;
    }
}
