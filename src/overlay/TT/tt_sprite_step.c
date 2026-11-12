/* @CompileEnv: IDO52_CONST_NOINREG_VALUES=32,128 */
/* tt_sprite_step - TT overlay, Address: 0x8014AB38, Size: 420 bytes (105 insns) */
#include "types.h"

extern void tt_sprite_pos_init(u8 *rec, s32 index);
extern void tt_sprite_move(u8 *rec);

extern u8 *tt_player_shot_buf;

void tt_sprite_step(void)
{
    u8 *p;
    s32 i;

    p = tt_player_shot_buf;

    for (i = 0; i != 12; i++) {
        if (p[0] != 0) {
            switch (*(u16 *)(p + 4)) {
            case 32:
                tt_sprite_pos_init(p, i);
                *(s16 *)(p + 4) = 64;
                *(s16 *)(p + 10) = 0;
                *(s16 *)(p + 8) = 0;
                *(s16 *)(p + 6) = 0;
                p[15] = 0;
                p[14] = 0;
                p[13] = 0;
                break;

            case 128:
                p[66] = 208;
                p[67] = 208;
                *(s16 *)(p + 68) = 1;
                *(s16 *)(p + 4) = 129;
                *(s16 *)(p + 10) = 0;
                *(s16 *)(p + 8) = 0;
                *(s16 *)(p + 6) = 0;
                p[15] = 0;
                p[14] = 0;
                p[13] = 0;
                *(s32 *)(p + 44) = 0;
                *(s32 *)(p + 40) = 0;
                break;

            case 129:
                if (--*(u16 *)(p + 68) == 0) {
                    *(s16 *)(p + 4) = 130;
                }
                break;

            case 130:
                p[66] = 224;
                p[67] = 208;
                *(s16 *)(p + 68) = 1;
                *(s16 *)(p + 4) = 131;
                break;

            case 131:
                if (--*(u16 *)(p + 68) == 0) {
                    *(s16 *)(p + 4) = 160;
                }
                break;

            case 160:
                p[0] = 0;
                break;
            }

            tt_sprite_move(p);
        }

        p += 84;
    }
}
