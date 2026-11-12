/* @PostProc: patch_stack_complete --old 96 --new 104 */
/* @PostProc: remap_stack_slots --from 92 --to 100 --from 64 --to 80 --from 52 --to 68 --from 56 --to 72 --from 60 --to 76 */
/* @SymbolStart: 0x8013A550 */
/* @SymbolSize: 484 */
/* Address: 0x8013A550 (cross-prologue head) / 0x8013A558 (prologue) */
/* Size: 484 bytes (121 insns incl. the 2-insn head) */
#include "types.h"

extern s32 taco_name_row;
extern u8 taco_rank_name[][4];

extern void taco_glyph_disp(s32, s32, s32, s32, s32, s32, s32, s32, s32);

void taco_name_disp(s32 in)
{
    s32 mode;
    s32 den;
    s32 q1;
    s32 q2;
    s32 rnd;
    s32 dy;

    switch (taco_name_row) {
    case 0:
        mode = 1;
        break;
    case 1:
        mode = 4;
        break;
    case 2:
        mode = 3;
        break;
    }

    den = 480;
    q1 = (in * 128) / den;
    q2 = (in * 28672) / den;
    rnd = (q2 * 12) / 4096;
    dy = -40 - (in / 16);

    taco_glyph_disp(taco_rank_name[taco_name_row][0], mode, 3, q1, -rnd, dy, q2,
                  q2, 0);
    taco_glyph_disp(taco_rank_name[taco_name_row][1], mode, 3, q1, 0, dy, q2, q2, 0);
    taco_glyph_disp(taco_rank_name[taco_name_row][2], mode, 3, q1, rnd, dy, q2, q2,
                  0);
}
