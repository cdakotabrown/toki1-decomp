#include "types.h"
void SD_CmdPut(s32 arg0) {
    void (*fp)(s32) = func_801ED9F0;
    (*fp)(arg0 & 0xFFFF);
}
