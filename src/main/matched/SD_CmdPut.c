/* Function: SD_CmdPut */
/* Likely Symbol: SD_CmdProc (from o.bin, offset +44 bytes from 0x80079BCC) */
/* Address: 0x80079BA0 */
/* Size: 36 bytes */
/* Region: OR (0x80070000+) */
/* Compiler: IDO 5.2 via ido-static-recomp */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern void cmd_que_put(u16 arg);

void SD_CmdPut(s32 arg0)
{
    cmd_que_put((u16)(arg0 & 0xFFFF));
}
