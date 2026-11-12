/* @PostProc: patch_stack_complete --old 64 --new 72 */
/* @PostProc: remap_stack_slots --from 36 --to 40 --from 45 --to 49 */
/* Address: 0x80134408 */
/* Size: 340 bytes (85 instructions) */
#include "types.h"

typedef struct {
    u8 d[27];
} Blk27;

extern void sub_sub_step_up(void);
extern void param_clamp(void);
extern void strcpy(void *, void *);

extern Blk27 shougatu_taku_set_tbl;
extern s32 serifu_no;
extern u8 wname[];
extern s16 shougatu_meet_now_serifu;
extern u8 shougatu_taku_no;
extern u8 shougatu_alone_flag;
extern u8 girl_no;
extern s16 para_tai;
extern s16 para_tai_a[];
extern u8 now_girl;

void shougatu_taku_set(void)
{
    Blk27 buf;
    s32 v;
    s32 t;

    buf = shougatu_taku_set_tbl;
    v = serifu_no;
    t = shougatu_meet_now_serifu;
    shougatu_taku_no = (u8)v;
    shougatu_meet_now_serifu = (s16)(t + v);
    strcpy(wname, &buf.d[v * 9]);
    if (shougatu_alone_flag == 0 && girl_no == 3 && serifu_no == 2) {
        strcpy(wname, &buf.d[9]);
    }
    v = serifu_no;
    if (v == 0) {
        para_tai_a[2] = (s16)(para_tai_a[2] + 10);
        para_tai_a[4] = (s16)(para_tai_a[4] + 10);
        now_girl = 14;
    } else if (v == 1) {
        para_tai = (s16)(para_tai + 25);
        now_girl = 14;
    }
    param_clamp();
    sub_sub_step_up();
}
