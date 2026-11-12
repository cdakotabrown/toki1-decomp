/* @CompileEnv: IDO52_FORCE_WEB_COLOR=4:v1,51:a0,52:v0,53:v0,54:v0,55:v1 */
/* @PostProc: swap_branch_operands --offsets 0x20 */
/* master_log_add - MASTER overlay, Address: 0x8013B804, 420 bytes. */
#include "types.h"

typedef struct {
    u8 code;
    u8 sub;
    u8 kind;
    u8 res;
    u8 p[4];
} LogRec;

typedef struct {
    u8 pad0[0x3E];
    u8 stat[4];
    u8 pad42[0xF4 - 0x42];
    u16 date;
    u8 padF6[0x2AA - 0xF6];
    u8 rank;
    u8 pad2AB[0x75E - 0x2AB];
    LogRec log[256];
    u8 log_n;
    u8 girl;
} GameState;

extern GameState screen_w;

void master_log_add(void)
{
    switch ((u32)screen_w.date >> 12) {
    case 7:
        screen_w.log[screen_w.log_n].sub = 7;
        break;
    case 8:
        screen_w.log[screen_w.log_n].sub = 8;
        break;
    case 9:
        screen_w.log[screen_w.log_n].sub = 9;
        break;
    case 10:
        screen_w.log[screen_w.log_n].sub = 10;
        break;
    }
    screen_w.log[screen_w.log_n].code = screen_w.girl;
    screen_w.log[screen_w.log_n].kind = 21;
    screen_w.log[screen_w.log_n].res = 5;
    screen_w.log[screen_w.log_n].p[0] = screen_w.stat[0];
    screen_w.log[screen_w.log_n].p[1] = screen_w.stat[1];
    screen_w.log[screen_w.log_n].p[2] = screen_w.stat[2];
    screen_w.log[screen_w.log_n].p[3] = screen_w.stat[3];
    if (++screen_w.log_n >= 255U) {
        screen_w.log_n = 254;
    }
}
