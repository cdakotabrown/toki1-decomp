/* @PostProc: patch_stack_complete --old 32 --new 64 */
/* @PostProc: remap_stack_slots --from 28 --to 44 --from 24 --to 48 */
/* @PostProc: swap_instructions --offset1 0x50 --offset2 0x54 */
/* taco_pair_spawn -- TACO overlay, Address: 0x801411A0, Size: 296 bytes */
#include "types.h"

extern void sd_cmd_limit_send(s32, s32);

extern u8 *taco_actor_rec;
extern u8 taco_effect_buf[];
extern u8 taco_effect2_buf[];

s32 taco_pair_spawn(s32 arg0, s32 arg1)
{
    u8 *a2;
    u8 *a3;
    u8 *p;
    s32 count;
    s32 off;

    if (arg0 == 0) {
        a3 = taco_actor_rec;
        a2 = taco_effect_buf;
    } else {
        a3 = taco_actor_rec + 1088;
        a2 = taco_effect2_buf;
    }

    off = 0;
    if (arg1 & 0x20) {
        count = 0;
        p = a2;
        do {
            off += 20;
            if ((*(u8 *)(p + 19) & 0x80) == 0) {
                count += 1;
            }
            p += 20;
        } while (off != 320);

        if (count != 16) {
            return -1;
        }

        if (arg0 == 0) {
            sd_cmd_limit_send(1, 1281);
        }

        *(u8 *)(a2 + 19) = 176;
        *(s16 *)(a2 + 0) = *(s16 *)(a3 + 116);
        *(s16 *)(a2 + 2) = *(s16 *)(a3 + 118) + 32;
        *(s16 *)(a2 + 4) = *(s16 *)(a3 + 120);
        *(s16 *)(a2 + 20) = *(s16 *)(a3 + 116);
        *(s16 *)(a2 + 22) = *(s16 *)(a3 + 118) + 32;
        *(s16 *)(a2 + 24) = *(s16 *)(a3 + 120);
        *(s16 *)(a2 + 8) = 0;
        *(s16 *)(a2 + 10) = 0;
        *(s16 *)(a2 + 12) = 0;
        *(u8 *)(a2 + 18) = 0;
        return 1;
    }
    return 0;
}
