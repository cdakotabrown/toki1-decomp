/* Function: date_bg_strip_disp */
/* Overlay: DATE */
/* Address: 0x80152684 */
/* Size: 708 bytes (177 insns) */
/* @PostProc: remap_stack_slots --from 80 --to 76 --from 84 --to 80 */
#include "types.h"

typedef struct {
    s32 unk00;
    s32 unk04;
} TexEnt;

typedef struct {
    TexEnt e[4];
} TexTbl;

extern TexTbl date_bg_strip_disp_tbl;
extern u8     date_bg_ptn[];

extern void date_tri_disp(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

void date_bg_strip_disp(s16 a0, s16 a1, s16 a2)
{
    s32 x;
    TexTbl local;

    local = date_bg_strip_disp_tbl;
    x = a0;

    if (x < 320) {
        date_tri_disp(320, -280, 160 - x, 40,
                      (160 - x) + (s16)(x / 32 + 10), 40,
                      local.e[date_bg_ptn[a1]].unk00,
                      local.e[date_bg_ptn[a1]].unk04,
                      a2, 1);
    } else if (x < 340) {
        date_tri_disp(320, -280, -160,
                      40 - (s16)((x * 7 - 2240) / 8),
                      -160, 40,
                      local.e[date_bg_ptn[a1]].unk00,
                      local.e[date_bg_ptn[a1]].unk04,
                      a2, 1);
        date_tri_disp(320, -280, -160, 40,
                      ((s16)(x / 32 + 10) - x) + 160, 40,
                      local.e[date_bg_ptn[a1]].unk00,
                      local.e[date_bg_ptn[a1]].unk04,
                      a2, 1);
    } else {
        date_tri_disp(320, -280, -160,
                      40 - (s16)((x * 7 - 2240) / 8),
                      -160,
                      (40 - (s16)((x * 7 - 2240) / 8)) +
                          (s16)(15 - (x - 320) / 32),
                      local.e[date_bg_ptn[a1]].unk00,
                      local.e[date_bg_ptn[a1]].unk04,
                      a2, 1);
    }
}
