/* @SymbolSize: 172 */
/* Address: 0x80143ED0 */
/* Size: 172 bytes (43 insns) */
#include "types.h"
#include "tokimeki/taiiku_ninin_run.h"

extern s16 taiiku_ninin_scrl_hi;
extern s32 taiiku_ninin_scrv;
extern s32 taiiku_ninin_vmax;
extern volatile s16 taiiku_ninin_run[];
extern s16 cal_cw_x;

#define taiiku_ninin_run_0_pos_int taiiku_ninin_run[11]

void taiiku_race_scroll_publish(void)
{
    s16 v;

    if (((TAIIKU_NININ_RUN *)taiiku_ninin_run)[0].accel < 0) {
        goto ef8;
    }
    if (taiiku_ninin_scrl_hi != 0) {
        goto f20;
    }
ef8:
    if (taiiku_ninin_scrv != 0) {
        ((TAIIKU_NININ_RUN *)taiiku_ninin_run)[0].pos.fix -= taiiku_ninin_scrv;
    }
    goto clamp;
f20:
    ((TAIIKU_NININ_RUN *)taiiku_ninin_run)[0].pos.fix += taiiku_ninin_vmax >> 2;
clamp:
    v = taiiku_ninin_run_0_pos_int;
    if (v >= -31) {
        taiiku_ninin_run_0_pos_int = -32;
        v = taiiku_ninin_run_0_pos_int;
    }
    if (v < -159) {
        taiiku_ninin_run_0_pos_int = -160;
        v = taiiku_ninin_run_0_pos_int;
    }
    cal_cw_x = v;
}
