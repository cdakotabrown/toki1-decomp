/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_COPYPROP_SITE=lod:4:deny,lod:5:deny IDO52_FORCE_WEB_COLOR=14:a0,50:a2 IDO52_AS1_NO_PASS1_PROP=14 */
/* Address: 0x8014C5E8, Size: 136 bytes (34 instructions) */
#include "types.h"

extern u8 *tt_slot_get(u8 *base, u8 *p);
extern void tt_slot_hit_init(u8 *p);

extern u8 *tt_slot_buf;

void tt_slot_hit_make(u8 *src)
{
    u8 *v0;
    u8 *base;
    s32 z;

    base = tt_slot_buf;
    v0 = tt_slot_get(base, base + 7680);
    if (v0 == 0) return;
    *(u8 *)(v0 + 0) = 1;
    z = 0;
    *(s16 *)(v0 + 4) = 113;
    *(u8 *)(v0 + 14) = z;
    *(u8 *)(v0 + 13) = z;
    *(s16 *)(v0 + 10) = 0;
    *(s16 *)(v0 + 8) = 0;
    *(s16 *)(v0 + 6) = 0;
    *(u8 *)(v0 + 15) = 0;
    *(s32 *)(v0 + 32) = *(s32 *)(src + 12);
    *(s32 *)(v0 + 36) = *(s32 *)(src + 16);
    *(s16 *)(v0 + 20) = *(s16 *)(v0 + 34);
    *(s16 *)(v0 + 22) = *(s16 *)(v0 + 38);
    goto L;
L:
    tt_slot_hit_init(v0);
}
