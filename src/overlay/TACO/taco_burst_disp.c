/* @CompileFlags: -cckr */
/* taco_burst_disp - TACO overlay, Size: 152 bytes (38 insns). */
#include "types.h"

extern s32 activeBuff;
extern u8 ot_buff0[];
extern s32 prim_alloc();
extern void safe_env();
extern void SetDrawMode();
extern s32 GetTPage();
extern void AddPrim();
extern void taco_fan_disp();

void taco_burst_disp(x, mode, rgb)
s32 x;
s32 mode;
s32 rgb;
{
    s32 buf[40];
    s32 unused;
    s32 handle;

    taco_fan_disp(buf, &unused, rgb, x);
    handle = prim_alloc(12, activeBuff);
    SetDrawMode(handle, 0, 1, GetTPage(1, mode, 0, 0), 0);
    AddPrim(ot_buff0 + (activeBuff << 10) + 16, handle);
    safe_env(4);
}
