#include "types.h"
typedef struct { s32 val; } Arg;
void SD_CmdPut(Arg arg0) {
    volatile s32 pad[4];
    pad[0] = 0;
    func_801ED9F0(arg0.val & 0xFFFF);
}
