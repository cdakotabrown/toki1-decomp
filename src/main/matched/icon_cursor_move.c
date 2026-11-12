/* Address: 0x80065C74 */
/* Size: 1008 bytes (252 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* @PostProc: strip_leading_instructions --count auto */
/* @CompileFlags: -Wc,-pic1 -cckr */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=11:s3 IDO52_AS1_XBB_SKIP_CAND=16 IDO52_AS1_XBB_VOLATILE_OK=8 */
#include "types.h"

typedef struct {
    u8  b0;
    u8  b1;
    u8  b2;
    u8  pad3[23];
    s16 h26;
    s16 h28;
    u8  pad30[8];
    s16 xhi;
    u8  pad40[2];
    s16 yhi;
    u8  pad44[23];
    u8  b67;
} Slot68;

typedef struct {
    s16 x;
    s16 y;
} Pos4;

typedef struct {
    u8 b0;
    u8 pad[15];
} Rec16;

extern u32    pad_data;
extern volatile s8 now_icon;
extern u8     icon_mode;
extern u8     icon_num;
extern u8     csr_speed;
extern Pos4   icon_xy[];
extern s8     icon_cursor_move_tbl0;
extern u8     icon_cursor_move_tbl1;
extern u8     icon_can_use[];
extern Rec16  menu0_hit0[];
extern Slot68 class_w[];
extern u8     card_busy;
extern s16    cw93_anim;

extern s32  icon_atari_check(s32 state, s32 dx, s32 dy);
extern void icon_menu_set(u8 arg0);
extern void menu_check(s32 a0, s16 a1, s16 a2);
extern void menu_cursor_move(s32 a0);
extern s32  rsin(s32 arg);

void icon_cursor_move(s32 mode) {
    s32 dx;
    s32 dy;
    s32 count;
    u8  prev;
    u32 pad;
    s32 i;
    u8  mag;
    s32 state;

    dx = 0;
    dy = 0;
    pad = pad_data;
    state = now_icon;
    prev = state;
    if (pad & 1) {
        if (icon_mode % 3 == 2) {
            mag = csr_speed;
            switch (pad & 0xA000) {
            case 0x8000:
                dx = -mag;
                break;
            case 0x2000:
                dx = mag;
                break;
            }
            switch (pad & 0x5000) {
            case 0x1000:
                dy = -mag;
                break;
            case 0x4000:
                dy = mag;
                break;
            }
            if (icon_atari_check(state, dx, dy) == 0) {
                dx = 0;
                dy = 0;
            }
            state = now_icon;
            icon_xy[state].x += dx;

            state = state | 0;
            icon_xy[state].y += dy;
            class_w[0].xhi += dx;
            class_w[0].yhi += dy;
            icon_menu_set(icon_num);
            state = now_icon;
            if (icon_xy[state].x > 128) {
                icon_xy[state].x = 128;
            }
            if (icon_xy[state].x < -138) {
                icon_xy[state].x = -138;
            }
            if (icon_xy[state].y < -92) {
                icon_xy[state].y = -92;
            }
            if (icon_xy[state].y > 92) {
                icon_xy[state].y = 92;
            }
        }
    } else {

        if (icon_mode % 3 == 2) {
        }
        menu_check(0, class_w[0].xhi, class_w[0].yhi);
        menu_cursor_move(0);
        state = now_icon;
    }

    if (prev != icon_cursor_move_tbl0) {
        icon_cursor_move_tbl0 = state;
        icon_cursor_move_tbl1 = 32;
    } else if (icon_cursor_move_tbl1 != 0) {
        icon_cursor_move_tbl1 = icon_cursor_move_tbl1 - 1;
    }

    count = 14;
    if (icon_num == 15) {
        count = 15;
    }
    if (icon_mode == 2) {
        count = 15;
    }

    for (i = 0; i < count; i++) {
        if (icon_can_use[i] == 0) {
            class_w[i + 80].b2 = 0;
            class_w[i + 64].b67 = 81;
            class_w[i + 64].b1 = 3;
        } else {
            class_w[i + 80].b2 = 1;
            class_w[i + 64].b1 = 4;
            class_w[i + 80].b1 = 3;
            if (menu0_hit0[i].b0 != 0) {
                class_w[i + 64].b67 = 82;
                class_w[i + 80].b1 = 2;
                class_w[i + 64].b1 = 3;
                if (mode == 0) {
                    class_w[i + 80].h26 = rsin((32 - icon_cursor_move_tbl1) << 6) + 4096;
                    class_w[i + 80].h28 = rsin((32 - icon_cursor_move_tbl1) << 6) + 4096;
                } else {
                    class_w[i + 80].h26 = 4096;
                    class_w[i + 80].h28 = 4096;
                }
            } else {
                class_w[i + 64].b67 = 83;
                if (mode == 0) {
                    class_w[i + 80].h26 = 4096;
                    class_w[i + 80].h28 = 4096;
                    class_w[i + 64].h26 = 4096;
                    class_w[i + 64].h28 = 4096;
                } else {
                    class_w[i + 80].h26 = 4096;
                    class_w[i + 80].h28 = 4096;
                }
            }
        }
    }

    if (card_busy != 0) {
        cw93_anim = 14;
    } else {
        cw93_anim = 13;
    }
}
