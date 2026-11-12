/* @CompileFlags: -cckr */
/* @SymbolSize: 532 */
/* @CompileEnv: IDO52_COPYPROP_VAR=-24 IDO52_FORCE_WEB_RANGE_SPLIT=26 IDO52_FORCE_WEB_NOSPILL=26.0:5 */
/* obj_pool_disp -- TT overlay, Address: 0x80135148, Size: 536 bytes */
#include "types.h"

extern u8 *tt_disp_wk;
extern u8 *tt_scale_buf;
extern u8 *tt_shot_buf;
extern u8 *tt_db_buf;

extern void anim_sprite_disp();
extern void tt_record_copy();
extern void tt_sparkle_disp();
extern void tt_sprite_ot_add_16x16();

typedef struct {
    u8 pad[0x8008];
    u32 tbl[1];
} Ot;

void obj_pool_disp()
{
    u8 *s0;
    u8 *s1;
    u8 *s2;
    int i;
    u8 *p;
    u8 *q;

    s1 = tt_db_buf + *(s32 *)(tt_disp_wk + 8) * 32952;
    s2 = tt_scale_buf;
    s0 = tt_shot_buf;
    for (i = 0; i != 128; i++, s0 += 104) {
        if (s0[0] == 0) {
            continue;
        }
        if (171 == *(u16 *)(s0 + 2)) {
            anim_sprite_disp(s0 + 20, s0 + 56, *(u16 *)(s0 + 16));
            continue;
        }
        if (172 == *(u16 *)(s0 + 2)) {
            tt_record_copy(s0);
            continue;
        }
        if (173 == *(u16 *)(s0 + 2)) {
            tt_sparkle_disp(s0);
            continue;
        }
        p = *(u8 **)(s1 + 4);
        *(u8 **)(s1 + 4) = p + 48;
        *(s16 *)(s2 + 4) = *(s16 *)(s0 + 20);
        *(s16 *)(s2 + 6) = *(s16 *)(s0 + 22);
        *(s16 *)(p + 8) = *(s16 *)(s0 + 20) - 8;
        *(s16 *)(p + 10) = *(s16 *)(s0 + 22) - 8;
        p[41] = 0;
        p[42] = 1;
        *(s16 *)(p + 16) = 16;
        *(s16 *)(p + 18) = 16;
        p[40] = s0[60];
        q = p;
        if (*(u16 *)(s0 + 2) != 168 && *(u16 *)(s0 + 2) != 169
            && *(u16 *)(s0 + 2) != 170) {
            p[12] = s0[86];
            p[13] = s0[87] + 128;
        } else {
            q[12] = s0[86];
            q[13] = s0[87];
        }
        *(u16 *)(q + 22) = *(u16 *)(s0 + 82);
        *(u16 *)(q + 14) = *(u16 *)(s0 + 84);
        q[43] = 0;
        q[44] = 0;
        tt_sprite_ot_add_16x16(&((Ot *)s1)->tbl[*(u16 *)(s0 + 16)], q, s2);
    }
}
