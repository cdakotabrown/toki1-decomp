/* Address: 0x8006EAE4 */
/* Size: 292 bytes (73 instructions) */
/* Region: Game (0x80041000-0x80086000) */
/* Compiler: IDO 5.2 */
/* @CompileEnv: IDO52_UGEN_TEMP_REG=2:t8,3:t9,6:t3,4:t0,5:t1,8:t4 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: remap_stack_slots --map '56:44,88:76' */
/* @PostProc: patch_insn_reg_field 0x64:rt:t3:t2 0x68:rs:t3:t2 0x68:rt:t3:t2 0x70:rs:t3:t2 0x74:rt:t7:t3 0x78:rs:t7:t3 0x78:rt:t7:t3 0x80:rs:t7:t3 0x9c:rt:t2:t5 0xa0:rs:t2:t5 0xa0:rt:t2:t5 0xa8:rd:t5:t6 0xa8:rt:t2:t5 0xac:rd:t6:t7 0xac:rt:t5:t6 0xb0:rs:t6:t7 0xe4:rd:t1:t2 0xe8:rs:t1:t2 */
#include "types.h"

extern u8  screen_w[];
extern u8  now_year;
extern u8  now_month;
extern u8  now_day;
extern s32 girl11_flag;
extern u8  ev_flag4c;
extern u8  ev_flag4e;
extern s32 reserved_date;

extern void get_grp(u8 *out, s16 *clr);
extern s32 rand(void);

s32 final_year_event_check(void)
{
    u8 out[16];
    s16 clr[16];
    s32 cnt;
    s32 i;
    u8 *p;

    get_grp(out, clr);
    cnt = 0;
    p = screen_w;
    for (i = 0; i < 11; i++) {
        if ((*(s32 *)(p + 0x1C8) << 30) < 0) {
            if (clr[i] >= 501) {
                cnt++;
            }
        }
        p += 0x38;
    }
    if (now_year != 0x62) {
        goto ret0;
    }
    if (now_month != 2) {
        goto ret0;
    }
    if (now_day != 15) {
        goto ret0;
    }
    if (cnt < 2) {
        goto ret0;
    }
    if ((u32)(girl11_flag << 20) >> 29 < 4) {
        goto ret0;
    }
    if (ev_flag4c == 0) {
        goto ret0;
    }
    if (ev_flag4e == 0) {
        goto ret0;
    }
    if ((reserved_date << 2) < 0) {
        goto ret0;
    }
    if ((rand() & 0xF) >= 5) {
        goto ret0;
    }
    return 1;
ret0:
    return 0;
}
