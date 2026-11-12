/* Address: 0x8013F2F4 */
/* Size: 316 bytes (79 instructions) */
/* @SymbolSize: 316 */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:10 */
/* @PostProc: patch_stack_complete --old 144 --new 152 */
/* @PostProc: remap_stack_slots --from 36 --to 40 */
#include "types.h"

extern s32 shougatu_pair_disp(void);
extern s32 shougatu_phase_set(void);

typedef struct { s32 w[27]; } Tbl108;
extern Tbl108 shougatu_main3_tbl;

extern u8 now_sub_sub_step;
extern u8 fade_bri;

typedef struct {
    u8 pad0[7];
    u8 b37;
    s16 b38;
    u8 pad1[12];
    volatile s16 b46;
    s16 b48;
} Blk30;
extern Blk30 cw96_active;
extern volatile u8 shougatu_pair_disp_on;
extern u8 shougatu_phase_on;

s32 shougatu_case3_pair_phase_step(void)
{
    Tbl108 buf;
    u32 idx;
    u32 v1;
    s32 cnt;

    buf = shougatu_main3_tbl;
    idx = now_sub_sub_step;
    cnt = ((s32 (*)(s32))buf.w[idx])(128);

    v1 = now_sub_sub_step;
    if (v1 < 8U) {
        cw96_active.b37 = fade_bri;
        if (cw96_active.b48 == 0 && cw96_active.b38 == 0) {
            cnt = (s16)(cw96_active.b46 + 1);
            cw96_active.b46 = cnt;
            if (cnt >= 3) {
                cw96_active.b46 = 0;
                cnt = 0;
            }
        }
    }
    if (v1 >= 10U && shougatu_pair_disp_on != 0) {
        cnt = shougatu_pair_disp();
        v1 = now_sub_sub_step;
    }
    if (v1 >= 10U && shougatu_phase_on != 0) {
        cnt = shougatu_phase_set();
    }
    return cnt;
}
