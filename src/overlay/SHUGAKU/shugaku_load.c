/* @SymbolStart: 0x801382C0 */
/* @SymbolSize: 168 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x1c:rs:v0:v1 0x28:rs:v0:v1 0x30:rs:v0:v1 0x38:rs:v0:v1 0x68:rt:v0:v1 0x6c:rs:v0:v1 0x6c:rt:v0:v1 0x74:rs:v0:v1 0x74:rt:v0:v1 0x7c:rt:v0:v1 */
#include "types.h"

extern void shugaku_read();
extern s32 get_CD_State2(void);
extern void shugaku_init(void);
extern void shugaku_cal_cw_scroll(void);
extern void game_reset(void);

extern s32 step_counter;

void shugaku_load(void)
{
    switch (step_counter) {
    case 0:
        shugaku_read();
        break;
    case 1:
        if (get_CD_State2() & 1) {
            step_counter = step_counter + 1;
        }
        break;
    case 2:
        shugaku_init();
        break;
    case 3:
        shugaku_cal_cw_scroll();
        break;
    default:
        game_reset();
        break;
    }
}
