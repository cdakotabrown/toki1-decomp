/* Address: 0x80046660 */
/* Size: 84 bytes (21 instructions) */
/* Compiler: IDO 5.2 (compiles to GCC-like output) */
/* Region: GCC (0x80041000-0x80080FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
#include "common.h"

extern u8 cdq_footer_chk;
extern void device_cmd_send(s32);
extern void cd_command_tag_pack(u8, s32, void*);
extern void cd_command_issue(s32, s32, void*);

void cd_read_start2(u8 arg0, s32 arg1, s32 arg2) {
    u8 buffer[32];
    volatile s32 pad[4];

    device_cmd_send(6);
    cd_command_tag_pack(arg0, arg1, buffer);
    cd_command_issue(6, arg2, buffer);

    cdq_footer_chk = 1;
}
