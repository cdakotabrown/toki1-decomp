/* Address: 0x801475B8, Size: 124 bytes (31 insns) */
/* @PostProc: swap_registers s2:s3 --bidirectional --exclude-offsets 0x1c,0x20,0x7c,0x80 */
#include "types.h"

extern void tt_ent_move(s32 entry);

extern s32 tt_ent_buf;

typedef struct {
    u8 b0;
    u8 pad1;
    u16 pad2;
    u16 h4;

} TtEntry;

void tt_ent_step_all(void)
{
    s32 i, max, expected;
    s32 entry;

    entry = tt_ent_buf;
    i = 0;
    expected = 64;
    max = 6;
    do {
        if (((TtEntry *)entry)->b0 != 0) {
            if (((TtEntry *)entry)->h4 == expected) {
                tt_ent_move(entry);
            }
        }
        i += 1;
        entry += 104;
    } while (i != max);
}
