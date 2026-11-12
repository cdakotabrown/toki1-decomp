/* @CompileEnv: IDO52_FORCE_WEB_COLOR=26:t0 IDO52_PRECOLOR_WEB=v0:20 */
/* Address: 0x8004FAD0 */
/* Size: 436 bytes (109 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions --offset1 0x4 --offset2 0x1c */
/* @PostProc: swap_instructions --offset1 0x8 --offset2 0x20 */
/* @PostProc: swap_instructions --offset1 0xc --offset2 0x2c */
/* @PostProc: swap_instructions --offset1 0x10 --offset2 0x1c */
/* @PostProc: swap_instructions --offset1 0x14 --offset2 0x20 */
/* @PostProc: swap_instructions --offset1 0x18 --offset2 0x2c */
/* @PostProc: swap_instructions --offset1 0x24 --offset2 0x2c */
/* @PostProc: swap_instructions --offset1 0x28 --offset2 0x2c */
/* @CompileFlags: -Wc,-pic1 */
/* `@PostProc: custom_script custom_func_8004FAD0.py`, now retired and deleted. */
/* @PostProc: patch_insn_reg_field 0xd4:rt:a0:v1 0xd8:rt:a1:t0 0xdc:rs:a1:t0 0xdc:rt:t3:t0 0xe0:rs:a0:v1 0xe0:rt:t1:v1 0xe4:rs:t3:t0 0xe8:rs:t1:v1 0x124:rd:t5:t1 0x128:rs:t5:t1 */
#include "types.h"

extern s32 pad_data;
extern s32 pad_trig;
extern s16 adjust_ofs_x;
extern s16 adjust_ofs_y;
extern u16 screen_w;
extern u16 screen_h;
extern u16 screen_intmode;

extern void pad_main(void);
extern void GsSetDrawBuffOffset(void);
extern void GsDefDispBuff(u16 a0, u16 a1, u16 a2, u16 a3);
extern void ot_buff_set(void);
extern void DrawSync(s32);
extern void VSync(s32);
extern void GsSwapDispBuff(void);
extern void GsInitGraph(u16, u16, u16, s32, s32);
extern void GsInit3D(void);

void screen_offset_adjust(void) {
    s16 v1_val;
    s16 t0_val;
    s32 buttons;
    s32 maskUp = 0x10000000;
    s32 maskDown = 0x40000000;
    s32 maskRight = 0x20000000;
    s32 maskLeft = (s32)0x80000000;
    s32 maskStart = 0x08000000;

    pad_trig = 0x80000;
    buttons = pad_data;
    v1_val = 0;
    t0_val = 0;

    do {
        if (buttons & maskUp) {
            t0_val = t0_val - 16;
        }
        if (buttons & maskDown) {
            t0_val = t0_val + 16;
        }
        if (buttons & maskRight) {
            v1_val = v1_val + 16;
        }
        if (buttons & maskLeft) {
            v1_val = v1_val - 16;
        }

        adjust_ofs_x = v1_val;
        if (v1_val < 0) {
            adjust_ofs_x = 0;
        }
        adjust_ofs_y = t0_val;
        if (t0_val < 0) {
            adjust_ofs_y = 0;
        }

        pad_main();
        GsSetDrawBuffOffset();

        {
            u32 a1_val;
            u32 a0_val;
            a1_val = adjust_ofs_y & 0xFFFF;
            a0_val = adjust_ofs_x & 0xFFFF;
            GsDefDispBuff(a0_val, a1_val, a0_val, a1_val);
        }

        ot_buff_set();
        DrawSync(0);
        VSync(0);
        GsSwapDispBuff();

        buttons = pad_data;
        if (buttons & maskStart) {
            GsInitGraph(screen_w, screen_h, screen_intmode, 0, 0);
            GsSetDrawBuffOffset();
            GsDefDispBuff(0, 0, 0, screen_h);
            GsInit3D();
            return;
        }

        t0_val = adjust_ofs_y;
        v1_val = adjust_ofs_x;
    } while (1);
}
