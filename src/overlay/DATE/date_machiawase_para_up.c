/* @SymbolStart: 0x8013F38C */
/* @SymbolSize: 148 */
/* @CompileFlags: -cckr */
#include "types.h"

typedef struct {
    s16 a;
    s16 pad02;
    s16 b;
    s16 pad06;
    s16 c;
    u8  pad0A[46];
} GirlRec;

typedef struct {
    u8      pad000[446];
    GirlRec rec[24];
} SaveBlk;

extern SaveBlk screen_w;
extern u8  now_basyo;
extern u8  machiawase_basyo;
extern u8  now_sub_sub_step;
extern u8  now_girl;
extern s16 date_now_serifu0;

void sub_sub_step_up();
void param_clamp();

void date_machiawase_para_up()
{
    if (now_basyo != machiawase_basyo) {
        date_now_serifu0 = 7;
        now_sub_sub_step += 20;
        return;
    }
    sub_sub_step_up();
    sub_sub_step_up();
    screen_w.rec[now_girl].c += 8;
    param_clamp();
}
