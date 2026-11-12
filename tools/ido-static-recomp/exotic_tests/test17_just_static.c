#include "types.h"
void SD_CmdPut(s32 arg0) {
    volatile s32 pad[4];
    static s32 *ptr;
    pad[0] = 0;
    ptr = &arg0;
    func_801ED9F0(arg0 & 0xFFFF);
}
