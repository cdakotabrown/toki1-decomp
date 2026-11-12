/* @SymbolStart: 0x8013D998 */
/* @SymbolSize: 500 */
/* Address: 0x8013D998 (window includes the absorbed cross-prologue head; */
/* Size: 492 bytes (123 insns; 125 with the absorbed cross-prologue head) */
/* the compare window covers via `@SymbolStart: 0x8013D998` / `@SymbolSize: 500` */
#include "types.h"

typedef union {
    s32 w;
    s16 h[2];
} Fix32;

extern s16 para_un;
extern s32 taiiku_tama_girl;
extern s32 taiiku_tama_spd;
extern Fix32 taiiku_tama_gscrl;
extern s32 taiiku_tama_dist;
extern s16 taiiku_tama_spd_tick;
extern s32 taiiku_tama_push[];
extern s32 taiiku_tama_ppush[];

extern void taiiku_ground_disp(void);
extern s32 debug_hex_show();

void taiiku_run_speed(void)
{
    s32 v;
    s32 lim;

    v = para_un;
    if (v >= 160) {
        lim = 0x18000;
    } else if (v >= 100) {
        lim = 0x16000;
    } else if (v >= 75) {
        lim = 0x14000;
    } else if (v >= 50) {
        lim = 0x12000;
    } else {
        lim = 0x10000;
    }
    if (taiiku_tama_girl == 2) {
        if (v >= 400) {
            lim = 0x18000;
        } else if (v >= 300) {
            lim = 0x16000;
        } else if (v >= 200) {
            lim = 0x14000;
        } else if (v >= 100) {
            lim = 0x12000;
        } else {
            lim = 0x10000;
        }
    }
    if (taiiku_tama_spd_tick >= 11) {
        if (taiiku_tama_push[1] > 0) {
            taiiku_tama_spd = taiiku_tama_spd + 8192;
        } else if (taiiku_tama_spd >= 0x10001) {
            taiiku_tama_spd = taiiku_tama_spd - 16384;
        } else {
            taiiku_tama_spd = taiiku_tama_spd - 6144;
        }
        if (taiiku_tama_spd >= 0xE001) {
            if (taiiku_tama_ppush[1] <= 0) {
                taiiku_tama_spd = taiiku_tama_spd - 16384;
                if (taiiku_tama_spd < 0xE000) {
                    taiiku_tama_spd = 0xE000;
                }
            }
        }
        if (lim < taiiku_tama_spd) {
            taiiku_tama_spd = lim;
        }
        if (taiiku_tama_spd < 0) {
            taiiku_tama_spd = 0;
        }
        taiiku_tama_spd_tick = 0;
    }
    taiiku_tama_gscrl.w = taiiku_tama_gscrl.w - taiiku_tama_spd;
    if (taiiku_tama_gscrl.h[1] < 0) {
        taiiku_tama_gscrl.h[1] = 128;
    }
    taiiku_ground_disp();
    debug_hex_show(taiiku_tama_spd, 0, -100);
    taiiku_tama_dist = taiiku_tama_dist + taiiku_tama_spd;
}
