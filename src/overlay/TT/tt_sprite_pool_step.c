/* @CompileEnv: IDO52_FORCE_WEB_COLOR=0:s4,74:s2,75:s3,76:s5,72:s6,73:s7 */
/* Address: 0x8013C0A8, Size: 340 bytes (85 insns) */
#include "types.h"

extern void tt_sprite_pool_move(u8 *p);
extern void tt_anim(u8 *a, u8 *b, u8 *c);

extern u8 *tt_scroll_wk;
extern u8 *tt_bell_buf;

void tt_sprite_pool_step(void)
{
    u8 *p;
    u8 *q;
    s32 i;
    s32 st;

    q = tt_scroll_wk;
    p = tt_bell_buf;
    i = 0;
    do {
        if (*(u8 *)(p + 0) != 0) {
            st = *(u16 *)(p + 4);
            switch ((u32)st) {
            case 64:
                break;
            case 144:
                if ((u16)(*(u16 *)(p + 98) += 128) >= 4097U) {
                    *(u16 *)(p + 98) = 4096;
                    *(u16 *)(p + 4) = 145;
                }
                break;
            case 145:
                *(s32 *)(p + 44) = *(s32 *)q;
                *(u16 *)(p + 90) = 60;
                *(u16 *)(p + 4) = 146;
                break;
            case 146:
                *(s32 *)(p + 44) = *(s32 *)q;
                if ((u16)(*(u16 *)(p + 90) -= 1) == 0) {
                    *(u8 *)(p + 0) = 0;
                }
                break;
            }
            tt_sprite_pool_move(p);
            if ((u32) * (u16 *)(p + 4) < 144U) {
                tt_anim(p + 56, p + 86, p + 87);
            }
        }
        i++;
        p += 108;
    } while (i != 32);
}
