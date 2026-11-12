/* Address: 0x800A2BD0 */
/* Size: 68 bytes (17 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 -- libgpu, recompiled (not Sony's object) */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v1:0 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

void AddPrim(u32 *ofs, u32 *p) {
    u32 y1 = *p;
    u32 y2;

    *p = ((((*ofs << 8) >> 8) ^ y1) << 8 >> 8) ^ y1;
    y2 = *ofs;
    *ofs = ((((u32)p ^ y2) << 8) >> 8) ^ y2;
}
