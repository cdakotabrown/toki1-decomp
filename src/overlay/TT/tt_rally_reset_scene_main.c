/* @SymbolSize: 536 */
/* @CompileFlags: -cckr */
/* @PostProc: swap_instructions --offset1 0x88 --offset2 0xB8 */
/* @PostProc: swap_instructions --offset1 0xB8 --offset2 0xBC */
/* @PostProc: patch_insn_reg_field 0x2c:rt:a0:a1 0x30:rt:a0:a1 0x4c:rt:a0:a1 0x58:rt:v1:a2 0x64:rt:v1:v0 0x6c:rs:v1:v0 0x78:rt:v1:v0 0x7c:rs:v1:v0 0x84:rs:v1:v0 0x84:rt:a0:a1 0x90:rs:v1:v0 0x9c:rs:v1:v0 0xa8:rs:v1:v0 0x1fc:rt:a0:a1 0x200:rs:v1:a2 0x214:rt:a0:a1 */
#include "types.h"

typedef struct {
    u8  pad0[4];
    u16 h4;
    u16 h6;
    u16 h8;
} Pad;

typedef struct {
    u8  pad00[16];
    Pad pad;
    u8  pad1A[56];
    u8  b82;
    u8  b83;
} TTMain;

typedef struct {
    u16 h0;
    u16 h2;
    u16 h4;
    u16 h6;
    u8  pad8;
    u8  b9;
    u8  b10;
    u8  b11;
    u8  padC[2];
    u8  b14;
    u8  padF[2];
    u8  b17;
} TTState;

extern TTMain  *tt_game_wk;
extern TTState *tt_step_wk;

void tt_rally_reset_scene_main()
{
    TTMain *g;
    TTState *h;
    Pad *pa;

    g = tt_game_wk;
    pa = &g->pad;
    h = tt_step_wk;
    h->h0 = 1;
    h->b9 = 0;
    h->b10 = 0;
    h->b11 = 0;
    h->h2 = 0;
    h->h4 = 0;
    h->h6 = 0;
    h->b14 = 1;
    h->b17 = 0;
    if (g->b83 == 0) {
        if (g->b82 == 255) {
            g->b82 = 0;
        }
        if (g->b82 != 0) {
            switch (g->b82) {
            case 1:
                tt_cd_read(0x80016000, 0xC820, 9248);
                tt_cd_read(0x8002E800, 0xC7CF, 27108);
                tt_cd_read(0x8019A000, 0xC7D3, 0x27D60);
                break;
            case 2:
                tt_cd_read(0x80016000, 0xC877, 9248);
                tt_cd_read(0x8002E800, 0xC825, 27108);
                tt_cd_read(0x8019A000, 0xC828, 0x27D60);
                break;
            case 3:
                tt_cd_read(0x80016000, 0xC8CB, 9248);
                tt_cd_read(0x8002E800, 0xC87B, 27108);
                tt_cd_read(0x8019A000, 0xC87F, 0x27D60);
                break;
            case 4:
                tt_cd_read(0x80016000, 0xC924, 9248);
                tt_cd_read(0x8002E800, 0xC8CF, 27108);
                tt_cd_read(0x8019A000, 0xC8D4, 0x27D60);
                break;
            }
            tt_cd_read(0x8001A000, 0xBFDF, 3616);
            tt_cd_read(0x801C2000, 0xBFAA, 0x1A680);
            h->h0 = 2;
        } else {
            h->h0 = 1;
        }
    } else if (pa->h4 & 0xF0) {
        h->h0 = 1;
    }
}
