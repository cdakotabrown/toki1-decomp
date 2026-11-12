/* @CompileFlags: -Wo,-loopunroll,0 -Wo,-notail */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=6:s0 */
/* date2_sched_show2 - DATE2 overlay, Address: 0x80138DEC, Size: 384 bytes (96 insns) */
/* @SymbolSize: 384 */
#include "types.h"

typedef struct {
    s16 h[30];
} Sched60;

extern s32 set_kanji_string();
extern s32 strlen(s32);
extern void k_disp_start(s32);
extern void k_reset(s32);
extern void date2_sched_menu_disp(s32);

extern Sched60 date2_sched_show2_tbl;
extern s16 date2_xa_timer;
extern s32 *date2_sched2_str_tbl;

void date2_sched_show2(void)
{
    s16 v1;
    Sched60 buf;
    register s32 found;
    s16 i;
    s32 r;

    buf = date2_sched_show2_tbl;
    v1 = date2_xa_timer;
    found = 255;
    for (i = 0; i < 30; i++) {
        if (v1 >= buf.h[i]) {
            found = (s16)(s32)i;
        }
    }

    if (v1 == 200) {
        set_kanji_string(-116, 54, 0, date2_sched2_str_tbl[0], 0);
        r = strlen(date2_sched2_str_tbl[0]);
        r = set_kanji_string((s16)(r * 7 - 116), 54, 0, date2_sched2_str_tbl[1], 0);
        k_disp_start(r);
    }

    v1 = date2_xa_timer;
    if (found != 255) {
        if (v1 == buf.h[found]) {
            k_reset(1);
            date2_sched_menu_disp((s16)found);
        }
    }

    v1 = date2_xa_timer;
    if (v1 == 3540) {
        k_reset(1);
    }
}
