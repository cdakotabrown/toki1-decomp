/* @SymbolStart: 0x80143DB0 */
/* @SymbolSize: 128 */
/* Address: 0x80143DB0, Size: 128 bytes (32 insns) */
#include "types.h"
#include "tokimeki/taiiku_ninin_run.h"

extern s16 cw100_frame;
extern TAIIKU_NININ_RUN taiiku_ninin_run[];

extern void taiiku_race_rate_set(void);
extern void taiiku_race_scroll_accum(void);
extern void taiiku_race_scroll_publish(void);
extern void taiiku_race_pose_step(void);
extern void taiiku_race_input(void);

void taiiku_race_run(void)
{
    s32 v0_hidden = cw100_frame;

    if (v0_hidden != 45) {
        if (v0_hidden != 57) {
            taiiku_race_rate_set();
            taiiku_race_scroll_accum();
            taiiku_race_scroll_publish();
            taiiku_race_pose_step();
            taiiku_race_input();
            return;
        }
    }
    if ((u32)taiiku_ninin_run[0].tick_a >= 50U) {
        cw100_frame = 47;
    }
}
