/* @SymbolStart: 0x80136B50 */
/* @RodataBase: 0x8015D828 */
/* @SymbolSize: 192 */
/* Address: 0x80136B58 */
#include "types.h"

extern u8 now_sub_sub_step;

extern s32 taco_scroll_pos;

extern void sub_sub_step_up(void);
extern void taco_clear0(void);
extern void taco_ramp6(void);
extern void taco_cmd_send7(void);
extern void taco_ramp1(void);
extern void taco_scroll3(void);
extern void taco_scroll5(void);
extern void taco_panel_wait4(void);

void taco_subsub_dispatch(void)
{
    u8 t6_in = now_sub_sub_step;

    switch (t6_in) {
        case 0: taco_clear0(); break;
        case 1: taco_ramp1(); break;
        case 2:
            taco_scroll_pos = 0;
            sub_sub_step_up();
            break;
        case 3: taco_scroll3(); break;
        case 4: taco_panel_wait4(); break;
        case 5: taco_scroll5(); break;
        case 6: taco_ramp6(); break;
        case 7: taco_cmd_send7(); break;
    }
}
