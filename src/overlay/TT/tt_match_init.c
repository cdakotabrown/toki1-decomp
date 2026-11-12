/* @SymbolStart: 0x80135870 */
/* @SymbolSize: 484 */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* Address: 0x80135870 (cross-prologue head absorbed), Size: 484 bytes */
/* @PostProc: swap_instructions --offset1 0x54 --offset2 0x60 */
/* @PostProc: swap_instructions --offset1 0x58 --offset2 0x5c */
#include "types.h"
#include "tokimeki/tt_box.h"

extern u8 tt_match_anim_tbl[];
extern u8 tt_match_frame_tbl[];
extern u8 tt_anim_tbl[];

extern u8 *tt_boss_wk;
extern TT_BOX tt_hit_box_tbl[];

extern void tt_unit_clear(void);
extern void tt_shape_clear(void);
extern void tt_court_layout(s32 a, s32 b, s32 c);

void tt_match_init(void)
{
    u8 *s;
    u8 *p;
    u8 *b;
    u8 *h;
    u8 *ta;
    u8 *tb;
    u8 *tbl;
    u8 *end;
    s32 i;

    s = tt_boss_wk;
    tt_unit_clear();
    tt_shape_clear();

    s[0] = 1;
    s[834] = 0;
    *(s32 *)(s + 32) = 0;
    *(s32 *)(s + 36) = 0xFFD00000;
    *(s16 *)(s + 20) = 0;
    *(s16 *)(s + 22) = -48;
    *(s32 *)(s + 40) = 0;
    *(s32 *)(s + 44) = 0x00020000;

    p = s;
    b = s;
    h = s;
    ta = tt_match_anim_tbl;
    tb = tt_match_frame_tbl;
    end = tt_match_frame_tbl + 6;
    tbl = tt_anim_tbl;
    do {
        *(s32 *)(p + 56) = (s32)tbl;
        p[60] = 0;
        p[61] = *ta;
        p[62] = *tb;
        p[64] = 96;
        p[67] = 15;
        p[72] = 0;
        b[784] = 1;
        tb += 1;
        p += 20;
        ta += 1;
        b += 1;
        h += 2;
        *(s16 *)(h + 678) = 8;
        *(s16 *)(h + 582) = 0;
        *(s16 *)(h + 630) = 0;
    } while (tb < end);

    p = s + 160;
    b = s + 8;
    i = 16;
    h = s + 16;
    do {
        *(s32 *)(p + 56) = (s32)tbl;
        p[60] = 0;
        p[61] = 4;
        p[62] = 0;
        p[64] = 96;
        p[67] = 15;
        b[784] = 1;
        i += 2;
        p += 20;
        b += 1;
        h += 2;
        *(s16 *)(h + 678) = 7;
        *(s16 *)(h + 582) = 0;
        *(s16 *)(h + 630) = 0;
    } while (i != 32);

    tt_court_layout(0, 0, 0);

    *(s32 *)(s + 552) = (s32)((u8 *)&tt_hit_box_tbl[7].x);
    *(s32 *)(s + 556) = (s32)((u8 *)&tt_hit_box_tbl[8].x);
    *(s32 *)(s + 568) = (s32)((u8 *)&tt_hit_box_tbl[9].x);
    *(s32 *)(s + 560) = (s32)((u8 *)&tt_hit_box_tbl[10].x);
    *(s16 *)(s + 780) = 96;
    *(s16 *)(s + 782) = 0;
    s[145] = 0;
    s[165] = 16;
    *(s16 *)(s + 932) = 0;
    *(s16 *)(s + 934) = 0;
    *(s16 *)(s + 936) = 0;
    *(u16 *)(s + 938) = 0xF800;
    *(s16 *)(s + 940) = 0;
    *(s16 *)(s + 942) = 1024;
    *(s16 *)(s + 944) = 0;
    *(s16 *)(s + 946) = 0;
    *(s16 *)(s + 948) = 0;
    *(s16 *)(s + 950) = 0;
    *(s16 *)(s + 952) = 0;
    *(s16 *)(s + 868) = 0;
    s[835] = 0;
    s[836] = 0;
}
