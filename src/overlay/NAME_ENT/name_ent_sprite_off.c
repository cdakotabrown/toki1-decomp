/* Address: 0x80142A20 */
/* Size: 60 bytes (15 insns) */
#include "types.h"

typedef struct {
    s32 f0;
    u8 pad[32];
} Ent;

extern Ent Sprites[];

void name_ent_sprite_off(void) {
    s32 i;

    for (i = 0; i < 13; i++) { Sprites[i].f0 = 0x80000000; }
}
