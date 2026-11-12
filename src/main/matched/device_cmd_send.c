/* Address: 0x80044980 - 0x800449A4 (36 bytes) */
/* Region: GCC (OR encoding) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern void SD_CmdPut(u16 arg);

s32 device_cmd_send(s32 a0) {
    SD_CmdPut((u16)(a0 & 0xFFFF));
    return 1;
}
