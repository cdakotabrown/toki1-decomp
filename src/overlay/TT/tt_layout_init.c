/* @CompileFlags: -Wo,-loopunroll,0 */
/* @PostProc: swap_instructions --offset1 0x1f0 --offset2 0x1f4 */
/* @PostProc: swap_instructions --offset1 0x1f4 --offset2 0x1f8 */
/* @PostProc: patch_insn_reg_field 0x24:rt:a1:a0 0x2c:rs:a1:a0 0x2c:rt:a1:a0 0x58:rt:a0:v1 0x7c:rt:a0:v1 0xa4:rt:a0:v1 0xc0:rt:a0:v1 0xc8:rs:a1:a0 0x19c:rt:t7:t8 0x1a0:rd:t6:t9 0x1a0:rt:t7:t8 0x1a4:rt:t8:t2 0x1a8:rt:t9:t3 0x1b4:rt:t6:t9 0x1c8:rt:t8:t2 0x1d0:rt:t9:t3 0x1d4:rd:t0:t7 0x1d8:rs:t0:t7 0x1d8:rt:t1:t6 0x1dc:rt:t1:t6 0x1e4:rd:t2:t0 0x1ec:rd:t2:t0 0x1f8:rd:t3:t1 0x1f8:rt:t2:t0 0x200:rt:t3:t1 */
/* tt_layout_init - TT overlay, Address: 0x80145FF0, Size: 548 bytes (137 insns) */
#include "types.h"

extern s32 rsin(s32 angle);
extern s32 rcos(s32 angle);

extern u8 *tt_scroll_wk;
extern u8 *tt_anim_buf;
extern u8 *tt_text_buf;
extern u8 *tt_scale_buf;

void tt_layout_init(void)
{
    u8 *hud;
    u8 *ctl;
    u8 *cam;
    u8 *icon;
    u8 *r;
    s32 i;
    s32 ang;
    s32 d;

    hud = tt_text_buf + 320;
    ctl = tt_scroll_wk;
    cam = tt_scale_buf;

    *(s16 *)(hud + 10) = -64;
    *(s16 *)(hud + 12) = 72;
    *(s16 *)(hud + 14) = 17;
    hud[2] = 1;
    hud[0] = 1;
    *(s16 *)(hud + 6) = 8;
    *(s16 *)(hud + 16) = 0;

    hud += 20;
    *(s16 *)(hud + 10) = 0;
    *(s16 *)(hud + 12) = 88;
    *(s16 *)(hud + 14) = 38;
    hud[2] = 1;
    hud[0] = 1;
    *(s16 *)(hud + 6) = 8;
    *(s16 *)(hud + 16) = 0;

    icon = tt_anim_buf;
    icon[0] = 1;
    icon[1] = 0;
    *(s16 *)(icon + 68) = 8;
    *(s16 *)(icon + 20) = 52;
    *(s16 *)(icon + 22) = (s16)((*(u16 *)(ctl + 24) << 4) + 126);
    *(u16 *)(icon + 60) = 8;
    *(u16 *)(icon + 62) = 8;
    icon[70] = 48;
    icon[71] = 16;
    *(s16 *)(icon + 64) = 10;
    *(s16 *)(icon + 66) = 15375;
    icon[59] = 101;
    *(s16 *)(icon + 16) = 0;

    r = tt_anim_buf + 6400;
    i = 0;
    ang = 0;
    do {
        r[0] = 1;
        r[1] = 0;
        r[59] = 128;
        *(s32 *)(r + 72) = (s32)cam;
        r[77] = 0;
        r[78] = 1;
        r[56] = 64;
        r[57] = 64;
        r[58] = 64;
        r[80] = 0;
        *(s16 *)(r + 20) = (s16)(*(s16 *)(cam + 4) - 36);
        *(s16 *)(r + 22) = (s16)(*(s16 *)(cam + 6) - 36);
        *(s16 *)(r + 20) = (s16)(rcos(ang) / 128);
        d = rsin(ang) / 48;
        ang += 85;
        r += 100;
        r[-30] = (u8)((i & 0xF) * 8);
        r[-21] = 0;
        *(u16 *)(r - 40) = 8;
        *(u16 *)(r - 38) = 8;
        *(s16 *)(r - 36) = 10;
        *(s16 *)(r - 34) = 15360;
        r[-24] = 0;
        *(s16 *)(r - 84) = 13;
        *(s16 *)(r - 78) = (s16)(d + 120);
        r[-29] = (u8)((i / 16) * 8);
        i++;
    } while (i != 3);
}
