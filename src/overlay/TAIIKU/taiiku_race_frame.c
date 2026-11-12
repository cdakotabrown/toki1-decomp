/* Address: 0x80144914 */
/* Size: 204 bytes (51 insns) */
/* @SymbolSize: 204 */
/* @PostProc: patch_stack_complete --old 48 --new 64 */
/* @PostProc: remap_stack_slots --from 40 --to 52 --from 44 --to 56 */
#include "types.h"

typedef struct { u16 h[3]; } Rec6;

extern Rec6 taiiku_race_frame_tbl;
extern u8 taiiku_ninin_run[][36];
extern u8 class_w[][68];

extern void taiiku_race_accel_set(void);
extern void taiiku_race_speed_set(void);
extern void taiiku_race_approach(void);
extern void taiiku_anim_step(void);
extern void taiiku_race_stumble(void);

void taiiku_race_frame(void) {
    Rec6 buf;
    s32 i;

    buf = taiiku_race_frame_tbl;

    for (i = 0; i < 3; i++) {
        if ((u32)*(u16 *)(taiiku_ninin_run[i] + 60) >= 51U) {
            if ((i + 54) == *(s16 *)(class_w[i] + 6620)) {
                *(s16 *)(class_w[i] + 6620) = ((s16 *)&buf)[i];
            }
        }
    }

    taiiku_race_accel_set();
    taiiku_race_speed_set();
    taiiku_race_approach();
    taiiku_anim_step();
    taiiku_race_stumble();
}
