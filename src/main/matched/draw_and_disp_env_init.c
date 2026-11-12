/* Address: 0x800410AC */
/* Size: 520 bytes (130 instructions) */
/* Region: Game (IDO 5.2) */
/* @CompileFlags: -Wc,-pic1 -cckr */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    s16 x;
    s16 y;
    s16 w;
    s16 h;
} Rect8;

typedef struct {
    u32 tag;
    u32 code[15];
} DR_ENV;

typedef struct {
    Rect8 clip;
    s16 ofs[2];
    Rect8 tw;
    u16 tpage;
    u8 dtd;
    u8 dfe;
    u8 isbg;
    u8 r0;
    u8 g0;
    u8 b0;
    DR_ENV dr_env;
} DRAWENV;

typedef struct {
    Rect8 disp;
    Rect8 screen;
    u8 isinter;
    u8 isrgb24;
    u8 pad0;
    u8 pad1;
} DISPENV;

void draw_and_disp_env_init()
{
    s32 i;
    DRAWENV draw;
    DISPENV disp;

    GetDrawEnv(&draw);
    print_dr_env(draw);
    draw.clip.x = 0;
    draw.clip.y = 0;
    draw.clip.w = 320;
    draw.clip.h = 240;
    PutDrawEnv(&draw);
    print_dr_env(draw);
    GetDispEnv(&disp);
    print_di_env(disp);
    disp.disp.x = 1;
    disp.disp.y = 1;
    disp.disp.w = 320;
    disp.disp.h = 240;
    PutDispEnv(&disp);
    print_di_env(disp);
}
