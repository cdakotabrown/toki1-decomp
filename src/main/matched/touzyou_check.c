/* Address: 0x8006EC08 */
/* Size: 864 bytes (216 instructions) */
/* Region: Game (0x80041000-0x80086000) */
/* Compiler: IDO 5.2, -cckr (K&R) */
/* @CompileFlags: -Wc,-pic1 -cckr */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=18:a3,120:a2 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    u32 b0:1;
    u32 active:1;
    u32 b2:7;
    u32 kind:3;
    u32 b12:6;
    u32 done:1;
    u32 b19:1;
    u32 loc:4;
    u32 rest:8;
    u32 pad4;
    u8 stats[21];
    u8 pad[27];
} Chara;

typedef struct {
    s16 a;
    s16 b;
} Pair;

typedef struct {
    u8 pad[244];
    u16 slot;
    u8 pad2[10];
    Pair prm[8];
    u8 pad3[168];
    Chara chars[11];
} Save;

extern Save screen_w;
extern u32 sched_icon;
extern u32 frame_counter;

touzyou_check(sel, sub)
u8 *sel;
u8 *sub;
{

    *sel = 0;
    *sub = 0;
    if (rand() % 3 != 0) {
        return 0;
    }
    if (screen_w.chars[1].active == 0 && (sched_icon & 0xF) == 0 && screen_w.prm[0].b >= 55) {
        *sel = 1;
        *sub = frame_counter & 1;
        return 1;
    }
    if (screen_w.chars[2].active == 0 && (sched_icon & 0xF) == 1 && screen_w.prm[1].b >= 60) {
        *sel = 2;
        *sub = frame_counter & 3;
        return 1;
    }
    if (screen_w.chars[3].active == 0 && (sched_icon & 0xF) == 2 && screen_w.prm[2].b >= 50) {
        *sel = 3;
        *sub = 0;
        return 1;
    }
    if (screen_w.chars[5].active == 0 && (sched_icon & 0xF) == 3 && screen_w.chars[0].loc != 8
        && screen_w.prm[3].b >= 81 && screen_w.prm[5].b >= 81) {
        *sel = 5;
        *sub = 0;
        return 1;
    }
    if (screen_w.chars[6].active == 0 && (sched_icon & 0xF) == 3 && screen_w.chars[0].loc != 9
        && screen_w.prm[3].b >= 31 && screen_w.prm[6].b >= 31) {
        *sel = 6;
        *sub = 0;
        return 1;
    }
    if (screen_w.chars[7].active == 0 && (sched_icon & 0xF) == 6 && screen_w.prm[5].b >= 101) {
        *sel = 7;
        *sub = 0;
        return 1;
    }
    if (screen_w.chars[8].active == 0) {
        if (screen_w.chars[8].kind == 0 && screen_w.prm[4].b >= 36) {
            *sel = 8;
            *sub = 0;
            return 1;
        }
        if (screen_w.chars[8].kind == 1 && screen_w.prm[4].b >= 56) {
            *sel = 8;
            *sub = 1;
            return 1;
        }
        if (screen_w.chars[8].kind == 2 && screen_w.prm[4].b >= 76) {
            *sel = 8;
            *sub = 2;
            return 1;
        }
        if (screen_w.chars[8].kind == 3 && (sched_icon & 0xF) == 5 && screen_w.prm[4].b >= 91) {
            *sel = 8;
            *sub = 3;
            return 1;
        }
    }
    if (screen_w.chars[4].active == 0 && screen_w.chars[4].loc != screen_w.slot >> 12
        && screen_w.prm[6].b >= 36) {
        *sel = 4;
        *sub = 0;
        return 1;
    }
    return 0;
}
