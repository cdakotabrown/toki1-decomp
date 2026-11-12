#include "types.h"
union { s32 i; u16 h[2]; } u;
void SD_CmdPut(s32 arg0) {
    u.i = arg0;
    func_801ED9F0(u.h[0]);
}
