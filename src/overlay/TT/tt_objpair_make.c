/* @CompileEnv: IDO52_COPYPROP_VAR=-12,-16,-20,-24 IDO52_CMOT_KILL_AVAIL=53,54 IDO52_UGEN_FORCE_TARGET_SAFE=5 IDO52_FORCE_WEB_COLOR=14:s6,17:s7,20:s8 IDO52_AS1_TIE_TAKE=201 */
/* tt_objpair_make - TT overlay, Address: 0x80134630, Size: 312 bytes (78 insns) */
#include "types.h"

extern u8 *get_tt_shot_free();
extern void tt_shot_sprite_set(u8 *obj, u16 pal, s32 n);
extern u8 *tt_shot_buf;

void tt_objpair_make(s16 arg0, s16 arg1, s32 arg2, u16 arg3)
{
    s32 i;
    u8 *obj;
    s32 stride;
    s32 k1;
    s32 k2;
    s32 sel;

    sel = arg3;
    obj = tt_shot_buf;
    i = 0;
    stride = 48;
    k1 = 160;
    k2 = 162;
    do {
        obj = get_tt_shot_free(obj, obj + 13312);
        if (obj == 0) {
            return;
        }
        *(s32 *)(obj + 32) = ((s32)arg0 - i * stride + 24) << 16;
        *(s32 *)(obj + 36) = ((s32)arg1) << 16;
        *(s32 *)(obj + 40) = 0;
        *(s32 *)(obj + 44) = arg2 << 16;
        if (sel == k1) goto A;
        if (sel == k2) goto B;
        i++;
        goto J;
    A:
        tt_shot_sprite_set(obj, k1, 18);
        i++;
        goto J;
    B:
        tt_shot_sprite_set(obj, k2, 20);
        i++;
    J:
        *(s16 *)(obj + 4) = 64;
        *(s16 *)(obj + 10) = 0;
        *(s16 *)(obj + 8) = 0;
        *(s16 *)(obj + 6) = 0;
        *(u8 *)(obj + 15) = 0;
        *(u8 *)(obj + 14) = 0;
        *(u8 *)(obj + 13) = 0;
    } while (i != 2);
}
