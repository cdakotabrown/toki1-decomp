/* Address: 0x80070F50 */
/* Size: 60 bytes */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: patch_stack 24 40 */
#include "types.h"

extern u8 season_no;
extern void girl_counter_periodic_decrement(void);
extern void syoushin_up(void);
extern void sub_sub_step_up(void);

void schdeule_girls_param(void) {
    u8 t6;

    t6 = season_no;

    if (t6 == 0) {
        girl_counter_periodic_decrement();
        syoushin_up();
    }

    sub_sub_step_up();
}
