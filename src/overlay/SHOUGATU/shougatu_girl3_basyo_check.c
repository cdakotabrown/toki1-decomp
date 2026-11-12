/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_UGEN_DEST_TEMP=4,11,19 IDO52_UGEN_TEMP_REG=3:t9,4:t0,5:t1,6:t2,7:t2,8:t3,9:t4,10:t5,11:t6,12:t7 IDO52_AS1_XBB_VOLATILE_OK=0 */
/* @SymbolStart: 0x80138FD0 */
/* @SymbolSize: 252 */
#include "types.h"
extern void sub_sub_step_up();
extern volatile u8 now_girl;
extern u8 screen_w[];
extern u16 basyo_pack;
extern u8 girl3_basyo;
extern s16 shougatu_date_now_serifu;

void shougatu_girl3_basyo_check()
{
    int flagA;
    int flagB;

    flagA = 0;
    screen_w[now_girl * 56 + 456] |= 4;
    flagB = 0;
    if (4 == basyo_pack >> 12) {
        flagA = 1;
    } else {
        flagA = 0;
    }
    if (4 == girl3_basyo >> 4) {
        flagB = 1;
    } else {
        flagB = 0;
    }
    if (flagA == 0 || flagB == 0) {
        if (flagB != 0) {
            shougatu_date_now_serifu += 2;
        } else {
            shougatu_date_now_serifu += 4;
            screen_w[now_girl * 56 + 456] &= ~4;
        }
    }
    sub_sub_step_up();
}
