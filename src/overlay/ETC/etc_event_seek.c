/* @SymbolSize: 208 */
/* @CompileEnv: IDO52_AS1_XBB_VOLATILE_OK=all */
/* Address: 0x8013EAB8, 200 bytes (50 insns). Hidden t6 selector (a0->t6). */
#include "types.h"

extern void etc_hizuke_set(void);
extern s32 etc_rec_year;
extern s32 etc_rec_month;
extern s32 etc_rec_day;
extern volatile s32 etc_record_index;

s32 etc_event_seek(void)
{
    s32 sel = etc_rec_year;

    s32 v0;

    if (sel != 0) {
        goto ret1;
    }
    if (etc_rec_month != 0) {
        goto ret1;
    }
    if (etc_rec_day != 0) {
        goto ret1;
    }
loop:
    v0 = etc_record_index;
    if (v0 < 255) {
        v0++;
        etc_record_index = v0;
    }
    etc_hizuke_set();
    if (etc_rec_year == 0) {
        goto reload;
    }
    if (etc_rec_month == 0) {
        goto reload;
    }
    if (etc_rec_day == 0) {
        goto reload;
    }
    return 1;
reload:
    v0 = etc_record_index;
    if (v0 == 255) {
        return 0;
    }
    etc_record_index = v0;
    goto loop;
ret1:
    return 1;
}
