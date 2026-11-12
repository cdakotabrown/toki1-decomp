/* Address: 0x80046544, Size: 16 bytes (4 instructions) */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern u32 cdq_result;

u32 cdq_result_word_get(void) {
    return cdq_result;
}
