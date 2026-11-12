/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:0 */
/* @PostProc: swap_instructions --offset1 0x60 --offset2 0x64 */
/* @SymbolSize: 204 */
/* Address: 0x801357C0, size 204 bytes (51 instructions). */
#include "types.h"

extern s32 taco_glyph_disp(s32, s32, s32, s32, s32, s32, s32, s32, s32);

s32 taco_glyph_str_disp(u8 *str, s32 a1, s32 a2, s32 a3, s32 arg5, s32 col,
                  s32 arg7)
{
    u8 *p;
    s32 i;
    s32 x;
    s32 ret;

    ret = 0;
    if (str[0] != 0) {
        p = str;
        i = 0;
        x = col;
        do {
            ret = taco_glyph_disp(*p, a3, 3, 128, x, arg7, a1, a2, arg5);
            i += 13;
            p++;
            x += 13;
            if (i >= 273) {
                break;
            }
        } while (*p != 0);
    }
    return ret;
}
