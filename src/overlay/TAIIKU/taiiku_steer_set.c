/* @SymbolStart: 0x8013CF7C */
/* @SymbolSize: 452 */
/* @PostProc: patch_stack_complete --old 64 --new 80 */
/* @PostProc: remap_stack_slots --map '48:56,52:60,56:64,60:68' */
/* Address: 0x8013CF84 (true entry 0x8013CF7C -- the hoisted taiiku_tama_fall head */
/* Size: 452 bytes (113 insns) including the absorbed head */
#include "types.h"

extern s32 pad_trig;
extern s32 taiiku_course_no;
extern s16 cal_cw_x;
struct { s32 x; s32 y; } taiiku_tama_push = {0, 0};
extern u16 taiiku_tama_push_tick;
extern s32 taiiku_tama_vel;
extern s32 taiiku_tama_vel_1;
extern s16 taiiku_tama_sx;
extern s32 taiiku_tama_fall;
struct { s32 x; s32 y; } taiiku_tama_ppush = {0, 0};
extern s32 taiiku_tama_gpush;
extern s32 taiiku_tama_gpush_1;
extern u16 taiiku_tama_ghold;

extern s32 taiiku_near_check(void);
extern void xy_rotate(s32, s32 *, s32 *, s32 *, s32 *);

void taiiku_steer_set(void)
{
    s32 sp68;
    s32 sp64;
    s32 sp60;
    s32 sp56;
    s32 ang;
    s32 base;

    if (taiiku_tama_fall != 0) {
        taiiku_tama_push.x = 0;
        taiiku_tama_push.y = 0;
        goto end;
    }
    if (taiiku_near_check() == 0) {
        goto tick;
    }
    if ((pad_trig & 0x20) == 0) {
        goto tick;
    }
    if (taiiku_tama_vel_1 < 0) {
        taiiku_tama_vel_1 = 0;
    }
    base = ((s32)(((taiiku_tama_sx >> 3) - cal_cw_x) << 10) >> 6);
    ang = -base;
    if ((taiiku_course_no % 3) != 0) {
        ang = -base - 512;
    }
    sp68 = 0;
    sp64 = 0x10000;
    xy_rotate(ang, &sp68, &sp64, &sp60, &sp56);
    taiiku_tama_ppush.x = sp60;
    taiiku_tama_ppush.y = sp56;
    taiiku_tama_push_tick = 0;
    goto end;

tick:
    if (taiiku_tama_push_tick < 11U) {
        goto end;
    }
    taiiku_tama_push_tick = 0;
    if (taiiku_tama_gpush_1 > 0) {
        goto reset;
    }
    if (taiiku_tama_ghold != 0) {
        goto reset;
    }
    taiiku_tama_ppush.y = taiiku_tama_ppush.y - 16384;
    if (taiiku_tama_vel > 0) {
        taiiku_tama_ppush.x = taiiku_tama_ppush.x - 16384;
        goto end;
    }
    if (taiiku_tama_vel < 0) {
        taiiku_tama_ppush.x = taiiku_tama_ppush.x + 16384;
    }
    goto end;

reset:
    taiiku_tama_ppush.x = 0;
    taiiku_tama_ppush.y = 0;

end:
    if (taiiku_tama_fall == 0) {
        taiiku_tama_push.x = taiiku_tama_ppush.x + taiiku_tama_gpush;
        taiiku_tama_push.y = taiiku_tama_ppush.y + taiiku_tama_gpush_1;
    }
}
