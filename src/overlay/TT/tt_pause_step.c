/* @SymbolSize: 280 */
/* @CompileEnv: IDO52_UGEN_TREG_ORDER=t6,t7,t8,t9,t0,t1 */
/* @PostProc: patch_insn_reg_field 0x40:rt:t1:t7 0x80:rt:t7:t2 0x84:rt:t0:v0 0x88:rs:t7:t2 0x88:rt:t8:t3 0x90:rd:t6:t4 0x90:rt:t8:t3 0x94:rs:t6:t4 0x94:rt:t9:t5 0x98:rs:t9:t5 0xa4:rt:t0:v0 0xb0:rs:t1:t7 0xb0:rt:t1:t7 0xb8:rs:t1:t7 0xb8:rt:t7:t8 0xc0:rs:t7:t8 0xc0:rt:t8:t6 0xc4:rs:t8:t6 0xe4:rt:t6:t9 0xec:rs:t6:t9 0xf0:rt:t9:t0 0xf4:rt:t0:t1 0xf8:rt:t1:t2 0xfc:rt:t7:t3 0x100:rt:t9:t0 0x104:rt:t0:t1 0x108:rt:t1:t2 0x10c:rt:t7:t3 */
/* Address: 0x8013ED68, real size 280 bytes (70 instructions). */
/* symbol (tt_pause_step + tt_hold_check + tt_dbuf_swap). @SymbolSize: 280 */
#include "types.h"

extern s32 device_cmd_send();
extern s32 SsUtAllKeyOff();
extern s32 tt_cmd_once();
extern s32 tt_konami_command_step();
extern u8 *tt_game_wk;
extern u8 *tt_step_wk;
extern u8 *tt_text_buf;

void tt_pause_step(void)
{
    u8 *p0;
    u8 *p1;
    u8 *s0;

    p0 = tt_game_wk;
    p1 = tt_step_wk;
    s0 = tt_text_buf + 2540;
    if (*(u8 *)(p1 + 16) != 0) {
        if ((*(u16 *)(p0 + 20) & 0x800) != 0) {
            device_cmd_send(241);
            *(u8 *)(p1 + 16) = 0;
            *(u8 *)(s0 + 0) = 0;
        }
        tt_konami_command_step();
        if ((((u32)*(u16 *)(p0 + 8) >> 4) & 1) != 0) {
            *(s16 *)(s0 + 6) = 1;
        } else {
            *(s16 *)(s0 + 6) = 0;
        }
    } else {
        p0 = tt_game_wk;
        if ((*(u16 *)(p0 + 20) & 0x800) != 0) {
            SsUtAllKeyOff(0);
            device_cmd_send(225);
            tt_cmd_once(1536);
            *(u8 *)(p1 + 16) = 1;
            *(s16 *)(s0 + 10) = -20;
            *(s16 *)(s0 + 12) = 116;
            *(s16 *)(s0 + 14) = 8;
            *(u8 *)(s0 + 2) = 6;
            *(u8 *)(s0 + 0) = 1;
            *(s16 *)(s0 + 16) = 0;
        }
    }
}
