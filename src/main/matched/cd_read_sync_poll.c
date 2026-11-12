/* Address: 0x8004541C */
/* Size: 56 bytes (14 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern u8 cd_state2;
extern s32 cd_read_sync_poll_tbl0;
extern char cd_result_buf[];

extern s32 CdReadSync(s32 a0, char *a1);

s32 cd_read_sync_poll(void) {
    cd_state2 = 128;
    cd_read_sync_poll_tbl0 = CdReadSync(1, cd_result_buf);
    return cd_read_sync_poll_tbl0;
}
