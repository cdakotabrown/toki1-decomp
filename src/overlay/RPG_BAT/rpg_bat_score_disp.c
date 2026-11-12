/* @CompileFlags: -cckr -Wc,-pic1 */
/* @CompileEnv: IDO52_AS1_TIE_TAKE=377,379 */
/* rpg_bat_score_disp - RPG_BAT overlay, Address: 0x8014BFE4, Size: 892 bytes (223 insns) */
#include "types.h"

typedef struct {
    u8 pad00[38];
    s16 x;
    u8 pad28[2];
    s16 y;
    u8 pad2C[24];
} Rec68;

extern s32 rpg_bat_slot0_ctl;
extern s32 rpg_bat_slot0_ctl_mode;
extern s32 rpg_bat_slot0_ctl_cnt;
extern s32 rpg_bat_slot1_ctl;
extern s32 rpg_bat_slot1_ctl_mode;
extern s32 rpg_bat_slot1_ctl_cnt;
extern s32 rpg_bat_slot2_ctl;
extern s32 rpg_bat_slot2_ctl_mode;
extern s32 rpg_bat_slot2_ctl_cnt;
extern s32 rpg_bat_slot3_ctl;
extern s32 rpg_bat_slot3_ctl_mode;
extern s32 rpg_bat_slot3_ctl_cnt;
extern Rec68 class_w[];

rpg_bat_score_disp(sel, x, y, score, mode)
{
    s32 i;
    s32 j;
    s32 k;
    s32 div;

    x -= 160;
    y -= 120;
    if (score < 10) goto scaled;
    if (score < 100) {
        x += 4;
        goto scaled;
    }
    if (score < 1000) {
        x += 9;
        goto scaled;
    }
    x += 13;
scaled:
    div = 1;
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
    for (i = 0, j = sel * 12; i < 4; i++, j++) {
        class_w[j + 64].x = x - i * 9;
        class_w[j + 64].y = y;
        class_w[j + 68].x = x - i * 9;
        class_w[j + 68].y = y;
        class_w[j + 72].x = x - i * 9;
        class_w[j + 72].y = y;
        k = j + 64;
        if (score / div != 0) {
            rpg_bat_cw_anim_set(k + 4, score / div % 10 + 39, 0);
            rpg_bat_cw_anim_set(k + 8, score / div % 10 + 39, 0);
            if (mode == 0) {
                rpg_bat_cw_anim_set(k, score / div % 10 + 29, 0);
            } else {
                rpg_bat_cw_anim_set(k, score / div % 10 + 19, 0);
            }
        } else {
            class_w[k].x = 160;
            class_w[k].y = 120;
            class_w[k + 4].x = 160;
            class_w[k + 4].y = 120;
            class_w[k + 8].x = 160;
            class_w[k + 8].y = 120;
        }
        div *= 10;
    }
    if (score == 0) {
        class_w[sel * 12 + 64].x = x;
        class_w[sel * 12 + 64].y = y;
        class_w[sel * 12 + 68].x = x;
        class_w[sel * 12 + 68].y = y;
        class_w[sel * 12 + 72].x = x;
        class_w[sel * 12 + 72].y = y;
        k = sel * 12 + 64;
        rpg_bat_cw_anim_set(k + 4, 39, 0);
        rpg_bat_cw_anim_set(k + 8, 39, 0);
        if (mode == 0) {
            rpg_bat_cw_anim_set(k, 29, 0);
        } else {
            rpg_bat_cw_anim_set(k, 19, 0);
        }
    }
}
