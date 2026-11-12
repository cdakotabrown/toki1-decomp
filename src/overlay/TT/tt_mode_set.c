/* @SymbolSize: 228 */
/* @PostProc: patch_insn_reg_field 0x20:rt:a2:a1 0x2c:rt:a2:a1 0x34:rt:a2:a1 */
#include "types.h"

extern u8 *tt_scroll_wk;
extern u8 *tt_stage_wk;

extern u8 tt_wave1_scr[];
extern u8 tt_obj1_scr[];
extern u8 tt_wave2_scr[];
extern u8 tt_obj2_scr[];
extern u8 tt_wave3_scr[];
extern u8 tt_obj3_scr[];
extern u8 tt_wave4_scr[];
extern u8 tt_obj4_scr[];

extern void tt_serve(void);
extern void tt_mode1_main(void);
extern void tt_match(void);
extern void tt_step_dispatch(void);

void tt_mode_set(void)
{
    u8 *cfg;
    u8 *vt;
    u8 k;

    cfg = tt_scroll_wk;
    vt = tt_stage_wk;
    *(s32 *)(vt + 0) = 0;
    k = *(u8 *)(cfg + 11);
    *(s32 *)(cfg + 0) = 0x8000;
    *(s32 *)(cfg + 4) = 0x8000;
    switch (k) {
    case 0:
    default:
        *(s32 *)(vt + 4) = (s32)tt_wave1_scr;
        *(s32 *)(vt + 8) = (s32)tt_obj1_scr;
        *(s32 *)(vt + 12) = (s32)tt_serve;
        break;
    case 1:
        *(s32 *)(vt + 4) = (s32)tt_wave2_scr;
        *(s32 *)(vt + 8) = (s32)tt_obj2_scr;
        *(s32 *)(vt + 12) = (s32)tt_mode1_main;
        break;
    case 2:
        *(s32 *)(vt + 4) = (s32)tt_wave3_scr;
        *(s32 *)(vt + 8) = (s32)tt_obj3_scr;
        *(s32 *)(vt + 12) = (s32)tt_match;
        break;
    case 3:
        *(s32 *)(vt + 4) = (s32)tt_wave4_scr;
        *(s32 *)(vt + 8) = (s32)tt_obj4_scr;
        *(s32 *)(vt + 12) = (s32)tt_step_dispatch;
        break;
    }
}
