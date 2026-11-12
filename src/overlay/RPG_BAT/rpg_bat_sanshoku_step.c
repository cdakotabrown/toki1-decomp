/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=138:t0,26:v1,83:v1,73:v0,79:v1 IDO52_COPYPROP_SITE=lod:16:deny,lod:17:deny IDO52_AS1_TIE_TAKE=710,718 IDO52_UGEN_DEST_TEMP=32 IDO52_UGEN_TEMP_REG=21:2 IDO52_UGEN_TEMP_REG_INPLACE=21 */
/* rpg_bat_sanshoku_step - RPG_BAT overlay, Address: 0x80154DD8, Size: 1204 bytes */
#include "types.h"
#include "tokimeki/class_w.h"

typedef struct {
    s32 pad0[5];
    s32 xEBAC;
    s32 pad1[43];
    s32 xEC5C;
    s32 pad2[1];
    s32 xEC64;
    s32 xEC68;
    s32 pad3[2];
    s32 xEC74;
    s32 xEC78;
    s32 xEC7C;
    s32 xEC80;
    s32 pad4[6];
    s32 xEC9C;
    s32 pad5[68];
    s32 xEDB0;
} Bat;

extern Bat rpg_bat_mode;
extern CLASS_W class_w[];

extern void rpg_bat_se_set2();
extern void rpg_bat_step_next();
extern void rpg_bat_state_next();
extern void sanshoku_bar_disp();

void rpg_bat_sanshoku_step()
{
    s32 sel;
    s32 mode;
    s32 n;
    s32 x;
    s32 y;

    rpg_bat_se_set2(1290, 0, 1);
    sel = rpg_bat_mode.xEDB0;
    if ((sel == 1 && rpg_bat_mode.xEC9C >= 61) || sel >= 2) {
        rpg_bat_se_set2(1292, 10, 7);
        sel = rpg_bat_mode.xEDB0;
    }
    switch (sel) {
    case 0:
        mode = rpg_bat_mode.xEBAC;
        rpg_bat_mode.xEC9C = 1;
        rpg_bat_mode.xEC74 = 10;
        rpg_bat_mode.xEC78 = 0;
        rpg_bat_mode.xEC7C = 10;
        rpg_bat_mode.xEC80 = 0;
        switch (mode) {
        case 0:
            rpg_bat_mode.xEC68 = 112;
            rpg_bat_mode.xEC5C = 16;
            rpg_bat_mode.xEC64 = -48;
            break;
        case 1:
            rpg_bat_mode.xEC68 = 168;
            rpg_bat_mode.xEC5C = 36;
            rpg_bat_mode.xEC64 = -28;
            class_w[155].prio = 4;
            class_w[157].prio = 4;
            break;
        case 2:
            rpg_bat_mode.xEC68 = 176;
            rpg_bat_mode.xEC5C = -24;
            rpg_bat_mode.xEC64 = -80;
            break;
        default:
            rpg_bat_mode.xEC68 = 184;
            rpg_bat_mode.xEC5C = 72;
            rpg_bat_mode.xEC64 = -80;
            break;
        }
        rpg_bat_state_next();
        return;
    case 1:
        n = rpg_bat_mode.xEC9C;
        x = rpg_bat_mode.xEC5C * n / rpg_bat_mode.xEC68;
        y = rpg_bat_mode.xEC64 * n / rpg_bat_mode.xEC68;
        x = -x;
        y = -y;
        sanshoku_bar_disp(n, x, y, 48, 5, 5, 52, 53, 191);
        sanshoku_bar_disp(rpg_bat_mode.xEC9C, x, y, 12, 20, 4, 50, 51, 223);
        rpg_bat_mode.xEC9C = rpg_bat_mode.xEC9C + 1;
        if (rpg_bat_mode.xEC68 < rpg_bat_mode.xEC9C) {
            rpg_bat_state_next();
            rpg_bat_mode.xEC9C = 1;
        }
        return;
    case 2:
        sanshoku_bar_disp(rpg_bat_mode.xEC68, -rpg_bat_mode.xEC5C, -rpg_bat_mode.xEC64, 48, 5, 5, 52, 53,
                      191);
        sanshoku_bar_disp(rpg_bat_mode.xEC68, -rpg_bat_mode.xEC5C, -rpg_bat_mode.xEC64, 12, 20, 4, 50, 51,
                      223);
        rpg_bat_mode.xEC9C = rpg_bat_mode.xEC9C + 1;
        if (rpg_bat_mode.xEC9C >= 121) {
            rpg_bat_state_next();
            rpg_bat_mode.xEC9C = 1;
        }
        return;
    case 3:
        n = rpg_bat_mode.xEC9C;
        x = rpg_bat_mode.xEC5C - n;
        y = n + rpg_bat_mode.xEC64;
        y = -y;
        x = -x;
        sanshoku_bar_disp(rpg_bat_mode.xEC68, x, y, 48, 5, 5, 52, 53, 191);
        sanshoku_bar_disp(rpg_bat_mode.xEC68, x, y, 12, 20, 4, 50, 51, 223);
        rpg_bat_mode.xEC9C = rpg_bat_mode.xEC9C + 1;
        if (rpg_bat_mode.xEC5C - rpg_bat_mode.xEC9C < rpg_bat_mode.xEC9C + rpg_bat_mode.xEC64) {
            rpg_bat_step_next();
        }
        return;
    }
}
