/* Address: 0x80046610 */
/* Size: 80 bytes (20 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: scheduler_normalizer */
/* @PostProc: remap_stack_slots --map '24:40' */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern u8 cdq_footer_chk;

extern void device_cmd_send(s32);
extern void cd_command_tag_pack(s32, s32, u8 *);
extern void cd_command_issue(s32, s32, u8 *);

void cd_read_start(s32 param0, s32 param1, u8 *param2) {
    u8 local_buf[32];

    device_cmd_send(6);
    cd_command_tag_pack((u8)param0, param1, local_buf);
    cd_command_issue(6, (s32)param2, local_buf);
    cdq_footer_chk = 0;
}
