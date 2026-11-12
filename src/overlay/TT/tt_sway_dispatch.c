/* @SymbolStart: 0x8013F7A0 */
/* @SymbolSize: 200 */
/* Address: 0x8013F7A8 */
/* Size: 192 bytes (48 insns) */
/* @PostProc: patch_insn_reg_field 0x24:rt:a0:v1 0x30:rt:a0:v1 0x44:rt:a0:v1 0x48:rt:v1:v0 0x50:rs:v1:v0 0x58:rs:v1:v0 0x58:rt:a0:v1 0x90:rt:v1:v0 0x98:rs:v1:v0 0xa0:rs:v1:v0 0xa0:rt:a0:v1 */
#include "types.h"

extern u8 *tt_boss_wk;
extern u8 *tt_step_wk;

extern void tt_sway_step(void);
extern void tt_xpos_settle(void);

void tt_sway_dispatch(void)
{
    u8 *t6_hidden = tt_boss_wk;
    u16 substate;

    switch (*(u16 *)(t6_hidden + 782)) {
    case 0:
        substate = *(u16 *)(tt_boss_wk + 6);
        switch (substate) {
        case 0:
            tt_sway_step();
            break;
        case 1:
            tt_xpos_settle();
            break;
        }
        break;
    case 1:
        substate = *(u16 *)(tt_boss_wk + 6);
        switch (substate) {
        case 0:
            tt_sway_step();
            break;
        case 1:
            tt_xpos_settle();
            break;
        }
        break;
    }
}
