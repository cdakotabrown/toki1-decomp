/* date_basyo12_bg_step41 -- DATE overlay, Address: 0x8013801C */
/* @CompileEnv: IDO52_CONST_INREG_VALUES=1,3,2:-1:2:4,2:-1:6:8 */
/* @PostProc: swap_instructions --offset1 0xb4 --offset2 0xb8 */
/* @PostProc: patch_insn_reg_field 0x5c:rt:v0:t8 0x64:rt:v0:t8 0x6c:rt:v0:t8 0x84:rt:v0:t8 0x8c:rt:t8:t0 0x90:rs:t8:t0 0x90:rt:t8:t0 0x9c:rs:t8:t0 0xa0:rt:v0:t1 0xa8:rt:v0:t1 0xb0:rt:v0:t1 0xb8:rt:v1:t2 0xbc:rt:v1:t2 0xc4:rt:v1:t2 0xcc:rt:v0:t1 0xd4:rt:v1:t2 */
#include "types.h"

extern void strcpy(void *a, void *b);
extern void device_cmd_send(s32 a);
extern void dec_bg_cache_step(s32 a);
extern void event_log_add(s32 a, s32 b);
extern void sub_sub_step_up(void);

extern u8 ba_string2[];
extern u8 date_basyo12_bg_step41_str0[];
extern u8 rec_idx;
extern u8 now_girl;
extern s16 kao2[];
extern s16 ans[];

void date_basyo12_bg_step41(void)
{
    strcpy(ba_string2, date_basyo12_bg_step41_str0);
    device_cmd_send(516);
    dec_bg_cache_step(17259);
    rec_idx = rec_idx - 1;
    event_log_add(0, 26);
    kao2[1] = 1;
    kao2[0] = 1;
    (*(u16 *)&ans[1]) = 0;
    (*(u16 *)&ans[0]) = 0;
    kao2[2] = 1;
    (*(u16 *)&ans[2]) = 0;
    if (now_girl == 2) {
        kao2[1] = 3;
        kao2[0] = 3;
        (*(u16 *)&ans[1]) = 2;
        (*(u16 *)&ans[0]) = 2;
        kao2[2] = 3;
        (*(u16 *)&ans[2]) = 2;
    }
    sub_sub_step_up();
}
