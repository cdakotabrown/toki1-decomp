/* @SymbolStart: 0x80140CAC */
/* @PostProc: patch_insn_reg_field 0x48:rt:v1:v0 0x50:rs:v1:v0 0x58:rs:v1:v0 0x90:rt:v1:v0 0x98:rs:v1:v0 0xa0:rs:v1:v0 */
/* @SymbolSize: 200 */
/* tt_sub_dispatch - TT overlay, Address: 0x80140CB4, 192 bytes (48 insns) */
#include "types.h"

extern u8 *tt_boss_wk;

extern void tt_marker_step(void);
extern void tt_serve_step(void);
extern void tt_serve_effect_step(void);
extern void tt_approach_step(void);

void tt_sub_dispatch(void)
{
    u8 *t6 = tt_boss_wk;
    u16 outer;
    u16 inner;
    u8 *p;

    outer = *(u16 *)(t6 + 782);
    switch (outer) {
    case 0:
        p = tt_boss_wk;
        inner = *(u16 *)(p + 6);
        switch (inner) {
        case 0:
            tt_marker_step();
            break;
        case 2:
            tt_serve_step();
            break;
        }
        break;
    case 1:
        p = tt_boss_wk;
        inner = *(u16 *)(p + 6);
        switch (inner) {
        case 0:
            tt_serve_effect_step();
            break;
        case 2:
            tt_approach_step();
            break;
        }
        break;
    }
}
