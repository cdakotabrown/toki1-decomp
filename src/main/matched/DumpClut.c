/* Address: 0x800A2B5C */
/* Size: 60 bytes (15 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 -O2 (pic0) */
/* @SymbolStart: 0x800A2B5C */
/* @SymbolSize: 60 */
/* @CompileEnv: IDO52_UGEN_DEST_TEMP=3,6 IDO52_UGEN_TEMP_REG=1:t7,2:t6 IDO52_PRECOLOR_WEB=v1:2 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: patch_insn_reg_field --patch 0x14:rt:t7:a1 --patch 0x20:rt:t7:a1 */
#include "types.h"

extern void (*GPU_printf)(void *table, s32 slot, s32 group);
extern u8 libgpu_dumpclut_fmt[];

void DumpClut(u16 idx)
{
    u32 v;

    v = idx;
    GPU_printf(libgpu_dumpclut_fmt, (v & 0x3F) << 4, v >> 6);
}
