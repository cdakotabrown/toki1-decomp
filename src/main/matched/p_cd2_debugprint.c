/* Function: p_cd2_debugprint */
/* Address: 0x8004658C */
/* Size: 112 bytes (28 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s32 cd2_debug_val[10];
extern void _debug_number_show(s32 a0, s32 a1, s32 a2, s32 a3);

void p_cd2_debugprint(s32 arg0, s32 arg1) {
    s32 *ptr;
    s32 i;

    if (arg1 < 10) {
        cd2_debug_val[arg1] = arg0;
        return;
    }

    ptr = cd2_debug_val;
    i = -50;
    do {
        _debug_number_show(*ptr, 150, i, 6);
        i += 9;
        ptr++;
    } while (i != -41);
}
