/* Address: 0x80142578 */
/* Size: 92 bytes (23 insns) */
#include "types.h"

extern s32 sub_step_up(void);
extern s32 taiiku_sub_sub_main(void);
extern u8 taiiku_ninin_girl1;

void taiiku_main2(void) {
    s32 v;
    v = taiiku_sub_sub_main();
    switch (v) {
    case 0: return;
    case 1: sub_step_up(); return;
    case 2:
        taiiku_ninin_girl1 = 0;
        sub_step_up();
        return;
    }
}
