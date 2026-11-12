/* @SymbolStart: 0x80138328 */
/* @SymbolSize: 68 */
#include "types.h"

extern void taco_actor_pool_clear(void);
extern void sub_sub_step_up(void);
extern u8 taco_load_no;
extern u8 taco_load_state;
extern s32 taco_view_flag;

void taco_main9_init0(void)
{
    taco_load_no = 20;
    taco_load_state = 0;
    taco_view_flag = 128;
    taco_actor_pool_clear();
    sub_sub_step_up();
}
