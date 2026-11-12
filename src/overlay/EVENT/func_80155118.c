/* @SymbolStart: 0x80155110 */
/* @SymbolSize: 608 */
/* @CompileFlags: -cckr */
#include "types.h"

typedef struct {
    u8 pad0[2];
    s16 a;
    u8 pad4[6];
    s16 b;
    u32 f0 : 4;
    u32 f4 : 1;
    u32 f5 : 27;
    u32 bday;
    u8 cnt[32];
} Chr;

typedef struct {
    u8 pad0[420];
    Chr chr[25];
    u8 pad[29];
    u8 last[16];
    u8 pad2[3814 - 1765];
    u8 cur;
    u8 pad3[4752 - 3815];
    s32 busy;
    u8 kind;
} Save;

extern Save D_800B0860;
extern s32 ev_flag18;
extern s16 D_800B0966;
extern u8 D_800B0896;
extern u8 D_800B0897;
extern u8 D_801243B0;
extern u8 D_800B0F2C;

void func_80155118()
{
    if (D_800B0860.busy == 0 && ev_flag18 >= 0 && D_800B0860.kind != 96) {
        D_800B0860.chr[D_800B0860.cur].cnt[ev_flag18]++;
    }
    if (D_800B0860.busy == 0) {
        if (ev_flag18 >= 0) {
            D_800B0860.last[D_800B0860.cur] = ev_flag18;
        }
        if (ev_flag18 == 7) {
            D_800B0860.last[D_800B0860.cur] = 6;
        }
        if (ev_flag18 == 5) {
            D_800B0860.last[D_800B0860.cur] = 4;
        }
        if (ev_flag18 == 11 || ev_flag18 == 12 || ev_flag18 == 13) {
            D_800B0860.last[D_800B0860.cur] = 10;
        }
    }
    if (D_800B0966 >= 35 || (D_800B0966 >= 30 && D_800B0860.cur != 7)) {
        D_800B0860.chr[D_800B0860.cur].b -= 40;
    } else {
        if (D_801243B0 == 18) {
            D_800B0860.chr[D_800B0860.cur].a++;
        }
        if ((D_800B0896 | 0) == 5 && (D_800B0897 == 4 || (D_800B0897 | 0) == 5)) {
            if (D_800B0860.chr[D_800B0860.cur].f4) {
                D_800B0860.chr[D_800B0860.cur].a += 2;
            }
        }
    }
    func_8004C250();
    D_800B0F2C = D_801243B0;
    func_80014CA8(0, 0);
    func_8003E638(1);
}
