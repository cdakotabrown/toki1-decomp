#include "types.h"
static s32 *g_ptr;
void SD_CmdPut(s32 arg0) {
    volatile s32 pad[4];
    pad[0] = 0;
    g_ptr = &arg0;
    func_801ED9F0(arg0 & 0xFFFF);
}
