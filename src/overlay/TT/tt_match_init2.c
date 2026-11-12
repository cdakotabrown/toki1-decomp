/* @SymbolSize: 436 */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @CompileEnv: IDO52_NO_LOOP_TEST_REPLACEMENT=occ:2 */
/* @CompileEnv: IDO52_COPYPROP_SITE=lod:3:deny,lod:4:deny */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=17:v1,110:a1,14:a0,50:a0 */
/* Address: 0x8013AC0C, Size: 436 bytes (109 insns) */
/* - `@SymbolSize: 436`.  TT.sym's `0x8013AC0C:560` is a MERGED BLOB: the */
/* @PostProc: swap_instructions --offset1 0x5c --offset2 0x6c */
/* @PostProc: swap_instructions --offset1 0x60 --offset2 0x6c */
/* @PostProc: swap_instructions --offset1 0x64 --offset2 0x6c */
/* @PostProc: swap_instructions --offset1 0x68 --offset2 0x6c */
/* @PostProc: swap_instructions --offset1 0x98 --offset2 0x9c */
/* @PostProc: swap_instructions --offset1 0xc0 --offset2 0xc8 */
/* @PostProc: swap_instructions --offset1 0xc4 --offset2 0xc8 */
#include "types.h"
#include "tokimeki/tt_box.h"

extern u8 tt_anim_tbl[];
extern s16 tt_match_pos;

extern u8 *tt_boss_wk;
extern TT_BOX tt_cell_box_tbl[];

extern void tt_unit_clear(void);
extern void tt_shape_clear(void);
extern void tt_cell0_set(void);
extern void tt_cell1_set(void);
extern void tt_cell2_set(void);
extern void tt_cell3_set(void);

void tt_match_init2(void)
{
    u8 *s;
    u8 *p;
    u8 *b;
    u8 *h;
    u8 *x;
    u8 *y;
    u8 *tbl;
    s32 i;
    s32 j;

    s = tt_boss_wk;
    tt_unit_clear();
    tt_shape_clear();

    s[0] = 1;
    s[834] = 0;
    *(s32 *)(s + 32) = 0;
    *(s32 *)(s + 36) = 0xFFB00000;
    *(s16 *)(s + 20) = 0;
    *(s16 *)(s + 22) = -80;

    tbl = tt_anim_tbl;
    i = 0;
    p = s;
    b = s;
    h = s;
    do {
        i += 1;
        *(s32 *)(p + 56) = (s32)tbl;
        p[60] = 0;
        *(s8 *)(p + 61) = 9;
        p[64] = 96;
        p[67] = 15;
        p[72] = 0;
        b[784] = 1;
        p += 20;
        b += 1;
        h += 2;
        *(s16 *)(h + 678) = 9;
    } while (i < 10);

    x = s + 2;
    x[784] = 0;
    x[785] = 0;

    j = 4;
    y = s + 4;
    do {
        j += 4;
        y[785] = 0;
        y[786] = 0;
        y[787] = 0;
        y += 4;
        y[780] = 0;
    } while (j != 24);

    s[786] = 1;
    s[787] = 1;
    s[788] = 1;
    s[789] = 1;
    s[790] = 1;
    s[791] = 1;
    s[792] = 1;
    s[140] = 0;
    s[142] = 0;
    *(s16 *)(s + 592) = 0;
    *(s16 *)(s + 640) = 0;
    *(s16 *)(s + 780) = 128;
    *(s16 *)(s + 782) = 0;
    *(s16 *)(s + 728) = 96;
    *(s16 *)(s + 730) = 96;
    *(s16 *)(s + 732) = 96;
    *(s16 *)(s + 734) = 96;
    *(s16 *)(s + 932) = 11;
    *(s16 *)(s + 934) = 11;
    *(s16 *)(s + 936) = 2;
    *(s16 *)(s + 938) = 2;
    *(s16 *)(s + 940) = 0;
    *(s32 *)(s + 996) = 0;
    s[835] = 0;
    s[836] = 0;
    s[837] = 0;
    s[838] = 0;
    *(s16 *)(s + 868) = 0;
    *(s16 *)(s + 870) = 0;
    *(s16 *)(s + 872) = 0;
    *(s16 *)(s + 874) = 0;

    tt_cell0_set();
    tt_cell1_set();
    tt_cell2_set();
    tt_cell3_set();

    tt_match_pos = 15952;
    *(s32 *)(s + 552) = (s32)((u8 *)&tt_cell_box_tbl[1].x);
    *(s32 *)(s + 556) = (s32)((u8 *)&tt_cell_box_tbl[2].x);
}
