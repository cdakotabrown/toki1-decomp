#include "types.h"
void SD_CmdPut(register s32 arg0) {
    static s32 *ptr;
    ptr = &arg0;
    func_801ED9F0(arg0 & 0xFFFF);
}
