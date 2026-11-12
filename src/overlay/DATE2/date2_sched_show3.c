/* @CompileFlags: -Wo,-loopunroll,0 -Wo,-notail */
/* @PostProc: patch_stack_complete --old 144 --new 160 */
/* @PostProc: remap_stack_slots --from 64 --to 76 */
/* @PostProc: remap_stack_slots --from 44 --to 60 */
/* date2_sched_show3 - DATE2 overlay, Address: 0x80139180, Size: 532 bytes (133 insns) */
#include "types.h"

typedef struct {
    s16 h[39];
} Sched39;

extern s32 set_kanji_string();
extern s32 strlen(s32);
extern void k_disp_start(s32);
extern void k_reset(s32);
extern void date2_sched_menu_disp(s32);

extern Sched39 date2_sched_show3_tbl;
extern s16 date2_xa_timer;
extern s32 *date2_sched3_str_tbl;

void date2_sched_show3(void)
{
    Sched39 buf;
    s16 v1;
    register s32 found;
    s32 i;
    s32 r;
    s32 r2;
    s32 r3;

    buf = date2_sched_show3_tbl;
    v1 = date2_xa_timer;
    found = 255;
    for (i = 0; i < 39; i = (s16)(i + 1)) {
        if (!(v1 < buf.h[i])) {
            found = (s16)i;
        }
    }

    if (v1 == 1200) {
        set_kanji_string(-116, 54, 0, date2_sched3_str_tbl[0], 0);
        r = strlen(date2_sched3_str_tbl[0]);
        set_kanji_string((s16)(r * 7 - 116), 54, 0, date2_sched3_str_tbl[1], 0);
        r2 = strlen(date2_sched3_str_tbl[0]);
        r3 = strlen(date2_sched3_str_tbl[1]);
        r = set_kanji_string((s16)(r3 * 7 + r2 * 7 - 116), 54, 0,
                          date2_sched3_str_tbl[2], 0);
        k_disp_start(r);
    }

    v1 = date2_xa_timer;
    if (found != 255) {
        if (v1 == buf.h[found]) {
            k_reset(1);
            date2_sched_menu_disp((s16)found);
            v1 = date2_xa_timer;
        }
    }

    if (v1 == 5400) {
        k_reset(1);
    }
}
