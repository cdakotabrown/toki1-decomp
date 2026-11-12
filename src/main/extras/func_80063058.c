/* Address: 0x80063058 */
/* Size: 108 bytes (27 instructions) */
/* Compiler: GCC-like (0x80040000-0x80080000 region) */
#include "types.h"

extern void title_show_init(void);
extern void title_show(void);
extern void title_elem(void);

extern s32 step_counter;

void func_80063058(s32 v1_val, s32 t6_val) {
    step_counter = t6_val + 1;

    if (v1_val == 0) {
        title_show_init();
    } else if (v1_val == 1) {
        title_show();
    } else if (v1_val == 2) {
        title_elem();
    }

}
