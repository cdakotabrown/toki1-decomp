/* Likely Symbol: card_event_step (from o.bin, offset +20 bytes from 0x80054100) */
/* Address: 0x80054114 - 0x800541CC (184 bytes) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "common.h"

extern s32 Sw_Test(void);
extern void card_step_next(void);
extern void card_step_set(u8 arg0);

extern u8 card_event_cnt;
extern u8 card_busy;
extern u8 card_result;

void card_event_step(u8 arg0) {
    s32 result;

    result = Sw_Test();

    switch (result) {
        case 0:
            break;
        case 1:
            card_step_next();
            break;
        case 8:
            card_step_set(arg0);
            break;
        case 4:
            card_event_cnt++;
            card_step_set(0);
            if ((u32)card_event_cnt >= 3) {
                card_busy = 0;
                card_result = 255;
            }
            break;
        default:
            card_step_set(0);
            break;
    }
}
