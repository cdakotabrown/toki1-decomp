/* tt_cell0_set - TT overlay, Address: 0x8013AA44, Size: 136 bytes (34 insns) */
#include "types.h"

extern u8 *tt_boss_wk;
extern u8 tt_cell0_kind_tbl[];
extern u8 tt_cell0_frame_tbl[];
extern s16 tt_cell0_ofs_tbl[];
extern s16 tt_cell0_ofs_tbl_1_0[];
extern u8 tt_cell0_sel_tbl[];

void tt_cell0_set(void)
{
    u8 *p;
    u16 i;
    s32 m;

    p = tt_boss_wk;
    i = *(u16 *)(p + 932);
    m = i % 3;
    p[60] = tt_cell0_kind_tbl[i];
    p[62] = tt_cell0_frame_tbl[i];
    *(s16 *)(p + 584) = tt_cell0_ofs_tbl[m];
    *(s16 *)(p + 632) = tt_cell0_ofs_tbl_1_0[m];
    p[80] = 2;
    p[82] = tt_cell0_sel_tbl[m];
    *(s16 *)(p + 586) = 21;
    *(s16 *)(p + 634) = 9;
}
