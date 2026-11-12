#include "types.h"
typedef struct { s32 val; } Arg;
void SD_CmdPut(Arg arg0) {
    func_801ED9F0(arg0.val & 0xFFFF);
}
