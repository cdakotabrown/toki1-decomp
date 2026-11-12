/* Function: GsInitGraph2 */
/* Address: 0x80098F98  (the recorded symbol 0x80098FDC is the PROLOGUE; */
/* Size: 120 bytes (30 instructions) -- ends at 0x8009900C `nop`; the next */
/* Region: Library (0x80086000+) */
/* Compiler: IDO 5.2 (the opening store chain is hoisted ABOVE the prologue - */
/* a thing Psy-Q GCC never does; an old "@Compiler: gcc260" annotation was */
/* homes); with `@CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all` (Hook 41b, */
/* @SymbolStart: 0x80098F98 */
/* @SymbolSize: 120 */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
/* @PostProc: swap_instructions --offset1 0x3C --offset2 0x40 */
/* @PostProc: swap_instructions --offset1 0x40 --offset2 0x44 */
/* @PostProc: swap_instructions --offset1 0x50 --offset2 0x54 */
/* @PostProc: swap_instructions --offset1 0x5C --offset2 0x64 */
/* @PostProc: remap_stack_slots --map '20:16' --keep-frame */
/* @PostProc: patch_insn_reg_field 0x34:rt:t6:v0 0x38:rt:t6:v0 0x40:rt:t7:a2 0x48:rt:t8:v1 0x4c:rt:t7:a2 0x5c:rt:t8:v1 */
#include "types.h"

extern u16 libgs_dispenv_disp_w;
extern u16 libgs_dispenv_disp_h;
extern u16 libgs_drawenv_tpage;
extern u8  libgs_drawenv_dither;
extern u8  libgs_drawenv_dfe;
extern u8  libgs_drawenv_isbg;
extern u8  libgs_dispenv_isinter;
extern u8  libgs_dispenv_isrgb24;
extern u16 PSDGPU;

extern void valiable_init(u32 a, u32 b);

void GsInitGraph2(u32 a0, u32 a1, u32 a2, u8 a3, u16 arg4)
{
    libgs_dispenv_disp_w = (u16)a0;
    libgs_dispenv_disp_h = (u16)a1;
    libgs_drawenv_tpage = 0;
    libgs_drawenv_dither = a3;
    libgs_drawenv_dfe = 0;
    libgs_drawenv_isbg = 0;
    libgs_dispenv_isinter = (u8)(a2 & 1);
    PSDGPU = (u16)(a2 & 4);
    libgs_dispenv_isrgb24 = (u8)arg4;
    a0 &= 0xffff;
    a1 &= 0xffff;
    valiable_init(a0, a1);
}
