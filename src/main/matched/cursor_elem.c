/* Address: 0x800627D4, Size: 52 bytes */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"
extern s32 work_fuku;
extern u8  class_w[];
void cursor_elem(void) {
    s32 idx = work_fuku;
    u8 *p = &class_w[idx * 68 + 6599];
    *p = (u8)(*p | 0x40);
}
