/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_COPYPROP_SITE=lod:4:deny,lod:5:deny,lod:6:deny,lod:7:deny IDO52_AS1_NO_PASS1_PROP=21,22 IDO52_FORCE_WEB_COLOR=13:a2,16:a0 */
/* @PostProc: remap_stack_slots --from 40 --to 44 */
/* tt_shot_polar_make - TT overlay, Address: 0x80134768, Size: 268 bytes (67 insns) */
#include "types.h"
#include "tokimeki/tt_box.h"
extern u8 *tt_shot_buf;
extern u8 tt_anim_tbl[];
extern TT_BOX tt_hit_box_tbl[];

extern u8 *get_tt_shot_free();
extern int rcos();
extern int rsin();

void tt_shot_polar_make(posX, posY, mul, angle)
short posX;
short posY;
int mul;
int angle;
{
    u8 *ptr;
    u8 *v0;
    int one;
    int z;

    ptr = tt_shot_buf;
    v0 = get_tt_shot_free(ptr, ptr + 13312);
    if (v0 == 0) {
        return;
    }
    one = 1;
    z = 0;
    *(u8 *)(v0 + 0) = one;
    *(s16 *)(v0 + 2) = 171;
    *(s16 *)(v0 + 4) = 64;
    *(s16 *)(v0 + 8) = z;
    *(s16 *)(v0 + 6) = z;
    *(s16 *)(v0 + 10) = 0;
    v0[15] = 0;
    v0[14] = 0;
    v0[13] = 0;
    *(s16 *)(v0 + 16) = one;
    *(s32 *)(v0 + 32) = posX << 16;
    *(s32 *)(v0 + 36) = posY << 16;
    goto L;
L:
    *(s32 *)(v0 + 40) = rcos(angle) * mul;
    *(s32 *)(v0 + 44) = rsin(angle) * mul;
    *(s32 *)(v0 + 56) = (s32)tt_anim_tbl;
    v0[60] = 0;
    v0[61] = 9;
    v0[62] = 20;
    v0[64] = 96;
    v0[65] = 0;
    *(s32 *)(v0 + 76) = (s32)((u8 *)&tt_hit_box_tbl[5].x);
}
