/* @SymbolStart: 0x8013F3A4 */
/* @SymbolSize: 468 */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=100:t1,15:t2,105:t3,106:ra,107:t5,108:t4,109:t0 */
/* Address: 0x8013F3A4 (cross-prologue head absorbed), Size: 468 bytes (117 insns) */
#include "types.h"
#include "tokimeki/tt_box.h"

extern u8 *tt_boss_wk;
extern u8 tt_anim_tbl[];
extern u8 tt_roster_kind_tbl[];
extern u8 tt_roster_frame_tbl[];
extern TT_BOX tt_cell_box_tbl[];

extern void tt_unit_clear(void);
extern void tt_shape_clear(void);

void tt_roster_init(void)
{
    u8 *q;
    volatile s32 one;
    s32 i;

    q = tt_boss_wk;
    tt_unit_clear();
    tt_shape_clear();

    q[0] = 1;
    q[834] = 0;
    *(s32 *)(q + 32) = 0;
    *(s32 *)(q + 36) = 0xFFC00000;
    *(s16 *)(q + 20) = 0;
    *(s16 *)(q + 22) = -64;
    *(s32 *)(q + 40) = 0;
    *(s32 *)(q + 44) = 0;
    *(s16 *)(q + 780) = 224;
    *(s16 *)(q + 782) = 0;
    *(s32 *)(q + 56) = (s32)tt_anim_tbl;
    q[60] = tt_roster_kind_tbl[0];
    q[61] = 10;
    q[62] = tt_roster_frame_tbl[0];
    *(s16 *)(q + 680) = 9;
    q[67] = 15;
    q[64] = 96;
    *(s16 *)(q + 632) = 0;
    *(s16 *)(q + 584) = 0;
    q[784] = 1;
    q[72] = 0;
    one = 1;

    for (i = 1; i < 11; i++) {
        *(s32 *)(q + 56 + 20 * i) = (s32)tt_anim_tbl;
        q[60 + 20 * i] = tt_roster_kind_tbl[i];
        q[61 + 20 * i] = 10;
        q[62 + 20 * i] = tt_roster_frame_tbl[i];
        q[64 + 20 * i] = 96;
        q[67 + 20 * i] = 15;
        q[72 + 20 * i] = 0;
        q[784 + i] = 1;
        *(s16 *)(q + 680 + 2 * i) = 9;
        *(s16 *)(q + 584 + 2 * i) = 0;
        *(s16 *)(q + 632 + 2 * i) = 0;
    }

    *(s32 *)(q + 996) = *(s32 *)(q + 32);
    *(s16 *)(q + 682) = 8;
    *(s16 *)(q + 684) = 8;
    *(s16 *)(q + 932) = 0;
    *(s16 *)(q + 934) = 0;
    *(s32 *)(q + 1000) = 0;
    *(s32 *)(q + 1004) = 0;
    *(s32 *)(q + 1008) = 0;
    *(s32 *)(q + 1012) = 0;
    *(s32 *)(q + 1016) = 0;
    *(s16 *)(q + 868) = 0;
    *(s16 *)(q + 870) = 0;
    *(s16 *)(q + 872) = 0;
    q[835] = 1;
    q[836] = 1;
    *(s32 *)(q + 552) = (s32)((u8 *)&tt_cell_box_tbl[31].x);
    *(s32 *)(q + 556) = (s32)((u8 *)&tt_cell_box_tbl[32].x);
    *(s32 *)(q + 560) = (s32)((u8 *)&tt_cell_box_tbl[33].x);
}
