/* tt_cell1_set - TT overlay, Address: 0x8013AACC, Size: 132 bytes (33 insns) */
#include "types.h"

extern u8 *tt_boss_wk;
extern u8 tt_cell1_kind_tbl[];
extern u8 tt_cell1_frame_tbl[];
extern s16 tt_cell1_ofs_tbl[];
extern s16 tt_cell1_ofs_tbl_1_0[];
extern u8 tt_cell1_sel_tbl[];

void tt_cell1_set(void)
{
    u8 *p;
    u16 i;
    s32 m;

    p = tt_boss_wk;
    i = *(u16 *)(p + 934);
    m = i % 3;
    p[100] = tt_cell1_kind_tbl[i];
    p[102] = tt_cell1_frame_tbl[i];
    *(s16 *)(p + 588) = tt_cell1_ofs_tbl[m];
    *(s16 *)(p + 636) = tt_cell1_ofs_tbl_1_0[m];
    p[120] = 0;
    p[122] = tt_cell1_sel_tbl[m];
    *(s16 *)(p + 590) = -21;
    *(s16 *)(p + 638) = 9;
}
