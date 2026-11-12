/* Address: 0x80070D04 */
/* Size: 460 bytes (115 instructions) */
/* Region: Game (IDO 5.2) */
/* @CompileFlags: -Wc,-pic1 -cckr */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    s16 pad0;
    s16 cnt;
    u32 pad4;
    u32 b0 : 1;
    u32 b1 : 1;
    u32 rest : 30;
    u8 pad12[44];
} Girl;

typedef struct {
    u8 pad0[64];
    u8 day;
    u8 pad65[383];
    Girl rec[11];
} Save;

extern Save screen_w;

void girl_counter_periodic_decrement()
{
    s32 i;

    for (i = 0; i < 11; i++) {
        if (screen_w.rec[i].b1) {
            if (screen_w.day / 7 == 0) {
                if (screen_w.rec[i].cnt > 0) {
                    screen_w.rec[i].cnt--;
                }
            } else if (screen_w.day / 7 == 2 && i == 0) {
                if (screen_w.rec[i].cnt > 0) {
                    screen_w.rec[i].cnt--;
                }
            }
        }
    }
}
