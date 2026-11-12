/* @CompileEnv: IDO52_FORCE_WEB_SPILL=0,10 IDO52_NO_CSE_SITE=cvt:1 */
/* @SymbolStart: 0x80134130 */
/* @SymbolSize: 200 */
/* kangei_girl_event_check - KANGEI overlay, Address: 0x80134138 (head at 0x80134130), */
#include "types.h"

typedef struct {
    s32 b0 : 1;
    s32 b1 : 1;
    s32 rest : 30;
    u8 pad[52];
} KRec;

extern void kangei_switch(void);
extern void kangei_switch2(void);
extern void sub_step_up(void);

extern KRec girl0_flag[11];
extern u8 now_girl;
extern u8 D_800EF6BF_b;
extern u8 now_year;

void kangei_girl_event_check(void)
{
    if (now_girl != 255) {
        if (girl0_flag[now_girl].b1) {
            kangei_switch();
            return;
        }
    }
    if (D_800EF6BF_b != 255) {
        if (!girl0_flag[D_800EF6BF_b].b1) {
            if ((u32)now_year < 97U) {
                if (D_800EF6BF_b != 10) {
                    kangei_switch2();
                    return;
                }
            }
        }
    }
    sub_step_up();
}
