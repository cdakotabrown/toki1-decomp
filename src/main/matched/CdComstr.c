/* Address: 0x80087914 */
/* Size: 52 bytes (13 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc260 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern char *CD_comstr[];
extern char libcd_none_str[];

char *CdComstr(u8 n) {
    if (n >= 28) {
        return libcd_none_str;
    }
    return CD_comstr[n];
}
