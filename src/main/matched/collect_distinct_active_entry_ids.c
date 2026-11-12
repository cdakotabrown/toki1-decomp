/* Address: 0x80076544 (true start 0x8007653C: the record-0 load is hoisted */
/* Size: 388 bytes over the absorbed head (97 instructions) */
/* Region: Game (IDO 5.2) */
/* @SymbolStart: 0x8007653C */
/* @SymbolSize: 388 */
/* @CompileFlags: -Wc,-pic1 -cckr */
/* @CompileEnv: IDO52_UGEN_TEMP_REG=4:t2,5:t3,6:t4,7:t5,8:t6,9:t7,10:t8 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    u32 id : 5;
    u32 pad : 24;
    u32 on : 1;
    u32 rest : 2;
} Rec;

typedef struct {
    u8 pad0[1356];
    Rec rec[8];
} Save;

extern Save screen_w;

s32 collect_distinct_active_entry_ids(out)
    s32 *out;
{
    s32 id;
    s32 cnt[32];
    s32 i;
    s32 n;
    s32 r;
    u32 w;

    switch (screen_w.rec[0].id) {
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
        return 1;
    }
    for (i = 0; i < 32; i++) {
        cnt[i] = 0;
    }
    for (i = 0; i < 8; i++) {
        w = *(u32 *)&screen_w.rec[i];
        if ((s32)(w << 2) >= 0) {
            break;
        }
        cnt[w & 0x1F]++;
    }
    cnt[16] = 0;
    cnt[17] = 0;
    cnt[19] = 0;
    cnt[20] = 0;
    cnt[18] = 0;
    n = 0;
    r = 0;
    for (i = 0; i < 32; i++) {
        if (cnt[i] != 0) {
            n++;
        }
    }
    if (n < 2) {
        *out = screen_w.rec[0].on;
        return 1;
    }
    for (i = 0; i < 8; i++) {
        if (!screen_w.rec[i].on) {
            break;
        }
        id = screen_w.rec[i].id;
        if (cnt[id] != 0) {
            out[r] = id;
            r++;
            cnt[screen_w.rec[i].id] = 0;
            if (r >= 3) {
                break;
            }
        }
    }
    return n;
}
