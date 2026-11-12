#include "types.h"
void SD_CmdPut(s32 arg0) {
    s32 arr[4];
    s32 *p = &arr[0];
    *p = arg0;
    func_801ED9F0(arg0 & 0xFFFF);
}
