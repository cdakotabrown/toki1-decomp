#include "types.h"
void SD_CmdPut(s32 arg0) {
    register s32 saved asm("s0") = arg0;
    static s32 *ptr;
    ptr = &arg0;
    func_801ED9F0(saved & 0xFFFF);
}
