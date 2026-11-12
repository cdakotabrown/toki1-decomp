/* @PostProc: pad_trailing_nop --count 1 */
/* Address: 0x8013AFAC */
/* Size: 404 bytes (101 insns) */
#include "types.h"

extern s32 vblnk_counter;
extern s32 cursor_work_bak[];
extern u8 *taco_drop_ptr;
extern s32 taco_drop_bright;
extern s32 taco_drop_ramp;

extern s32 rand(void);
extern void srand(s32 a0);

void taco_drop_init(void)
{
    s32 i;
    s32 off;
    s32 lim;
    s32 d30;
    s32 d5;
    s32 ramp;

    srand(vblnk_counter);
    taco_drop_ptr = (u8 *)cursor_work_bak;
    taco_drop_bright = 0;
    taco_drop_ramp = 0;
    i = 0;
    off = 0;
    lim = 100;
    d30 = 30;
    d5 = 5;
    do {
        if (i < 80) {
            if (rand() % d5 < 4) {
                ramp = i * 1000;
                *(s32 *)(taco_drop_ptr + off) = rand() % 4096;
                *(s32 *)(taco_drop_ptr + off + 4) = (ramp * i) / d30;
            } else {
                *(s32 *)(taco_drop_ptr + off + 4) = 0;
            }
        } else {
            *(s32 *)(taco_drop_ptr + off + 4) = 0;
        }
        i++;
        off += 8;
    } while (i != lim);
}
