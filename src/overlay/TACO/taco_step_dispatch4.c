/* @SymbolStart: 0x80144780 */
/* @RodataBase: 0x8015DBF0 */
/* @SymbolSize: 188 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:t6 0x14:rs:v0:t6 0x14:rt:v0:t6 0x1c:rs:v0:t6 0x28:rt:v0:t6 */
#include "types.h"

extern void goto_sub_sub_step(s32 a0);
extern void func_8014483C(void);
extern void func_80144948(void);
extern void taco_cutscene1(void);
extern void taco_event1(void);
extern void taco_event2(void);

extern volatile u8 now_sub_sub_step;
extern s32 pad_trig;

void taco_step_dispatch4(void) {
    u8 cmd = now_sub_sub_step;

    switch (cmd) {
    case 0:
        func_8014483C();
        break;
    case 1:
        taco_cutscene1();
        break;
    case 2:
        taco_event1();
        break;
    case 3:
        taco_event2();
        break;
    case 4:
        func_80144948();
        break;
    }
    if (now_sub_sub_step != 5) {
        if ((pad_trig & 0x860) != 0) {
            goto_sub_sub_step(4);
        }
    }
}
