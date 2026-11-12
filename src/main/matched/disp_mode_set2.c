/* Address: 0x80059558 */
/* Size: 304 bytes (76 instructions) */
/* Region: Game (0x80041000-0x80086000) */
/* Compiler: IDO 5.2 */
/* @CompileEnv: IDO52_CONST_INREG_VALUES=512,320,240 IDO52_AS1_XBB_VOLATILE_OK=0 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions --offset1 0x60 --offset2 0x64 */
/* @PostProc: swap_instructions --offset1 0x78 --offset2 0x7c */
/* @PostProc: swap_instructions --offset1 0xd4 --offset2 0xd8 */
/* @PostProc: swap_instructions --offset1 0xec --offset2 0xf0 */
/* @PostProc: patch_insn_reg_field 0x28:rt:v1:v0 0x2c:rt:t6:a0 0x30:rs:v1:v0 0x38:rs:t6:a0 0x38:rt:t6:a0 0x3c:rt:a0:at 0x40:rt:t6:at 0x48:rt:t7:t6 0x50:rt:t7:t6 0x58:rt:t8:t7 0x5c:rt:t8:t7 0x60:rt:t9:a0 0x68:rt:t9:a0 0xa4:rt:v0:a0 0xa8:rs:v0:a0 0xa8:rt:v0:a0 0xac:rs:v1:v0 0xb0:rt:a0:at 0xb4:rs:v0:a0 0xb4:rt:a0:at 0xbc:rt:t0:t8 0xc4:rt:t0:t8 0xcc:rt:t1:t9 0xd0:rt:t1:t9 0xd4:rt:t2:a0 0xdc:rt:t2:a0 */
#include "types.h"
extern volatile u16 screen_w;
extern s16 screen_h;
extern s16 screen_intmode;
extern s16 disp_ofs_x[];
extern void frame_buf_clear(void);
extern void DrawSync(s32 a);
extern void ResetGraph(s32 a);
extern void VSync(s32 a);
extern void SetDispMask(s32 a);
extern void GsInitGraph(s32 w, s32 h, s32 c, s32 d, s32 e);
extern void gs_env_init(void);
extern void back_clear_switch(s32 a);
void disp_mode_set2(s32 arg0)
{
    s32 w;
    frame_buf_clear();
    DrawSync(0);
    ResetGraph(1);
    SetDispMask(0);
    if (arg0 != 0 && screen_w != 512) {
        disp_ofs_x[0] = 256;
        disp_ofs_x[1] = 256;
        screen_w = 512;
        screen_h = 240;
        screen_intmode = 0;
        GsInitGraph(512, 240, 0, 0, 0);
        gs_env_init();
        back_clear_switch(1);
    } else {
        w = screen_w;
        if (arg0 == 0 && w != 320) {
            disp_ofs_x[0] = 160;
            disp_ofs_x[1] = 160;
            screen_w = 320;
            screen_h = 240;
            screen_intmode = 0;
            GsInitGraph(320, 240, 0, 0, 0);
            gs_env_init();
            back_clear_switch(1);
        }
    }
    VSync(0);
    SetDispMask(1);
}
