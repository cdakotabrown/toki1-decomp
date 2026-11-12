/* @SymbolStart: 0x80143388 */
/* @SymbolSize: 224 */
/* @PostProc: patch_insn_reg_field 0x70:rt:v1:t9 0x74:rs:v1:t9 0x74:rt:v1:t9 0x78:rt:a1:a0 0x7c:rs:v1:t9 0x7c:rt:a1:a0 0x80:rt:a0:v1 0x84:rs:a0:v1 0x84:rt:a0:v1 0x88:rt:v1:t3 0x8c:rd:t9:t0 0x90:rs:a0:v1 0x90:rt:t9:t0 0x94:rt:t0:t1 0x9c:rs:t0:t1 0x9c:rt:t1:t2 0xa0:rt:t1:t2 0xa4:rs:v1:t3 0xa4:rt:v1:t3 0xac:rs:v1:t3 0xac:rt:a1:a0 0xb0:rd:t2:t4 0xb4:rs:a0:v1 0xb4:rt:t2:t4 0xb8:rt:t3:t5 0xc0:rs:t3:t5 0xc0:rt:t4:t6 0xc8:rt:t4:t6 */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* shougatu_serifuev_set - SHOUGATU overlay, Address: 0x8014338C (cross-prologue head */
#include "types.h"

extern s16 now_serifuev;
extern s16 now_serifuev_in;
extern volatile u8 now_girl;
extern s32 shougatu_ss_serifuev;
extern s32 shougatu_ss_serifuev_kind;
extern s32 shougatu_sss_soundev;
extern u8 screen_w[][56];
extern u8 ba_string[];
extern u8 shougatu_serifuev_set_str0[];
extern s32 shougatu_serifuev_tbl[];
extern s32 shougatu_serifuev_kind_tbl[];
extern s32 shougatu_soundev_tbl[];

extern void date_name_set(s32 arg0);
extern void shougatu_ptrs3_set(void);
extern void sub_sub_step_up(void);
extern void param_clamp(void);
extern s32 strcpy(u8 *arg0, u8 *arg1);

void shougatu_serifuev_set(void)
{
    now_serifuev = 0;
    now_serifuev_in = 0;
    date_name_set(now_girl);
    shougatu_ptrs3_set();
    shougatu_ss_serifuev = shougatu_serifuev_tbl[60];
    shougatu_ss_serifuev_kind = shougatu_serifuev_kind_tbl[60];
    shougatu_sss_soundev = shougatu_soundev_tbl[60];
    sub_sub_step_up();
    *(s16 *)(screen_w[now_girl] + 446) += 1;
    *(s16 *)(screen_w[now_girl] + 450) += 2;
    param_clamp();
    strcpy(ba_string, shougatu_serifuev_set_str0);
}
