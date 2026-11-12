/* @SymbolStart: 0x80145A00 */
/* @PostProc: patch_insn_reg_field 0x18:rt:v0:v1 0x1c:rs:v0:v1 0x1c:rt:v0:v1 0x30:rs:v0:v1 0x3c:rs:v0:v1 0x44:rs:v0:v1 */
/* @SymbolSize: 140 */
/* Address: 0x80145A10 */
/* Size: 124 bytes (31 instructions) */
#include "types.h"

extern void save_file_fade_out(void);
extern void save_file_init(void);
extern void save_file_step(void);
extern void save_file_anim_disp(void);
extern void save_file_shade_disp(void);

extern s32 step_state;
extern u8 now_sub_step;

void save_file_main(void)
{
    s32 a0 = step_state;
    s32 v1 = now_sub_step;
    s32 tmp;
    tmp = a0 + 1;
    step_state = tmp;
    switch (v1) {
    case 0:
        save_file_init();
        break;
    case 1:
        save_file_step();
        break;
    case 2:
        save_file_fade_out();
        break;
    }
    save_file_anim_disp();
    save_file_shade_disp();
}
