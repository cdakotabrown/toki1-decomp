/* @SymbolStart: 0x80139140 */
/* @SymbolSize: 156 */
/* @RodataBase: 0x8015D988 */
/* Address: 0x80139148 */
/* Size: 148 bytes (37 insns) */
#include "types.h"

extern u8 now_sub_sub_step;

extern void taco_round_end();
extern void taco_wait4();
extern void taco_goto_sub5_skip();
extern void taco_scroll_in1();
extern void taco_scroll_out3();
extern void taco_name_entry();

void taco_step_dispatch(void)
{
    u8 t6 = now_sub_sub_step;

    switch (t6) {
    case 0: taco_round_end(); break;
    case 1: taco_scroll_in1(); break;
    case 2: taco_name_entry(); break;
    case 3: taco_scroll_out3(); break;
    case 4: taco_wait4(); break;
    case 5: taco_goto_sub5_skip(); break;
    }
}
