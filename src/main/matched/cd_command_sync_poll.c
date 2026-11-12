/* Address: 0x80045454 */
/* Size: 100 bytes */
/* Compiler: IDO 5.2 */
/* Region: GCC (0x80040000-0x80080000) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s32 CdSync(s32 a0, void *a1);

extern u8 cd_state2;
extern s8 cd_head_pos;
extern u8 cd_sync_result;

void cd_command_sync_poll(void)
{
    s32 result;

    result = CdSync(1, &cd_sync_result);
    switch (result) {
    case 2:
        cd_state2 |= 2;
        break;
    case 5:
        cd_head_pos = -1;
        break;
    }
}
