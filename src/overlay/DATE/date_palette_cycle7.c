/* @SymbolStart: 0x80140CC4 */
/* @SymbolSize: 124 */
/* @CompileEnv: IDO52_AS1_NO_FALLTHRU_FILL=1 */
/* @PostProc: swap_instructions --offset1 0x68 --offset2 0x6c */
/* Address: 0x80140CD4, Size: 108 bytes (27 instructions) */
/* @PostProc: patch_insn_reg_field 0x54:rt:v1:t8 0x58:rs:v1:t8 0x60:rs:v1:t8 */
#include "types.h"

extern void palette_upload_enqueue(s32, s32, s32, s32, s32);
extern u32 frame_counter;
extern volatile u8 date_aug_pal_ptn;
extern s32 date_aug_res_pal;

void date_palette_cycle7(void)
{
    s32 x;
    u32 t;

    if (frame_counter % 7U != 0) {
        return;
    }
    x = date_aug_pal_ptn + 1;
    t = x & 0xFF;
    x = t;
    if ((u32)t >= 4U) {
        x = 0;
    }
    date_aug_pal_ptn = (u8)x;
    palette_upload_enqueue(date_aug_res_pal, 17, 1, 3, x);
}
