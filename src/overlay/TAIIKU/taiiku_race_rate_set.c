/* @SymbolSize: 60 */
/* Address: 0x80143E30 */
/* Size: 60 bytes (15 insns) incl. the alt-entry tail at 0x80143E54 */
#include "types.h"
#include "tokimeki/taiiku_ninin_run.h"

extern s32 taiiku_ninin_gain;
extern TAIIKU_NININ_RUN taiiku_ninin_run[];

void taiiku_race_rate_set(void)
{
    if (taiiku_ninin_run[0].run_flag != 0) {
        taiiku_ninin_run[0].accel = taiiku_ninin_gain;
    } else {
        taiiku_ninin_run[0].accel = -taiiku_ninin_gain;
    }
}
