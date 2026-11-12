/* Address: 0x8013EA30 */
/* Size: 48 bytes (12 insns) */
#include "types.h"

typedef struct {
    u8  pad0[7];
    u8  f7;
    u8  pad8[60];
} Rec;

extern Rec class_w[];

void rpg_bat_sprite_bri_set(s32 v) {
    s32 i;

    for (i = 0; i < 96; i++) {
        class_w[i + 64].f7 = v;
    }
}
