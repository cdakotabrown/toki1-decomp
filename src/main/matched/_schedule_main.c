/* Address: 0x80071040 */
/* Size: 80 bytes (20 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* Compiler: IDO 5.2 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: swap_registers v0 v1 */
#include "types.h"

extern u8 now_sub_sub_step;
extern void schedule_day_main(void);
extern void schedule_exit(void);

void _schedule_main(void)
{
    switch (now_sub_sub_step) {
        case 0:
            schedule_day_main();
            break;
        case 1:
            schedule_exit();
            break;
    }
}
