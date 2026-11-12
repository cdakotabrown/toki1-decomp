/* @SymbolStart: 0x801353A4 */
/* @SymbolSize: 132 */
/* @PostProc: patch_insn_reg_field 0x18:rt:v0:v1 0x1c:rs:v0:v1 0x1c:rt:v0:v1 0x30:rs:v0:v1 0x3c:rs:v0:v1 0x44:rs:v0:v1 */
/* Address: 0x801353A4 (cross-prologue head) / 0x801353B4 (prologue) */
/* Size: 132 bytes (33 insns) */
#include "types.h"

extern s32 step_counter;
extern u8 now_sub_sub_step;

extern void rand_tile_disp(void);
extern void taco_select_init(void);
extern void taco_select_adjust(void);
extern void taco_menu_select_step(void);

void taco_select_step(void)
{
    s32 val;
    s32 sel;

    val = step_counter;
    sel = now_sub_sub_step;

    step_counter = val + 1;

    switch (sel) {
    case 0:
        taco_select_init();
        break;
    case 1:
        taco_select_adjust();
        break;
    case 2:
        taco_menu_select_step();
        break;
    }

    rand_tile_disp();
}
