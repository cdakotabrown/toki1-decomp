#include "types.h"
extern s32 arr[4];
void SD_CmdPut(s32 arg0) {
    arr[0] = arg0;
    func_801ED9F0(arg0 & 0xFFFF);
}
