/* Address: 0x80146D28 */
/* Size: 148 bytes (37 insns) */
/* @SymbolSize: 148 */
#include "types.h"

extern s32 taiiku_spoon_scrv;
struct { u8 unk0; u8 pad[3]; u32 unk4; s32 unk8; } taiiku_spoon_pl = {0};
extern s32 taiiku_spoon_pl_acc;
extern s32 taiiku_spoon_pl_spd;

void taiiku_pose_advance(void) {
    u32 pace;
    u8 pose;

    if (taiiku_spoon_scrv == 0) {
        taiiku_spoon_pl.unk0 = 0;
        return;
    }

    pace = taiiku_spoon_pl.unk4;
    if ((pace >= 6 && taiiku_spoon_pl_acc > 0) || (pace >= 11 && taiiku_spoon_pl_spd != 0)) {
        pose = taiiku_spoon_pl.unk0;
        if ((u32)pose >= 7) {
            taiiku_spoon_pl.unk0 = 2;
            taiiku_spoon_pl.unk4 = 0;
            return;
        }
        taiiku_spoon_pl.unk0 = taiiku_spoon_pl.unk0 + 1;
        taiiku_spoon_pl.unk4 = 0;
    }
}
