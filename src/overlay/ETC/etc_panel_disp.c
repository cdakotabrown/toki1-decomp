/* @PostProc: scheduler_normalizer --no-forced-reorder */
/* @PostProc: patch_insn_reg_field 0x6c:rs:v1:s6 0x6c:rt:s6:v1 0xfc:rs:v1:fp 0xfc:rt:fp:v1 0x21c:rd:t6:t7 0x220:rd:t5:t6 0x224:rs:t5:t6 0x23c:rt:t6:t7 0x274:rt:t7:t8 0x27c:rd:t8:t9 0x27c:rt:t7:t8 0x284:rt:t8:t9 0x290:rt:t9:t0 0x294:rt:t9:t0 */
/* Address: 0x80148B44 */
/* Size: 720 bytes (180 instructions) */
#include "types.h"

extern void etc_num_disp(s32, s32, s32, s32, s32);

typedef struct {
    s32 flags;
    s32 rest[13];
} PanelEnt;

extern PanelEnt girl0_flag[];
extern s32 girl11_flag;
extern s16 etc_rank_x[];
extern s16 etc_rank_y[];
extern s16 etc_rank_score[];
extern s32 etc_rank_order[];
extern s32 etc_rank_juni[];
extern s32 etc_rank_low;

void etc_panel_disp(void)
{
    s32 i;
    s32 id;

    for (i = 0; i != 14; i++) {
        id = etc_rank_order[i];

        if (13 == id) {
            etc_num_disp(etc_rank_score[13], etc_rank_x[i] + 134,
                          etc_rank_y[i], 1, 0);
            etc_num_disp(0, etc_rank_x[i] + 142, etc_rank_y[i], 0, 0);
            etc_num_disp(etc_rank_juni[13], etc_rank_x[i] + 4,
                          etc_rank_y[i], 0, 1);
            if (i >= 7) {
                etc_rank_low = 1;
            } else {
                etc_rank_low = 0;
            }
        } else if (10 == id) {
            etc_num_disp(etc_rank_score[10], etc_rank_x[i] + 134,
                          etc_rank_y[i], 1, 0);
            etc_num_disp(0, etc_rank_x[i] + 142, etc_rank_y[i], 0, 0);
            etc_num_disp(etc_rank_juni[10], etc_rank_x[i] + 4,
                          etc_rank_y[i], 0, 1);
        } else if (id == 11) {
            if ((((u32)girl11_flag << 20) >> 29) != 0) {
                etc_num_disp(etc_rank_score[11], etc_rank_x[i] + 134,
                              etc_rank_y[i], 1, 0);
                etc_num_disp(0, etc_rank_x[i] + 142, etc_rank_y[i], 0, 0);
                etc_num_disp(etc_rank_juni[11], etc_rank_x[i] + 4,
                              etc_rank_y[i], 0, 1);
            }
        } else {
            if ((girl0_flag[id].flags << 30) < 0) {
                etc_num_disp(etc_rank_score[id], etc_rank_x[i] + 134,
                              etc_rank_y[i], 1, 0);
                etc_num_disp(0, etc_rank_x[i] + 142, etc_rank_y[i], 0, 0);
                etc_num_disp(etc_rank_juni[etc_rank_order[i]], etc_rank_x[i] + 4,
                              etc_rank_y[i], 0, 1);
            }
        }
    }
}
