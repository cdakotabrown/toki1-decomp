/* Address: 0x80133BA4 */
/* Size: 60 bytes (15 insns) */
#include "types.h"

u8 *get_tt_shot_free(u8 *p, u8 *end) {
    u8 *found;

    found = 0;
    while (p < end) {
        if (*p == 0) {
            return p;
        }
        p += 104;
    }
    return found;
}
