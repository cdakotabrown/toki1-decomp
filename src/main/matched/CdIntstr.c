/* Address: 0x80087948 */
/* Size: 52 bytes (13 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc260 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern char *CD_intstr[];
extern char libcd_none_str[];

char *CdIntstr(u8 n) {
    if (n >= 7) {
        return libcd_none_str;
    }
    return CD_intstr[n];
}
