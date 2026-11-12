/* kangei_schedule_next - KANGEI overlay, Address: 0x801335C8, Size: 56 bytes (14 insns) */
/* @SymbolStart: 0x801335B0 */
/* @SymbolSize: 80 */
#include "types.h"

extern void hizuke_advance(void);
extern void hizuke_init(void);
extern void sub_step_up(void);

extern u8 schedule_cursor;
extern u8 kangei_birth_done;
extern u8 kangei_switch2_done_flag;

void kangei_schedule_next(void)
{
    schedule_cursor = schedule_cursor + 1;
    kangei_birth_done = 0;
    kangei_switch2_done_flag = 0;
    hizuke_advance();
    hizuke_init();
    sub_step_up();
}
