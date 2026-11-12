/* @SymbolStart: 0x80133374 */
/* @SymbolSize: 280 */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @PostProc: swap_instructions --offset1 0x48 --offset2 0x4c */
/* @PostProc: swap_instructions --offset1 0x74 --offset2 0x7c */
/* @PostProc: patch_insn_reg_field 0x50:rt:t0:t2 0x54:rt:t2:t4 0x58:rt:t4:t6 0x5c:rt:t6:t0 0x64:rs:t0:t2 0x64:rt:t1:t3 0x68:rs:t2:t4 0x68:rt:t3:t5 0x6c:rs:t4:t6 0x6c:rt:t5:t7 0x70:rs:t6:t0 0x70:rt:t7:t1 0x74:rt:t5:t7 0x78:rt:t3:t5 0x7c:rt:t1:t3 0x84:rt:t7:t1 0xdc:rt:t6:t7 0xe4:rs:t6:t7 0xe4:rt:t7:t8 0xec:rt:t7:t8 */
/* Function: master_basyo_return */
/* Overlay: MASTER */
#include "types.h"

extern s32 shared_system_state_reset();
extern s32 dec_bg_reset();
extern s32 set_dec_bri();
extern s32 master_log_basyo_add();
extern s32 return_schedule();
extern s32 common_disp();

extern s16 back_clear_b[][28];
extern s16 girl5_blk_end[][28];
extern s16 screen_w[][28];
extern u16 basyo_pack;
extern s32 master_basyo_girl_no;

void master_basyo_return(void)
{
    s16 (*p)[28];
    s32 v1;
    u32 idx;
    u8 *bp;

    ((u8 *)screen_w)[0x71E] |= 0x10;
    screen_w[0][223] += 20;
    p = back_clear_b;
    do {
        p[1][223] += 20;
        p[2][223] += 20;
        p[3][223] += 20;
        p[0][223] += 20;
        p += 4;
    } while (p != girl5_blk_end);
    v1 = master_basyo_girl_no;
    if (v1 != 255) {
        screen_w[v1][225] += 2;
    }
    idx = (u32)basyo_pack >> 12;
    bp = (u8 *)screen_w + idx * 4;
    bp[1647] |= 0x40;
    shared_system_state_reset();
    dec_bg_reset();
    set_dec_bri(0);
    master_log_basyo_add();
    return_schedule(1);
    common_disp();
}
