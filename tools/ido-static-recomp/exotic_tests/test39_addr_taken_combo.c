#include "types.h"
static s32 *g_ptr;
void SD_CmdPut(s32 arg0) {
    s32 pad[4];
    g_ptr = &arg0;
    pad[0] = arg0;
    func_801ED9F0(pad[0] & 0xFFFF);
}
