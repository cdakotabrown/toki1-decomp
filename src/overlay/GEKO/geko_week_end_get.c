/* @CompileFlags: -Wc,-pic0 */
/* @CompileEnv: IDO52_COPYPROP_VAR=-4,-44 IDO52_FORCE_WEB_COLOR=0:a2,21:a3,23:t0,29:v1,38:t1,40:a1,43:a0 */
/* geko_week_end_get -- GEKO overlay, Address: 0x801440C8 */
/* @SymbolSize: 216 */
/* @PostProc: patch_insn_reg_field 0x28:rt:t2:t1 0x2c:rs:t2:t1 0x38:rs:t2:t1 0x44:rs:t2:t1 0x4c:rt:t8:t3 0x50:rs:t2:t1 0x58:rt:t7:t4 0x5c:rs:t2:t1 0x6c:rs:t2:t1 0x70:rt:t3:t2 0x74:rt:t3:t2 0x7c:rs:t8:t3 0x7c:rt:t8:t3 0x80:rs:t7:t4 0x80:rt:t7:t4 0x84:rd:t4:t5 0x88:rd:t5:t8 0x88:rs:t2:t1 0x88:rt:t4:t5 0x8c:rs:t5:t8 0x90:rd:t1:v1 0x90:rs:t8:t3 0x90:rt:t7:t4 0x94:rs:t1:v1 */
#include "types.h"

typedef struct {
    s16 h[12];
} GekoBuf24;

extern GekoBuf24 geko_week_end_get_tbl;
extern volatile u8 now_year;
extern u8 now_month;
extern u8 now_day;
extern u8 now_youbi;

s32 geko_week_end_get(s32 mode)
{
    s32 m;
    s32 state;
    s32 idx;
    s32 cur;
    GekoBuf24 buf;
    s32 thr;
    s32 ra0;
    s32 ra1;
    s32 pad;

    m = mode;
    state = now_year;
    buf = geko_week_end_get_tbl;
    if (state == 96) {
        buf.h[1] = 29;
    }
    idx = now_month;
    cur = buf.h[idx - 1];
    thr = ((s32)now_day - (s32)now_youbi) + 6;
    ra1 = state;
    ra0 = idx;
    if (cur < thr) {
        ra0 = idx + 1;
        thr = thr - cur;
        if (ra0 >= 13) {
            ra0 = 1;
            ra1 = state + 1;
        }
    }
    if (m == 0) {
        return ra1;
    }
    if (m == 1) {
        return ra0;
    }
    return thr;
}
