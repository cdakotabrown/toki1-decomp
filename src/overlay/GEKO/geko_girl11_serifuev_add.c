/* @SymbolStart: 0x80140F40 */
/* @SymbolSize: 172 */
/* @PostProc: swap_instructions --offset1 0x70 --offset2 0x74 */
/* @PostProc: swap_instructions --offset1 0x84 --offset2 0x88 */
/* @PostProc: swap_instructions --offset1 0x80 --offset2 0x84 */
/* @PostProc: patch_insn_reg_field 0x10:rt:a0:v0 0x14:rs:a0:v0 0x14:rt:a0:v0 0x1c:rt:a0:v0 0x20:rd:a0:v0 0x24:rs:a0:v0 0x3c:rt:a0:v0 0x4c:rs:a0:v0 0x4c:rt:a0:v0 0x54:rt:a0:v0 0x58:rd:a0:v0 0x60:rt:v0:t6 0x68:rs:v0:t6 0x68:rt:v0:t6 0x6c:rt:a0:v0 0x7c:rd:t6:t7 0x7c:rt:v0:t6 0x88:rd:t7:t8 0x88:rt:t6:t7 0x8c:rt:t9:t2 0x90:rs:t7:t8 0x90:rt:v0:t9 0x94:rd:t8:t0 0x94:rt:v0:t9 0x98:rs:t8:t0 0x9c:rs:t9:t2 0x9c:rt:t0:t3 0xa0:rd:t2:t4 0xa0:rs:t0:t1 0xa0:rt:t1:t3 0xa8:rt:t2:t4 */
/* geko_girl11_serifuev_add -- GEKO overlay, Address: 0x80140F48 (true start 0x80140F40) */
/* Size: 172 bytes (43 insns) including the absorbed 2-word head. */
#include "types.h"

extern void sub_sub_step_up(void);
extern u8 screen_w[];
extern volatile s32 girl11_flag;
extern u16 now_serifuev;

void geko_girl11_serifuev_add(void)
{
    u8 *base;
    u32 field;
    u32 t;

    base = (u8 *)((u32)screen_w | 0);
    field = ((u32)girl11_flag << 20) >> 29;
    if (field >= 5) {
        base[1073] = (base[1073] & 0xFFF1) | 0x0008;
        field = ((u32)girl11_flag << 20) >> 29;
    }
    now_serifuev = now_serifuev + field * 2;
    t = ((u32)girl11_flag << 20) >> 29;
    t = t + 1;
    t = t * 2;
    base[1073] = (t & 0x000E) | (base[1073] & 0xFFF1);
    sub_sub_step_up();
}
