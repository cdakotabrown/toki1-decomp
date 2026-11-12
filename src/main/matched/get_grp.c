/* Address: 0x800537F0 */
/* Size: 276 bytes (69 instructions) */
/* Region: Game (0x80041000-0x80086000) */
/* Compiler: IDO 5.2 */
/* @CompileEnv: IDO52_CONST_NOINREG_VALUES=20 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern u8 get_grp_tbl0[];

extern void get_h_yuukou_table(u8 *p);
extern void get_h_tokimeki_table(u8 *p);
extern void get_grp_point(u8 *out, s16 *clr);
extern void grp_table_calc(u8 *out, s16 *clr);

void get_grp(u8 *out, s16 *clr)
{
    s32 p0;
    s32 p1;
    u8 b[16];
    u8 a[16];
    s32 i;

    get_h_yuukou_table(a);
    get_h_tokimeki_table(b);
    for (i = 0; i < 16; i++) {
        clr[i] = 0;
        a[i] = (u8)((u32)a[i] / 5);
        if (a[i] >= 20U) {
            a[i] = 19;
        }
        b[i] = (u8)((u32)b[i] / 5);
        if (b[i] >= 20U) {
            b[i] = 19;
        }
        out[i] = get_grp_tbl0[a[i] + b[i] * 20];
    }
    get_grp_point(out, clr);
    grp_table_calc(out, clr);
}
