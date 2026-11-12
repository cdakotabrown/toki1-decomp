/* @CompileFlags: -cckr */
/* @SymbolSize: 296 */
#include "types.h"

extern void bg_set();
extern void palette_upload_enqueue();
extern void clut_gray_set();

extern u8 cursor_work_bak[];
extern u8 bg_pri;
extern u32 Bg;

void name_ent_bg_set(base, param2)
    u32 base;
    int param2;
{
    int i;
    u32 arr[6];
    int unused;

    for (i = 0; i < 6; i++) arr[i] = base + i * 0x4000;
    bg_set(arr, 0, 6, 3, 2, param2 + 480, -160, -120, 8, 8);
    palette_upload_enqueue(base + 0x18000, 15, 1, 2, 0);
    clut_gray_set(base + 0x18000, cursor_work_bak);
    palette_upload_enqueue(cursor_work_bak, 14, 1, 2, 0);
    bg_pri = 13;
    Bg = 0x41000000;
}
