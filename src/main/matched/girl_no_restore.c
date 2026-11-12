/* Address: 0x80084994 */
/* Size: 44 bytes (11 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* Compiler: IDO 5.2 */
/* Debug Symbol: date_name_set (from o.bin) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern void sub_sub_step_up(void);

extern u8 girl_no;
extern u8 now_girl;

void girl_no_restore(void)
{
    u8 t6;

    t6 = girl_no;
    now_girl = t6;
    sub_sub_step_up();
}
