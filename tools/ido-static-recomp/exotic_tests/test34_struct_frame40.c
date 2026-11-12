#include "types.h"
typedef struct { s32 a; s32 b; s32 c; s32 d; s32 e; s32 f; } Big;
extern void use_big(Big *);
void SD_CmdPut(Big arg0) {
    use_big(&arg0);
    func_801ED9F0(arg0.a & 0xFFFF);
}
