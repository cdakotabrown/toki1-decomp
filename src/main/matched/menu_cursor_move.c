/* Address: 0x80050030 */
/* Size: 1812 bytes (453 instructions) */
/* @SymbolStart: 0x80050030 */
/* @SymbolSize: 1812 */
/* @CompileFlags: -Wc,-pic1 -cckr */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    s16 x;
    s16 y;
    u8 w;
    u8 h;
    u8 hit;
    u8 skip;
} Row;

typedef struct {
    u8 pad0[3984];
    Row row[16][2];
    u8 cnt[3];
    s8 sel[2];
    s8 chosen[2];
} Save;

extern Save screen_w;
extern s32 pad_trig;
extern s32 pad_data;
extern u8 menu_mode;
extern s16 cursor_cw_x;
extern s16 cursor_cw_y;
extern void step_directional_scroll_once();
extern s32 menu_sel_find();
extern s32 nearest_menu_marker_find();

void menu_cursor_move(slot)
    s32 slot;
{
    Save *sv;
    s32 r;
    s32 pad;
    s32 dir;
    s32 fuse;
    s32 i;

    pad = pad_trig;
    sv = &screen_w;
    dir = 0;
    fuse = 16;
    if (pad & 0x100) {
        menu_mode++;
        screen_w.sel[slot] = -1;
        return;
    }
    switch (menu_mode & 3) {
    case 0:
        step_directional_scroll_once();
        for (i = 0; i < sv->cnt[slot]; i++) {
            if (screen_w.row[i][slot].hit) {
                screen_w.sel[slot] = i;
                break;
            }
        }
        break;
    case 1:
        if (pad_data & 0xF) {
        } else {
            switch (pad & 0xF000) {
            case 0x1000:
            case 0x8000:
                dir = -1;
                screen_w.sel[slot]--;
                break;
            case 0x2000:
            case 0x4000:
                dir = 1;
                screen_w.sel[slot]++;
                break;
            default:
                break;
            }
        }
        if (screen_w.sel[slot] >= (int)screen_w.cnt[slot]) {
            screen_w.sel[slot] = 0;
        }
        if (screen_w.sel[slot] < 0) {
            screen_w.sel[slot] = screen_w.cnt[slot] - 1;
        }
        while (sv->row[sv->sel[slot]][slot].skip == 1) {
            if (fuse-- == 0) {
                break;
            }
            if (dir == 1) {
                screen_w.sel[slot] = screen_w.sel[slot] + 1;
            }
            if (dir == -1) {
                screen_w.sel[slot] = screen_w.sel[slot] - 1;
            }
            if (screen_w.sel[slot] >= (int)screen_w.cnt[slot]) {
                screen_w.sel[slot] = 0;
            }
            if (screen_w.sel[slot] < 0) {
                screen_w.sel[slot] = screen_w.cnt[slot] - 1;
            }
        }
        cursor_cw_x = screen_w.row[screen_w.sel[slot]][slot].x + screen_w.row[screen_w.sel[slot]][slot].w - 4;
        cursor_cw_y = sv->row[sv->sel[slot]][slot].y + 8;
        break;
    case 2:
        if (pad_data & 4) {
            if (pad & 4) {
                screen_w.sel[slot] = screen_w.chosen[slot];
            } else {
                switch (pad & 0xF000) {
                case 0x2000:
                case 0x4000:
                    dir = -1;
                    screen_w.sel[slot]++;
                    break;
                case 0x1000:
                case 0x8000:
                    dir = 1;
                    screen_w.sel[slot]--;
                    break;
                default:
                    break;
                }
                if (screen_w.sel[slot] >= (int)screen_w.cnt[slot]) {
                    screen_w.sel[slot] = 0;
                }
                if (screen_w.sel[slot] < 0) {
                    screen_w.sel[slot] = screen_w.cnt[slot] - 1;
                }
                while (sv->row[sv->sel[slot]][slot].skip == 1) {
                    if (fuse-- == 0) {
                        break;
                    }
                    if (dir == 1) {
                        screen_w.sel[slot] = screen_w.sel[slot] + 1;
                    }
                    if (dir == -1) {
                        screen_w.sel[slot] = screen_w.sel[slot] - 1;
                    }
                    if (screen_w.sel[slot] >= (int)screen_w.cnt[slot]) {
                        screen_w.sel[slot] = 0;
                    }
                    if (screen_w.sel[slot] < 0) {
                        screen_w.sel[slot] = screen_w.cnt[slot] - 1;
                    }
                }
            }
            cursor_cw_x = screen_w.row[screen_w.sel[slot]][slot].x + screen_w.row[screen_w.sel[slot]][slot].w - 4;
            cursor_cw_y = sv->row[sv->sel[slot]][slot].y + 8;
        } else {
            step_directional_scroll_once();
            for (i = 0; i < sv->cnt[slot]; i++) {
                if (screen_w.row[i][slot].hit) {
                    screen_w.sel[slot] = i;
                }
            }
        }
        break;
    case 3:
        i = -1;
        switch (pad & 0xF000) {
        case 0x1000:
            i = nearest_menu_marker_find(1, -1, slot);
            break;
        case 0x4000:
            i = nearest_menu_marker_find(1, 1, slot);
            break;
        case 0x2000:
            i = nearest_menu_marker_find(0, 1, slot);
            break;
        case 0x8000:
            i = nearest_menu_marker_find(0, -1, slot);
            break;
        }
        if (i != -1) {
            screen_w.sel[slot] = i;
        }
        if (screen_w.sel[slot] >= (int)screen_w.cnt[slot]) {
            screen_w.sel[slot] = menu_sel_find(slot, 1);
        }
        if (screen_w.sel[slot] < 0) {
            screen_w.sel[slot] = menu_sel_find(slot, 0);
        }
        cursor_cw_x = screen_w.row[screen_w.sel[slot]][slot].x + screen_w.row[screen_w.sel[slot]][slot].w - 4;
        cursor_cw_y = sv->row[sv->sel[slot]][slot].y + 8;
        break;
    }
    if (cursor_cw_x > 140) {
        cursor_cw_x = 140;
    }
    if (cursor_cw_x < -150) {
        cursor_cw_x = -150;
    }
    if (cursor_cw_y < -100) {
        cursor_cw_y = -100;
    }
    if (cursor_cw_y > 100) {
        cursor_cw_y = 100;
    }
}
