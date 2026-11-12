/* Address: 0x80098E44 */
/* Size: 340 bytes (85 instructions) */
/* Region: Library */
/* @Compiler: gcc260 */
/* @CompileFlags: -O1 -fschedule-insns -fschedule-insns2 --fill-epilogue */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions 0x4 0x14 */
/* @PostProc: swap_instructions 0x8 0x1c */
/* @PostProc: swap_instructions 0xc 0x14 */
/* @PostProc: swap_instructions 0x10 0x28 */
/* @PostProc: swap_instructions 0x14 0x18 */
/* @PostProc: swap_instructions 0x1c 0x20 */
/* @PostProc: swap_instructions 0x20 0x28 */
/* @PostProc: swap_instructions 0x28 0x2c */
/* @PostProc: swap_instructions 0x3c 0x44 */
/* @PostProc: swap_instructions 0x40 0x48 */
#include "types.h"

extern u16 libgs_drawenv_ofs_x;
extern u16 libgs_drawenv_ofs_y;
extern u16 libgs_drawenv_tw_x;
extern u16 libgs_drawenv_tw_y;
extern u16 libgs_drawenv_tw_w;
extern u16 libgs_drawenv_tw_h;
extern u16 libgs_drawenv_tpage;
extern u8  libgs_drawenv_dither;
extern u8  libgs_drawenv_dfe;
extern u8  libgs_drawenv_isbg;
extern u16 GsDISPENV;
extern u16 libgs_dispenv_disp_y;
extern u16 libgs_dispenv_disp_w;
extern u16 libgs_dispenv_disp_h;
extern u16 libgs_dispenv_screen_x;
extern u16 libgs_dispenv_screen_y;
extern u16 libgs_dispenv_screen_w;
extern u16 libgs_dispenv_screen_h;
extern u8  libgs_dispenv_isinter;
extern u8  libgs_dispenv_isrgb24;
extern u16 PSDGPU;
extern u8  libgs_dispenv_pal_flag;

extern void ResetGraph(s32 arg0);
extern s32  PutDrawEnv(void *arg0);
extern s32  GetVideoMode(void);
extern void PutDispEnv(void *arg0);

void gpu_init(u16 arg0, u16 arg1, u32 arg2, u32 arg3, u16 arg4) {
    u16 *p;
    u16 *q;
    u32 x;

    ResetGraph(0);
    libgs_drawenv_ofs_y = 0;
    p = &libgs_drawenv_ofs_x;
    *p = 0;
    libgs_drawenv_tw_h = 0;
    libgs_drawenv_tw_w = 0;
    libgs_drawenv_tw_y = 0;
    libgs_drawenv_tw_x = 0;
    libgs_drawenv_tpage = 0;
    libgs_drawenv_dither = arg3;
    libgs_drawenv_dfe = 0;
    libgs_drawenv_isbg = 0;
    PutDrawEnv((u8 *)p - 8);
    q = &GsDISPENV;
    *q = 0;
    libgs_dispenv_disp_y = 0;
    libgs_dispenv_disp_w = arg0;
    libgs_dispenv_disp_h = arg1;
    libgs_dispenv_screen_x = 0;
    libgs_dispenv_screen_y = 0;
    libgs_dispenv_screen_w = 0;
    libgs_dispenv_screen_h = 0;
    x = arg2;
    if (GetVideoMode() == 1) {
        libgs_dispenv_screen_y = 24;
        libgs_dispenv_pal_flag = 1;
    }
    libgs_dispenv_isinter = x & 1;
    PSDGPU = arg2 & 4;
    libgs_dispenv_isrgb24 = arg4;
    PutDispEnv(q);
}
