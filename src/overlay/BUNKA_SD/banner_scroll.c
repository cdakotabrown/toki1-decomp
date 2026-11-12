/* @RodataBase: 0x80139F9C */
/* @SymbolSize: 220 */
/* @CompileFlags: -Wo,-nordstore */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:3 */
/* @PostProc: swap_instructions --offset1 0xe0 --offset2 0xe8 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v1:v0 0x1c:rs:v1:v0 0x30:rd:a0:v1 0x34:rs:a0:v1 0x44:rd:a0:v1 0x4c:rd:a1:a0 0x4c:rt:a0:v1 0x78:rt:a1:a0 0x8c:rt:a1:a0 0xa0:rt:a1:a0 0xb4:rt:a1:a0 0xc8:rt:a1:a0 0xd8:rt:v1:t8 0xe0:rs:v1:t8 0xe8:rt:v1:t8 */
/* banner_scroll - BUNKA_SD overlay, Address: 0x80132708, Size: 220 bytes (55 insns) */
#include "types.h"
#include "tokimeki/class_w.h"

extern u8  scene_entry_c;
extern s16 cw98_frame;
extern s32 bunka_sd_kind;
extern CLASS_W class_w[];

s32 banner_scroll(void)
{
    u32 tick;
    u32 phase;
    s32 step;
    u32 next;

    class_w[98].anim_flag = 64;

    tick = scene_entry_c;
    phase = tick / 14;
    if (phase >= 2) {
        tick = 0;
        phase = 0;
    }

    step = phase * 2;

    switch ((u32)bunka_sd_kind) {
    case 0:
        cw98_frame = (s16)(25 - step);
        break;
    case 1:
        cw98_frame = (s16)(69 - step);
        break;
    case 2:
        cw98_frame = (s16)(113 - step);
        break;
    case 3:
        cw98_frame = (s16)(157 - step);
        break;
    case 4:
    default:
        cw98_frame = (s16)(189 - step);
        break;
    }

    tick = tick + 1;
    next = tick & 0xFF;
    scene_entry_c = (u8)next;
    return (s32)next;
}
