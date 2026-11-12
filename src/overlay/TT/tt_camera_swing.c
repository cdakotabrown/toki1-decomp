/* tt_camera_swing - TT overlay, Address: 0x80149BA0, Size: 436 bytes (109 insns) */
/* 1. `@CompileEnv: IDO52_FORCE_WEB_COLOR_NOSPLIT=15` (Hook 37) -- THE wall. */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR_NOSPLIT=15 */
/* @PostProc: swap_instructions --offset1 0x140 --offset2 0x154 */
/* @PostProc: patch_insn_reg_field 0x100:rt:t4:v1 0x10c:rs:t4:v1 0x110:rs:t4:v1 0x114:rs:t4:v1 0x154:rt:t2:t5 0x17c:rt:a2:t0 0x184:rs:a2:t0 0x184:rt:a2:t2 0x188:rs:a2:t2 0x188:rt:t0:t3 0x18c:rs:t0:t3 0x194:rt:a2:t2 0x198:rt:t3:t6 0x1a0:rs:t3:t6 0x1a0:rt:t2:t5 */
#include "types.h"

extern s32 rsin(s32 angle);
extern s32 rcos(s32 angle);
extern void tt_view_reset(void);

extern u8 *tt_step_wk;
extern u8 *tt_scroll_wk;
extern u8 *tt_anim_buf;
extern u8 *tt_scale_buf;

void tt_camera_swing(void)
{
    u8 *hdr;
    u8 *cam;
    u8 *ctl;
    u8 *pool;
    u8 *rec;
    u16 state;
    s32 ang;
    u32 next;
    s32 wide;
    s32 far;

    hdr = tt_step_wk;
    cam = tt_scale_buf + 16;
    state = *(u16 *)(hdr + 4);
    ctl = tt_scroll_wk;

    switch (state) {
    case 64:
        ang = *(u16 *)(ctl + 18);

        *(s16 *)(cam + 4) = -128 - (rcos(ang) / 32);

        wide = 4096 - (rsin(ang) / 2);
        *(s16 *)(cam + 2) = wide;
        *(s16 *)(cam + 0) = wide;

        *(s16 *)(cam + 20) = (rcos(ang) / 32) + 128;

        far = (rsin(ang) / 2) + 4096;
        *(s16 *)(cam + 18) = far;
        *(s16 *)(cam + 16) = far;

        pool = tt_anim_buf;
        rec = pool + 4300;

        *(s32 *)(pool + 4032) = (*(s16 *)(cam + 20) - 108) << 16;
        *(s16 *)(rec - 280) = *(s16 *)(pool + 4034);

        *(s32 *)(rec - 168) = (*(s16 *)(cam + 20) - 24) << 16;
        *(s16 *)(rec - 180) = *(s16 *)(rec - 166);

        *(s32 *)(rec - 68) = (*(s16 *)(cam + 4) - 96) << 16;
        *(s16 *)(rec - 80) = *(s16 *)(rec - 66);

        *(s32 *)(rec + 32) = (*(s16 *)(cam + 4) + 32) << 16;
        *(s16 *)(rec + 20) = *(s16 *)(rec + 34);

        next = *(u16 *)(ctl + 18) + 24;
        *(u16 *)(ctl + 18) = (u16)next;
        if ((next & 0xFFFF) >= 2048U) {
            *(u16 *)(hdr + 4) = 160;
        }
        break;

    case 160:
        tt_view_reset();
        break;
    }
}
