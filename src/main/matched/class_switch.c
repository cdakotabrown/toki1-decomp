/* Address: 0x80049038 */
/* Size: 24 bytes (6 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @SymbolSize: 36 */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern u8 class_flag;

void class_switch(s32 a0) {
    if (a0 != 0) {
        class_flag = 1;
    } else {
        class_flag = 0;
    }
}
