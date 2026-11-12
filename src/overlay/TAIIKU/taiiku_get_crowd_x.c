/* Address: 0x80135E68 */
/* Size: 212 bytes (53 insns) */
/* @PostProc: patch_stack_complete --old 264 --new 272 */
/* @PostProc: remap_stack_slots --from 32 --to 268 */
#include "types.h"

typedef struct {
    s16 v[11][10];
} D_80149760_t;

extern D_80149760_t taiiku_get_crowd_x_tbl;
extern s16 taiiku_kari_card;

extern s32 rand(void);

s16 taiiku_get_crowd_x(void)
{
    D_80149760_t buf;
    s32 v0;
    s32 rem;
    s32 a0;

    buf = taiiku_get_crowd_x_tbl;

    v0 = rand();
    rem = v0 % 11;
    a0 = rem;
    if (rem >= taiiku_kari_card) {
        a0 = rem + 1;
    }

    if (a0 >= 11) {
        v0 = rand();
        a0 = v0 % 10;
    }

    v0 = rand();
    rem = v0 % 10;

    return buf.v[a0][rem];
}
