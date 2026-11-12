#include "types.h"
typedef struct { s32 val; } Arg;
extern void use_ptr(volatile s32 *);
void SD_CmdPut(Arg arg0) {
    volatile s32 pad[4];
    use_ptr(pad);
    func_801ED9F0(arg0.val & 0xFFFF);
}
