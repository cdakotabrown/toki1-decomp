#include "types.h"
typedef struct { s32 a; s32 b; s32 c; s32 d; s32 e; } Arg;
void SD_CmdPut(Arg arg0) {
    func_801ED9F0(arg0.a & 0xFFFF);
}
