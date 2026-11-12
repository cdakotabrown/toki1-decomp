/* @PostProc: scheduler_normalizer --block-aware */
/* @PostProc: patch_insn_reg_field 0x6c:rs:s4:v0 0x6c:rt:v0:s4 */
#include "types.h"

extern s32 tt_rec_init();
extern s32 tt_rec_uv_step(void *);
extern s32 tt_rec_move();

extern u8 *tt_rec_pool;

void tt_rec_step(void)
{
    u8 *p;
    s32 s1;
    s32 c64, c8, c32;
    u16 v0;

    p = tt_rec_pool;
    s1 = 0;
    c64 = 64;
    c8 = 8;
    c32 = 32;
    do {
        if (*(u8 *)(p + 0) != 0 && *(u8 *)(p + 1) == 0) {
            v0 = *(u16 *)(p + 4);
            switch (v0) {
            case 32:
                tt_rec_init(p, s1);
                *(u16 *)(p + 4) = c64;
                *(u16 *)(p + 10) = 0;
                *(u16 *)(p + 8) = 0;
                *(u16 *)(p + 6) = 0;
                *(u8 *)(p + 15) = 0;
                *(u8 *)(p + 14) = 0;
                *(u8 *)(p + 13) = 0;
                break;
            case 64:
                tt_rec_uv_step(p);
                break;
            default:
                break;
            }
            tt_rec_move(p, s1);
        }
        s1 += 1;
        p += 88;
    } while (s1 != c8);
}
