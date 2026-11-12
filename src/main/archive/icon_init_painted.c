/* Function: icon_init */
/* Address: 0x8006523C */
/* Size: 1076 bytes (269 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: custom_script custom_8006523C.py */
#include "types.h"

extern u8 icon_mode;
extern u8 icon_num;
extern u8 screen_w;
extern u8 icon_can_use;
extern u8 class_w;
extern u32 basyo_init_tbl1;
extern u32 basyo_init_tbl2;
extern u32 _sys_default_tpage_set_tbl2;
extern s16 basyo_init_tbl3;
extern u8 cw94_prio;
extern u8 cw78_prio;
extern s16 cw94_anim;
extern s16 cw94_x;
extern s16 cw94_y;
extern s16 cw78_anim;
extern s16 cw78_x;
extern s16 cw78_y;
extern s16 icon_xy[];
extern s16 icon_init_tbl1[];
extern s16 icon_init_tbl0[];
extern s16 D_800BBFF0[];
extern s16 D_8012DC7C[];
extern u32 hud_palette_init_tbl1;
extern u32 hud_palette_init_tbl2;
extern u32 icon_init_tbl2;

extern void class_clear_n(s32);
extern void icon_menu_set(s32);
extern void func_80044568(s32, s32, s32, s32, s32);

void icon_init(void) {
    u8 *a1;
    s32 s1;
    u8 *s0;
    ;

    a1 = &screen_w;

    if (icon_mode == 2) {
        icon_num = 15;
    }

    s1 = 0;
    if (icon_num != 0) {
        u8 *p = &icon_can_use;
        do {
            *p = 1;
            s1++;
            p++;
        } while ((u32)s1 < a1[3949]);
    }

    s1 = 0;
    if (icon_num != 0) {
        s0 = &class_w;

        do {
            class_clear_n(s1 + 80);
            s0[5441] = 3;
            s0[5442] = 1;
            *(u32 *)&s0[5496] = 0x40000000;
            s0[5443] = 132;
            *(u32 *)&s0[5452] = (u32)&basyo_init_tbl1;
            *(u32 *)&s0[5456] = (u32)&basyo_init_tbl2;
            *(u32 *)&s0[5492] = (u32)&_sys_default_tpage_set_tbl2;
            *(s16 *)&s0[5460] = basyo_init_tbl3;
            s0[5444] = 8;
            s0[5445] = 1;
            *(s16 *)&s0[5462] = (s16)s1;
            s0[5446] = 1;

            class_clear_n(s1 + 64);
            s0[4353] = 4;
            s0[4354] = 64;
            *(u32 *)&s0[4408] = 0x40000000;
            s0[4355] = 132;
            *(u32 *)&s0[4364] = (u32)&basyo_init_tbl1;
            *(u32 *)&s0[4368] = (u32)&basyo_init_tbl2;
            *(u32 *)&s0[4404] = (u32)&_sys_default_tpage_set_tbl2;
            *(s16 *)&s0[4372] = basyo_init_tbl3;
            s0[4356] = 8;
            s0[4357] = 1;
            *(s16 *)&s0[4376] = 2;

            s1++;
            s0 += 68;
        } while ((u32)s1 < a1[3949]);
    }

    cw94_prio = 3;
    cw78_prio = 4;
    cw94_anim = 15;
    cw94_x = 0;
    cw94_y = 0;
    cw78_anim = 15;
    cw78_x = 0;
    cw78_y = 0;

    if (icon_mode == 0) {
        s1 = 0;
        if (icon_num != 0) {
            s16 *v1 = icon_init_tbl1;
            s16 *v0 = icon_xy;
            do {
                v0[0] = v1[0];
                v0[1] = v1[1];
                s1++;
                v0 += 2;
                v1 += 2;
            } while ((u32)s1 < a1[3949]);
        }
    } else if (icon_mode == 1) {
        s1 = 0;
        if (icon_num != 0) {
            s16 *v1 = icon_init_tbl0;
            s16 *v0 = icon_xy;
            do {
                v0[0] = v1[0];
                v0[1] = v1[1];
                s1++;
                v0 += 2;
                v1 += 2;
            } while ((u32)s1 < a1[3949]);
        }
    } else {
        s32 cnt;
        s16 *v0;
        s16 *v1;
        s32 i;

        cnt = 0;
        v0 = icon_xy;
        v1 = (s16 *)((u8 *)icon_xy + 60);
        do {
            if (v0[0] == 0 && v0[1] == 0) {
                cnt++;
            }
            if (cnt >= 2) {
                break;
            }
            v0 += 2;
        } while (v0 != v1);

        if (cnt >= 2) {
            icon_xy[0] = icon_init_tbl1[0];
            icon_xy[1] = icon_init_tbl1[1];
            icon_xy[2] = icon_init_tbl1[2];
            icon_xy[3] = icon_init_tbl1[3];
            icon_xy[4] = icon_init_tbl1[4];
            icon_xy[5] = icon_init_tbl1[5];

            for (i = 0; i < 12; i++) {
                D_8012DC7C[i * 2] = D_800BBFF0[i * 2];
                D_8012DC7C[i * 2 + 1] = D_800BBFF0[i * 2 + 1];
            }
        }
    }

    icon_menu_set((s32)icon_num);
    func_80044568(288, 497, 16, 1, (s32)&hud_palette_init_tbl1);
    func_80044568(288, 498, 16, 1, (s32)&hud_palette_init_tbl2);
    func_80044568(288, 499, 16, 1, (s32)&icon_init_tbl2);
}
