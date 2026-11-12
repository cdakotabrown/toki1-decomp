#include "types.h"
#include <stdarg.h>
void SD_CmdPut(s32 arg0, ...) {
    va_list ap;
    va_start(ap, arg0);
    func_801ED9F0(arg0 & 0xFFFF);
    va_end(ap);
}
