/* Function: nearest_menu_marker_find -- pick the nearest marker in a direction */
/* Address: 0x80050828 */
/* Size: 1784 bytes (446 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @CompileFlags: -Wc,-pic1 -cckr */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    s16 x;
    s16 y;
    u8 pad[3];
    u8 used;
} Mark;

typedef struct {
    u8 pad[3984];
    Mark mark[16][2];
    u8 count[3];
    s8 cur[3];
} Save;

extern Save screen_w;
extern s32 abs();

nearest_menu_marker_find(axis, mode, side)
s32 axis;
s32 mode;
s32 side;
{
    Save *sv;
    s32 i;
    s32 n;
    s16 best;
    s16 diff[16];
    s32 found;

    sv = &screen_w;
    n = 16;
    best = 320;
    if (sv->count[side] < 16) {
        n = sv->count[side];
    }
    switch (axis) {
    case 0:
        for (i = 0; i < n; i++) {
            diff[i] = sv->mark[i][side].x - sv->mark[sv->cur[side]][side].x;
        }
        i = 0;
        break;
    case 1:
        for (i = 0; i < n; i++) {
            diff[i] = sv->mark[i][side].y - sv->mark[sv->cur[side]][side].y;
        }
        i = 0;
        break;
    default:
        i = 0;
        break;
    }
    switch (mode) {
    case -1:
        for (i = 0; i < n; i++) {
            if (diff[i] > 0) {
                diff[i] = 0;
            } else {
                diff[i] = -diff[i];
            }
        }
        i = 0;
        break;
    case 1:
        for (i = 0; i < n; i++) {
            if (diff[i] < 0) {
                diff[i] = 0;
            }
        }
        i = 0;
        break;
    }
    found = -1;
    for (i = 0; i < n; i++) {
        if (sv->mark[i][side].used == 0 && diff[i] != 0) {
            if (axis == 0 && sv->mark[i][side].y == sv->mark[sv->cur[side]][side].y) {
                if (best > 0) {
                    best = -320;
                }
                if (-abs(sv->mark[i][side].x - sv->mark[sv->cur[side]][side].x) > best) {
                    best = -abs(sv->mark[i][side].x - sv->mark[sv->cur[side]][side].x);
                    found = i;
                }
            } else if (axis == 1 && sv->mark[i][side].x == sv->mark[sv->cur[side]][side].x) {
                if (best > 0) {
                    best = -320;
                }
                if (-abs(sv->mark[i][side].y - sv->mark[sv->cur[side]][side].y) > best) {
                    best = -abs(sv->mark[i][side].y - sv->mark[sv->cur[side]][side].y);
                    found = i;
                }
            } else if (best > 0) {
                if (diff[i] < best) {
                    best = diff[i];
                    found = i;
                } else if (best == diff[i]) {
                    switch (axis) {
                    case 0:
                        if (abs(sv->mark[found][side].y - sv->mark[sv->cur[side]][side].y) >
                            abs(sv->mark[i][side].y - sv->mark[sv->cur[side]][side].y)) {
                            found = i;
                        }
                        break;
                    case 1:
                        if (abs(sv->mark[found][side].x - sv->mark[sv->cur[side]][side].x) >
                            abs(sv->mark[i][side].x - sv->mark[sv->cur[side]][side].x)) {
                            found = i;
                        }
                        break;
                    }
                }
            }
        }
    }
    if (found >= 0) {
        return found;
    }
    return -1;
}
