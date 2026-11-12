/* Address: 0x8013E97C */
/* Size: 44 bytes (11 insns) */
#include "types.h"

typedef struct {
    u8  pad0[38];
    s16 f38;
    u8  pad40[2];
    s16 f42;
    u8  pad44[24];
} Rec;

extern Rec class_w[];

void rpg_bat_sprite_pos_set(s32 idx, s32 x, s32 y) {
    Rec *r;

    r = &class_w[idx];
    r[96].f38 = x - 160;
    r[96].f42 = y - 120;
}
