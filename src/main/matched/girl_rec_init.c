/* Function: girl_rec_init */
/* Address: 0x80041DEC */
/* Size: 796 bytes (199 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @CompileFlags: -cckr -Wo,-loopunroll,0 -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    s16 f0;
    s16 f2;
    s16 f4;
    s16 f6;
    s16 f8;
    s16 f10;
    u16 b0 : 1;
    u16 b1 : 1;
    u16 b2 : 1;
    u16 b3 : 1;
    u16 b4 : 2;
    u16 b6 : 1;
    u16 b7 : 1;
    u16 b8 : 1;
    u16 b9 : 3;
    u16 b12 : 4;
    u16 c0 : 4;
    u16 c4 : 4;
    u16 c8 : 8;
    u16 d0 : 4;
    u16 d4 : 5;
    u16 d9 : 7;
    u8 pad[38];
} Rec;

typedef struct {
    u8 pad0[444];
    Rec rec[16];
} Save;

extern Save screen_w;

girl_rec_init()
{
    s32 i;

    bzero(screen_w.rec, 896);
    for (i = 0; i < 16; i++) {
        screen_w.rec[i].f2 = 0;
        screen_w.rec[i].f6 = 50;
        screen_w.rec[i].f10 = 0;
        screen_w.rec[i].c8 = 0;
        screen_w.rec[i].b1 = 0;
        screen_w.rec[i].b3 = 0;
        screen_w.rec[i].b6 = 0;
        screen_w.rec[i].b7 = 0;
        screen_w.rec[i].b9 = 0;
        screen_w.rec[i].c4 = 11;
    }
    screen_w.rec[0].b1 = 1;
    screen_w.rec[12].b1 = 1;
    screen_w.rec[0].b3 = 1;
    screen_w.rec[0].b6 = 1;
    screen_w.rec[0].b7 = 1;
    screen_w.rec[12].b3 = 1;
    screen_w.rec[0].d0 = 2;  screen_w.rec[0].d4 = 14;
    screen_w.rec[1].d0 = 2;  screen_w.rec[1].d4 = 3;
    screen_w.rec[2].d0 = 7;  screen_w.rec[2].d4 = 7;
    screen_w.rec[3].d0 = 9;  screen_w.rec[3].d4 = 30;
    screen_w.rec[4].d0 = 1;  screen_w.rec[4].d4 = 13;
    screen_w.rec[5].d0 = 6;  screen_w.rec[5].d4 = 13;
    screen_w.rec[6].d0 = 12; screen_w.rec[6].d4 = 3;
    screen_w.rec[7].d0 = 11; screen_w.rec[7].d4 = 15;
    screen_w.rec[8].d0 = 10; screen_w.rec[8].d4 = 17;
    screen_w.rec[9].d0 = 9;  screen_w.rec[9].d4 = 5;
    screen_w.rec[10].d0 = 5; screen_w.rec[10].d4 = 16;
    screen_w.rec[11].d0 = 3; screen_w.rec[11].d4 = 3;
    screen_w.rec[12].d0 = 8; screen_w.rec[12].d4 = 23;
    screen_w.rec[13].d0 = 4; screen_w.rec[13].d4 = 4;
}
