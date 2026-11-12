/* Function: holiday_tel */
/* Address: 0x80077FF8 */
/* Size: 116 bytes (29 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @SymbolStart: 0x80077FE8 */
/* @SymbolSize: 132 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: swap_registers v0:v1 --unidirectional */
#include "types.h"

extern u32 step_counter;
extern u8 now_sub_sub_step;

extern void holiday_tel_init(void);
extern void holiday_tel_call(void);
extern void holiday_tel_exit(void);
extern void cal_base_show(void);

void holiday_tel(void) {
    u8 mode;

    step_counter++;
    mode = now_sub_sub_step;
    switch (mode) {
    case 0:
        holiday_tel_init();
        break;
    case 1:
        holiday_tel_call();
        break;
    case 2:
        holiday_tel_exit();
        break;
    }
    cal_base_show();
}
