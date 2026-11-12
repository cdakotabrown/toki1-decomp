/* Function: disp_mode_set2 (obsolete painted source; not the tilemap VRAM uploader) */
/* Address: 0x80059558 */
/* Size: 304 bytes (76 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: custom_script custom_srn_vram_set.py */
#include "types.h"

extern void frame_buf_clear(void);
extern void DrawSync(s32 arg0);
extern void ResetGraph(s32 arg0);
extern void SetDispMask(s32 arg0);
extern void GsInitGraph(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
extern void gs_env_init(void);
extern void func_80043AFC(s32 arg0);
extern void VSync(s32 arg0);

extern u16 screen_w;
extern s16 screen_h;
extern s16 screen_intmode;
extern s16 disp_ofs_x;
extern s16 D_800EE776;

void disp_mode_set2(s32 arg0) {
    frame_buf_clear();
    DrawSync(0);
    ResetGraph(1);
    SetDispMask(0);
    if (arg0 != 0 && screen_w != 512) {
        disp_ofs_x = 256;
        D_800EE776 = 256;
        screen_w = 512;
        screen_h = 240;
        screen_intmode = 0;
        GsInitGraph(512, 240, 0, 0, 0);
        gs_env_init();
        func_80043AFC(1);
    } else if (arg0 == 0 && screen_w != 320) {
        disp_ofs_x = 160;
        D_800EE776 = 160;
        screen_w = 320;
        screen_h = 240;
        screen_intmode = 0;
        GsInitGraph(320, 240, 0, 0, 0);
        gs_env_init();
        func_80043AFC(1);
    }
    VSync(0);
    SetDispMask(1);
}
