/* @RodataBase: 0x8014CA10 */
/* @SymbolSize: 200 */
#include "types.h"

extern void etc_sub_sub_step0(void);
extern void etc_sub_sub_main(s32 a0, s32 a1);
extern void etc_sub_sub_step4(void);
extern void etc_sub_sub_step5(void);
extern void etc_sub_sub_step6(void);
extern void etc_sub_sub_step7(void);
extern u8 now_sub_sub_step;

void etc_sub_step1(void) {
    s32 cmd = now_sub_sub_step;

    switch (cmd) {
    case 0:
        etc_sub_sub_step0();
        break;
    case 1:
        etc_sub_sub_main(2, 0);
        break;
    case 2:
        etc_sub_sub_main(3, 1);
        break;
    case 3:
        etc_sub_sub_main(4, 2);
        break;
    case 4:
        etc_sub_sub_step4();
        break;
    case 5:
        etc_sub_sub_step5();
        break;
    case 6:
        etc_sub_sub_step6();
        break;
    case 7:
        etc_sub_sub_step7();
        break;
    }
}
