#include "types.h"
#include <setjmp.h>
void SD_CmdPut(s32 arg0) {
    jmp_buf buf;
    if (setjmp(buf) == 0) func_801ED9F0(arg0 & 0xFFFF);
}
