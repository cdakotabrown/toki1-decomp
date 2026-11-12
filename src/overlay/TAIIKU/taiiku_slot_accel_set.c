/* @SymbolStart: 0x8014723C */
/* @SymbolSize: 128 */
/* Address: 0x80147248 (real start 0x8014723C; cross-prologue head loads the */
#include "types.h"

typedef struct { s32 v[3]; } S3;
typedef struct { u8 pad0[12]; u8 b12; u8 pad13[3]; s32 w16; u8 pad14[8]; } Rec;

extern S3 taiiku_slot_accel_set_tbl;
extern Rec taiiku_spoon_cpu[];
extern Rec taiiku_spoon_bal[];

void taiiku_slot_accel_set(void) {
    s32 i;
    S3 local;

    local = taiiku_slot_accel_set_tbl;
    for (i = 0; i < 3; i++) {
        if (taiiku_spoon_bal[i + 1].b12 != 0) {
            taiiku_spoon_cpu[i].w16 = -16384;
        } else {
            taiiku_spoon_cpu[i].w16 = local.v[i];
        }
    }
}
