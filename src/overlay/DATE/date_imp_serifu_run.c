/* @SymbolStart: 0x8013B114 */
/* @SymbolSize: 152 */
/* @PostProc: scheduler_normalizer */
/* Function: date_imp_serifu_run */
/* Overlay: DATE */
/* Address: 0x8013B114 (cross-prologue head) / 0x8013B11C (body) */
/* Size: 152 bytes (38 instructions) */
#include "types.h"

extern s32 serifu_main(s32, s32, s32);
extern void sub_sub_step_up(void);

extern s32 serifu_no;
extern s16 imp[];

extern volatile s32 date_serifu0_tbl_2;
extern volatile s32 date_serifu0_kind_tbl_2;
extern volatile s32 date_sound0_tbl_2;

extern volatile s32 now_s;
extern volatile s32 now_s_in;
extern volatile s32 ss_s;
extern volatile s32 ss_s_kind;
extern volatile s32 sss_so;

void date_imp_serifu_run(void) {
    s16 buf;
    s32 v0;

    v0 = serifu_no;
    now_s = (s32)&buf;
    now_s_in = (s32)&imp[v0];
    ss_s = date_serifu0_tbl_2;
    ss_s_kind = date_serifu0_kind_tbl_2;
    sss_so = date_sound0_tbl_2;
    buf = 0;
    if (serifu_main(v0 & 0xFF, 0, 0) != 0) {
        sub_sub_step_up();
    }
}
