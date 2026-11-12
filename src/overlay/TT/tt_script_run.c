/* @SymbolSize: 436 */
/* @RodataBase: 0x8014D834 */
/* tt_script_run - TT overlay, Address: 0x8014C2E8, Size: 436 bytes */
/* leaves no gap.  `@SymbolSize: 436` above is the authoritative window */
#include "types.h"

extern u8 *tt_stage_wk;

extern s32 tt_ent_spawn(s32, u32, s32);
extern s32 tt_curve_spawn(s32);
extern s32 tt_trio_spawn(s32);
extern s32 tt_sway_spawn(s32);
extern s32 tt_arc_spawn(s32);
extern s32 tt_spread_spawn(s32);
extern s32 tt_orbit_spawn(s32);
extern s32 tt_rise_spawn(s32);
extern s32 tt_column_spawn(s32);

void tt_script_run(void)
{
    u8 *ctl;
    u8 *p;
    s32 id;
    s32 sentinel;
    u32 op;

    sentinel = -1;
    ctl = tt_stage_wk;
    for (;;) {
        p = *(u8 **)(ctl + 4);
        id = *(s16 *)p;
        if (sentinel == id) {
            return;
        }
        if (*(u16 *)(ctl + 2) < (u32)id) {
            return;
        }
        switch ((u32)*(u16 *)(p + 2)) {
        case 192: tt_curve_spawn(*(u8 *)(p + 4)); p = *(u8 **)(ctl + 4); break;
        case 193: tt_trio_spawn(*(u8 *)(p + 4)); p = *(u8 **)(ctl + 4); break;
        case 194: tt_sway_spawn(*(u8 *)(p + 4)); p = *(u8 **)(ctl + 4); break;
        case 195: tt_arc_spawn(*(u8 *)(p + 4)); p = *(u8 **)(ctl + 4); break;
        case 196: tt_spread_spawn(*(u8 *)(p + 4)); p = *(u8 **)(ctl + 4); break;
        case 197: tt_orbit_spawn(*(u8 *)(p + 4)); p = *(u8 **)(ctl + 4); break;
        case 198: tt_rise_spawn(*(u8 *)(p + 4)); p = *(u8 **)(ctl + 4); break;
        case 199: tt_column_spawn(*(u8 *)(p + 4)); p = *(u8 **)(ctl + 4); break;
        case 48:
        case 49:
            tt_ent_spawn(*(s8 *)(p + 4), (u32)*(u16 *)(p + 2), *(u8 *)(p + 5));
            p = *(u8 **)(ctl + 4);
            break;
        }
        *(u8 **)(ctl + 4) = p + 6;
    }
}
