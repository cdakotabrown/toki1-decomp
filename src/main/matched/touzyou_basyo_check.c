/* Address: 0x8006F2BC */
/* Size: 780 bytes (195 instructions) */
/* Region: Game (0x80041000-0x80086000) */
/* @CompileFlags: -Wc,-pic1 -cckr */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    u32 b0:1;
    u32 active:1;
    u32 b2:16;
    u32 done:1;
    u32 b19:1;
    u32 loc:4;
    u32 rest:8;
    u32 pad4;
    u8 stats[21];
    u8 pad[27];
} Chara;

typedef struct {
    u16 cnt;
    u16 pad;
} Day;

typedef struct {
    u8 pad[456];
    Chara chars[11];
    u8 pad2[572];
    Day days[16];
} Save;

extern Save screen_w;
extern u16 basyo_pack;
extern u32 frame_counter;

touzyou_basyo_check(sel, sub)
u8 *sel;
u8 *sub;
{
    Save *sv;
    s32 slot;

    *sel = 0;
    *sub = 0;
    slot = basyo_pack >> 12;
    sv = &screen_w;
    if (rand() % 3 != 0) {
        return 0;
    }
    if (sv->chars[1].active == 0 && screen_w.chars[1].loc == slot && sv->days[slot].cnt >= 21) {
        screen_w.chars[1].done = 1;
        *sel = 1;
        if (slot == 0) {
            *sub = (frame_counter & 1) + 2;
        } else {
            *sub = 4;
        }
        return 1;
    }
    if (sv->chars[2].active == 0 && screen_w.chars[2].loc == slot && sv->days[slot].cnt >= 21) {
        screen_w.chars[2].done = 1;
        *sel = 2;
        if (slot == 2) {
            *sub = 4;
        } else {
            *sub = 5;
        }
        return 1;
    }
    if (sv->chars[3].active == 0 && screen_w.chars[3].loc == slot && sv->days[slot].cnt >= 21) {
        screen_w.chars[3].done = 1;
        *sel = 3;
        if (slot == 4) {
            *sub = 1;
        } else {
            *sub = 2;
        }
        return 1;
    }
    if (sv->chars[4].active == 0 && screen_w.chars[4].loc == slot && sv->days[slot].cnt >= 21) {
        screen_w.chars[4].done = 1;
        *sel = 4;
        if (slot == 6) {
            *sub = 1;
        } else {
            *sub = 2;
        }
        return 1;
    }
    if (sv->chars[5].active == 0 && slot == 8 && screen_w.chars[0].loc != 8 && sv->days[slot].cnt >= 21) {
        screen_w.chars[5].done = 1;
        *sel = 5;
        *sub = 1;
        return 1;
    }
    if (sv->chars[6].active == 0 && slot == 9 && screen_w.chars[0].loc != 9) {
        screen_w.chars[6].done = 1;
        *sel = 6;
        *sub = 1;
        return 1;
    }
    return 0;
}
