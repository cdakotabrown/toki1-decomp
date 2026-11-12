#include "types.h"
void SD_CmdPut(s32 arg0) {
    static s32 *last_arg;
    last_arg = &arg0;
    func_801ED9F0(arg0 & 0xFFFF);
}
