/* @CompileFlags: -cckr */
#include "types.h"

struct Spr {
    unsigned char pad0[38];
    short x;
    short pad40;
    short y;
    unsigned char pad44[24];
};

extern int rpg_bat_slot0_ctl;
extern int rpg_bat_slot0_ctl_mode;
extern int rpg_bat_slot0_ctl_cnt;
extern int rpg_bat_slot1_ctl;
extern int rpg_bat_slot1_ctl_mode;
extern int rpg_bat_slot1_ctl_cnt;
extern int rpg_bat_slot2_ctl;
extern int rpg_bat_slot2_ctl_mode;
extern int rpg_bat_slot2_ctl_cnt;
extern int rpg_bat_slot3_ctl;
extern int rpg_bat_slot3_ctl_mode;
extern int rpg_bat_slot3_ctl_cnt;
extern struct Spr class_w[];

void rpg_bat_slot_set(sel, x, y)
int sel;
int x;
int y;
{
    int v;
    int unused;
    int h;
    int i;

    x -= 158;
    y -= 120;
    switch (sel) {
    case 0:
        rpg_bat_slot0_ctl = 0;
        rpg_bat_slot0_ctl_mode = 1;
        rpg_bat_slot0_ctl_cnt = 0;
        break;
    case 1:
        rpg_bat_slot1_ctl = 0;
        rpg_bat_slot1_ctl_mode = 1;
        rpg_bat_slot1_ctl_cnt = 0;
        break;
    case 2:
        rpg_bat_slot2_ctl = 0;
        rpg_bat_slot2_ctl_mode = 1;
        rpg_bat_slot2_ctl_cnt = 0;
        break;
    case 3:
        rpg_bat_slot3_ctl = 0;
        rpg_bat_slot3_ctl_mode = 1;
        rpg_bat_slot3_ctl_cnt = 0;
        break;
    }
    v = sel * 12;
    h = v + 64;
    class_w[v + 64].x = x;
    class_w[v + 64].y = y;
    class_w[v + 68].x = x;
    class_w[v + 68].y = y;
    class_w[v + 72].x = x;
    class_w[v + 72].y = y;
    rpg_bat_cw_anim_set(h, 52, 0);
    rpg_bat_cw_anim_set(h + 4, 53, 0);
    rpg_bat_cw_anim_set(h + 8, 53, 0);
    for (i = 1; i < 4; i++) {
        class_w[v + i + 64].x = 160;
        class_w[v + i + 64].y = 120;
        class_w[v + i + 68].x = 160;
        class_w[v + i + 68].y = 120;
        class_w[v + i + 72].x = 160;
        class_w[v + i + 72].y = 120;
    }
}
