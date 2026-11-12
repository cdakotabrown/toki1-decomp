#include "types.h"
extern void use_array(volatile s32 *);
void SD_CmdPut(s32 arg0) {
    volatile s32 pad[4];
    static s32 *ptr;
    use_array(pad);
    ptr = &arg0;
    func_801ED9F0(arg0 & 0xFFFF);
}
