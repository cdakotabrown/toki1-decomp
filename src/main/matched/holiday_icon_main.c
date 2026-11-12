/* Address: 0x80075CD8 - 0x80075D38 (96 bytes, 24 instructions) */
/* Region: GCC (OR encoding) */
/* Compiler: IDO 5.2 via ido-static-recomp */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern void k_disp_inc(void);
extern void step_directional_scroll_once(void);
extern s32 holiday_icon_check(void);
extern s32 tel_code_check(void);
extern s32 hotspot_table_check2(void);
extern s32 near_click_check(void);

void holiday_icon_main(void) {
    k_disp_inc();
    step_directional_scroll_once();
    if (holiday_icon_check() != 0) {
        return;
    }
    if (tel_code_check() != 0) {
        return;
    }
    if (hotspot_table_check2() != 0) {
        return;
    }
    near_click_check();
}
