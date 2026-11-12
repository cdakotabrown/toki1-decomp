/* Address: 0x800565F0 */
/* Size: 116 bytes (29 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: patch_stack_complete --old 24 --new 40 */
/* @PostProc: swap_instructions --offset1 0x34 --offset2 0x3C */
#include "types.h"

extern void card_state_clear(void);
extern void card_w_clear(void);
extern void card_step_init(void);

extern u8 album_status[];
extern u8 album_status1;
extern u8 card_sys_present;

void save_flag_clear(void)
{
    register u8 *ptr;
    register u8 *end;

    card_state_clear();
    card_w_clear();

    album_status[0] = 0;
    album_status1 = 0;
    album_status[2] = 0;
    ptr = (&album_status[3]) - 4;
    end = (&album_status[3]) + 8;
    do {
        ptr += 4;
        ptr[1] = 0;
        ptr[2] = 0;
        ptr[3] = 0;
        ptr[0] = 0;
    } while (ptr != end);
    card_step_init();
    card_sys_present = 0;
}
