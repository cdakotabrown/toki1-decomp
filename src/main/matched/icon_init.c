/* Address: 0x8006523C */
/* Size: 1076 bytes (269 instructions) */
/* Region: Game (IDO 5.2) */
/* @CompileFlags: -Wc,-pic1 */
/* @CompileEnv: IDO52_AS1_XBB_VOLATILE_OK=1 IDO52_FORCE_WEB_COLOR=110:a0 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    u8  pad0[3948];
    u8  mode;
    u8  count;
} BgState;

typedef struct {
    u8  b0;
    u8  b1;
    u8  b2;
    u8  b3;
    u8  b4;
    u8  b5;
    u8  b6;
    u8  b7;
    u8  pad8[4];
    void *p12;
    void *p16;
    s16 h20;
    s16 h22;
    s16 h24;
    u8  pad26[26];
    void *p52;
    u32 w56;
    u8  pad60[8];
} Slot68;

typedef struct {
    s16 x;
    s16 y;
} Pos;

extern BgState screen_w;
extern volatile u8 icon_mode;
extern volatile u8 icon_num;
extern u8 icon_can_use[];
extern Slot68 class_w[];
extern u8 basyo_init_tbl1[];
extern u8 basyo_init_tbl2[];
extern u8 _sys_default_tpage_set_tbl2[];
extern s16 basyo_init_tbl3;
extern u8 cw94_prio;
extern u8 cw78_prio;
extern s16 cw94_anim;
extern s16 cw94_x;
extern s16 cw94_y;
extern s16 cw78_anim;
extern s16 cw78_x;
extern s16 cw78_y;
extern Pos icon_xy[];
extern Pos icon_init_tbl1[];
extern Pos icon_init_tbl0[];
extern u8 hud_palette_init_tbl1[];
extern u8 hud_palette_init_tbl2[];
extern u8 icon_init_tbl2[];

extern void class_clear_n(s32);
extern void icon_menu_set(s32);
extern void load_image_rect(s32, s32, s32, s32, void *);

void icon_init(void)
{
    BgState *st;
    u32 i;
    s32 j;
    s32 cnt;

    st = &screen_w;
    if (icon_mode == 2) {
        icon_num = 15;
    }
    i = 0;
    if (icon_num != 0) {
        do {
            icon_can_use[i] = 1;
            i++;
        } while (i < st->count);
    }
    i = 0;
    if (icon_num != 0) {
        do {
            class_clear_n(i + 80);
            class_w[i + 80].b1 = 3;
            class_w[i + 80].b2 = 1;
            class_w[i + 80].w56 = 0x40000000;
            class_w[i + 80].b3 = 132;
            class_w[i + 80].p12 = basyo_init_tbl1;
            class_w[i + 80].p16 = basyo_init_tbl2;
            class_w[i + 80].p52 = _sys_default_tpage_set_tbl2;
            class_w[i + 80].h20 = basyo_init_tbl3;
            class_w[i + 80].b4 = 8;
            class_w[i + 80].b5 = 1;
            class_w[i + 80].h22 = i;
            class_w[i + 80].b6 = 1;
            class_clear_n(i + 64);
            class_w[i + 64].b1 = 4;
            class_w[i + 64].b2 = 64;
            class_w[i + 64].w56 = 0x40000000;
            class_w[i + 64].b3 = 132;
            class_w[i + 64].p12 = basyo_init_tbl1;
            class_w[i + 64].p16 = basyo_init_tbl2;
            class_w[i + 64].p52 = _sys_default_tpage_set_tbl2;
            class_w[i + 64].h20 = basyo_init_tbl3;
            class_w[i + 64].b4 = 8;
            class_w[i + 64].b5 = 1;
            class_w[i + 64].h24 = 2;
            i++;
        } while (i < st->count);
    }
    cw94_prio = 3;
    cw78_prio = 4;
    cw94_anim = 15;
    cw94_x = 0;
    cw94_y = 0;
    cw78_anim = 15;
    cw78_x = 0;
    cw78_y = 0;
    switch ((u32)icon_mode) {
    case 0:
        i = 0;
        if (icon_num != 0) {
            do {
                icon_xy[i].x = icon_init_tbl1[i].x;
                icon_xy[i].y = icon_init_tbl1[i].y;
                i++;
            } while (i < st->count);
        }
        break;
    case 1:
        i = 0;
        if (icon_num != 0) {
            do {
                icon_xy[i].x = icon_init_tbl0[i].x;
                icon_xy[i].y = icon_init_tbl0[i].y;
                i++;
            } while (i < st->count);
        }
        break;
    case 2:
        cnt = 0;
        for (i = 0; i < 15; i++) {
            if (icon_xy[i].x == 0 && icon_xy[i].y == 0) {
                cnt++;
            }
            if (cnt >= 2) {
                for (j = 0; j < 15; j++) {
                    icon_xy[j].x = icon_init_tbl1[j].x;
                    icon_xy[j].y = icon_init_tbl1[j].y;
                }
                break;
            }
        }
        break;
    }
    icon_menu_set(icon_num);
    load_image_rect(288, 497, 16, 1, hud_palette_init_tbl1);
    load_image_rect(288, 498, 16, 1, hud_palette_init_tbl2);
    load_image_rect(288, 499, 16, 1, icon_init_tbl2);
}
