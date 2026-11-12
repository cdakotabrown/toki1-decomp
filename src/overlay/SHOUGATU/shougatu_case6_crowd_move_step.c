/* Address: 0x80141AA0 */
/* Size: 240 bytes (60 instructions) */
#include "types.h"

extern void shougatu_crowd_move(void);

typedef void (*FnPtr)(s32);

struct FnTable {
    FnPtr fp[40];
};

extern struct FnTable shougatu_main6_tbl;

extern u8 now_sub_sub_step;
extern u8 fade_bri;

struct Blk {
    u8 pad0[7];
    u8 b37;
    s16 b38;
    u8 pad1[12];
    s16 b46;
    s16 b48;
};
extern struct Blk cw96_active;

void shougatu_case6_crowd_move_step(void)
{
    s32 state;
    struct FnTable buf;
    s32 v;

    buf = shougatu_main6_tbl;
    state = now_sub_sub_step;
    buf.fp[state](128);

    state = now_sub_sub_step;
    if ((u32)state < 16U) {
        cw96_active.b37 = fade_bri;
        if (cw96_active.b48 == 0 && cw96_active.b38 == 0) {
            v = (s16)(cw96_active.b46 + 1);
            cw96_active.b46 = v;
            if (v >= 3) {
                cw96_active.b46 = 0;
            }
        }
    }
    shougatu_crowd_move();
}
