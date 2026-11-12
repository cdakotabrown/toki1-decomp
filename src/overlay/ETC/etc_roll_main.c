/* @SymbolStart: 0x80142960 */
/* @SymbolSize: 160 */
/* @PostProc: swap_registers a0:v1 --unidirectional --exclude-offsets 0x74,0x94 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x1c:rs:v0:v1 0x28:rs:v0:v1 0x7c:rt:v1:a0 0x9c:rt:v1:a0 */
/* etc_roll_main - ETC overlay, Address: 0x80142968, Size: 152 bytes (38 insns) */
#include "types.h"

extern s32 goto_step();
extern s32 get_CD_State();
extern s32 etc_roll_main0();
extern s32 etc_sub_main();
extern s32 pad_trig;

extern u8 now_sub_step;

void etc_roll_main(void)
{
    s32 a0 = now_sub_step;

    s32 v0;
    s32 t7;

    switch (a0) {
    case 0:
        etc_roll_main0();
        break;
    case 1:
        etc_sub_main();
        break;
    }

    v0 = get_CD_State();
    if ((v0 & 0x4) == 0) {
        return;
    }

    t7 = pad_trig;
    if ((t7 << 10) < 0) {
        goto_step(196);
    }

    t7 = pad_trig;
    if ((t7 << 9) < 0) {
        goto_step(145);
    }
}
