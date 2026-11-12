/* @CompileEnv: IDO52_FORCE_WEB_COLOR=49:v1,48:a0 */
/* Address: 0x801478A4 */
/* Size: 204 bytes (51 instructions) */
#include "types.h"

typedef struct {
    u8 flag;
    u8 pad1[3];
    u8 voice;
    u8 pad2[63];
} EvRec;

extern volatile u8 D_800EAFA7;
extern s32 ev_flag04;
extern EvRec D_800E9E1B[];
extern s32 D_800EC190[];

extern void func_8004C984(void);
extern void func_8004C060(s32, s32);

s32 func_801478A4(void) {
    u32 x;
    s32 r;

    func_8004C984();
    x = D_800EAFA7;
    ev_flag04 = 1;
    D_800E9E1B[1].voice = x;
    D_800E9E1B[0].voice = x;
    func_8004C060(0, x);
    func_8004C060(1, D_800EAFA7);
    if ((u32)D_800EAFA7 < 8) {
        r = -129;
        ev_flag04 = 0;
        D_800E9E1B[1].flag = D_800E9E1B[1].flag & r;
        D_800E9E1B[0].flag = D_800E9E1B[0].flag & r;
        D_800EC190[0] = D_800EC190[0] | 0x80000000;
        D_800EC190[9] = D_800EC190[9] | 0x80000000;
        return r;
    }
}
