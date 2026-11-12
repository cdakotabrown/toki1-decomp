/* Address: 0x80144C98 */
/* Size: 72 bytes (18 insns) */
#include "types.h"

typedef struct {
    u8 b0;
    u8 pad[99];
} S100;

extern u8   *tt_scroll_wk;
extern S100 *tt_anim_buf;

void tt_sprite_clear(void) {
    u8 *st;
    S100 *rec;
    u32 i;

    st = tt_scroll_wk;
    i = 0;
    rec = tt_anim_buf + 1;
    for (; i < (u32)(2 - st[12]); i++) {
        rec->b0 = 0;
        rec--;
    }
}
