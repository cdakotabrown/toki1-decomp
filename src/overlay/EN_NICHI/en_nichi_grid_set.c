/* @PostProc: patch_stack_complete --old 392 --new 400 */
/* @PostProc: remap_stack_slots --map 136:132 */
/* en_nichi_grid_set - EN_NICHI overlay, Address: 0x8013577C, Size: 976 bytes */
#include "types.h"

typedef struct {
    s32 v[8][8];
} WaveTbl;

extern WaveTbl en_nichi_grid_set_tbl;
extern s16 en_nichi_grid_buf[][12][4];
extern volatile s32 en_nichi_sway_cnt;

void en_nichi_grid_set(void)
{
    WaveTbl buf;
    s32 t;
    s32 i;
    s32 j;
    s32 c5;
    s32 c7;

    t = en_nichi_sway_cnt;
    buf = en_nichi_grid_set_tbl;
    c5 = (t / 5) % 8;
    c7 = (t / 7) % 8;
    for (i = 0; i < 12; i++) {
        for (j = 0; j < 15; j++) {
            en_nichi_grid_buf[j][i][1] = (s16)buf.v[(i + 3 * j) % 8][c5];
            en_nichi_grid_buf[j][i][3] = (s16)buf.v[(3 * i + j) % 8][c7];
        }
    }
}
