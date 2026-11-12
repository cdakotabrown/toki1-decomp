/* Address: 0x8005944C */
/* Size: 268 bytes (67 instructions) */
/* Region: Game (0x80041000-0x80086000) */
/* Compiler: IDO 5.2 */
/* `@CompileEnv: IDO52_CONST_INREG_VALUES=256,240,320` is load-bearing and is */
/* @CompileEnv: IDO52_CONST_INREG_VALUES=256,240,320 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions --offset1 0x70 --offset2 0x74 */
/* @PostProc: swap_instructions --offset1 0xc8 --offset2 0xcc */
#include "types.h"

extern s16 screen_w;
extern s16 screen_h;
extern s16 screen_intmode;
extern s16 disp_ofs_x[];

extern void DrawSync(s32 a);
extern void ResetGraph(s32 a);
extern void VSync(s32 a);
extern void SetDispMask(s32 a);
extern void GsInitGraph(s32 w, s32 h, s32 c, s32 d, s32 e);
extern void gs_env_init(void);
extern void back_clear_switch(s32 a);

void disp_mode_set(s32 arg0)
{
    DrawSync(0);
    ResetGraph(1);
    VSync(0);
    SetDispMask(0);
    if (arg0 != 0) {
        disp_ofs_x[0] = 128;
        disp_ofs_x[1] = 128;
        screen_w = 256;
        screen_h = 240;
        screen_intmode = 0;
        GsInitGraph(256, 240, 0, 0, 0);
        gs_env_init();
        back_clear_switch(1);
    } else {
        disp_ofs_x[0] = 160;
        disp_ofs_x[1] = 160;
        screen_w = 320;
        screen_h = 240;
        screen_intmode = 0;
        GsInitGraph(320, 240, 0, 0, 0);
        gs_env_init();
        back_clear_switch(1);
    }
    VSync(0);
    SetDispMask(1);
}
