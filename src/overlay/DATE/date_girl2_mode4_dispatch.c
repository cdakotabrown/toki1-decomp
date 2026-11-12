/* Address: 0x801511A8, Size: 332 bytes (83 insns) */
/* @PostProc: patch_stack_complete --old 192 --new 208 */
/* @PostProc: remap_stack_slots --from 32 --to 44 */
/* @PostProc: patch_insn_reg_field 0xb8:rt:v0:t2 0xbc:rt:t2:v1 0x100:rs:v0:v1 */
#include "types.h"
#include "tokimeki/sprite.h"
#include "tokimeki/class_w.h"

extern void date_light_effect(void);

typedef struct {
    void (*f[40])(s32);
} FnTable;

extern FnTable date_girl2_mode4_dispatch_tbl;
extern u8 now_sub_sub_step;
extern s32 date_sprite_on;
extern s32 frame_counter;
extern u8 fade_bri;
extern CLASS_W class_w[];
extern SPRITE_REC Sprites[];

void date_girl2_mode4_dispatch(void)
{
    FnTable local;
    u32 rem;

    local = date_girl2_mode4_dispatch_tbl;
    local.f[now_sub_sub_step](128);

    if (date_sprite_on != 0) {
        class_w[98].disp_flag = class_w[98].disp_flag | 0x80;
        rem = (u32)frame_counter % 40U;
        if (rem < 20U) {
            (*(s32 *)&Sprites[57].attribute) = (*(s32 *)&Sprites[57].attribute) & 0x7FFFFFFF;
        } else {
            (*(s32 *)&Sprites[57].attribute) = (*(s32 *)&Sprites[57].attribute) | 0x80000000;
        }
        Sprites[57].b = fade_bri;
        Sprites[57].g = fade_bri;
        Sprites[57].r = fade_bri;
        class_w[98].bright = fade_bri;
    } else {
        class_w[98].disp_flag = class_w[98].disp_flag & 0xFF7F;
        (*(s32 *)&Sprites[57].attribute) = (*(s32 *)&Sprites[57].attribute) | 0x80000000;
    }
}
