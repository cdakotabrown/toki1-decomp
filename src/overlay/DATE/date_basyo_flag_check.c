/* @SymbolStart: 0x8013C76C */
/* @SymbolSize: 780 */
/* Function: date_basyo_flag_check */
/* Overlay: DATE */
/* Address: 0x8013C76C (head) / 0x8013C770, Size: 780 bytes (195 insns) */
/* @PostProc: swap_instructions --offset1 0x48 --offset2 0x50 */
/* @PostProc: swap_instructions --offset1 0xe4 --offset2 0xe8 */
/* @PostProc: swap_instructions --offset1 0x1d0 --offset2 0x1d4 */
/* @PostProc: swap_instructions --offset1 0x274 --offset2 0x27c */
/* @PostProc: swap_instructions --offset1 0x278 --offset2 0x280 */
/* @PostProc: swap_instructions --offset1 0x27c --offset2 0x284 */
/* @PostProc: swap_instructions --offset1 0x280 --offset2 0x284 */
/* @PostProc: swap_instructions --offset1 0x288 --offset2 0x290 */
/* @PostProc: swap_instructions --offset1 0x28c --offset2 0x294 */
/* @PostProc: swap_instructions --offset1 0x290 --offset2 0x298 */
/* @PostProc: swap_instructions --offset1 0x294 --offset2 0x298 */
/* @PostProc: swap_instructions --offset1 0x2a0 --offset2 0x2a4 */
/* @PostProc: swap_instructions --offset1 0x2a4 --offset2 0x2ac */
/* @PostProc: swap_instructions --offset1 0x2a8 --offset2 0x2ac */
/* @PostProc: swap_instructions --offset1 0x2b8 --offset2 0x2bc */
/* @PostProc: patch_insn_reg_field 0x18:rt:a2:a3 0x24:rd:t0:a2 0x28:rs:a2:a3 0x28:rt:a2:a3 0x2c:rd:a1:v1 0x30:rt:t2:t1 0x34:rt:t1:t0 0x38:rt:a3:a1 0x3c:rt:a1:v1 0x40:rd:v1:v0 0x40:rs:t6:a3 0x40:rt:a2:t6 0x44:rs:v1:v0 0x48:rd:t4:t6 0x50:rd:t3:t4 0x54:rt:t0:a2 0x58:rs:v1:v0 0x58:rt:v0:t8 0x60:rd:t8:t9 0x60:rt:v0:t8 0x64:rd:t9:t2 0x64:rt:t8:t9 0x68:rs:a3:a1 0x68:rt:t9:t2 0x70:rs:t3:t4 0x78:rs:t4:t6 0x7c:rt:t5:t7 0x80:rs:t5:t7 0x80:rt:t5:t7 0x84:rt:t6:t8 0x88:rs:t1:t0 0x88:rt:t5:t7 0x90:rs:t6:t8 0x90:rt:t6:t8 0x98:rs:t2:t1 0x98:rt:t6:t8 0x9c:rt:t7:t9 0xa4:rt:t7:t9 0xa8:rs:a2:a3 0xa8:rt:t8:t3 0xac:rt:t3:t5 0xb0:rs:t8:t3 0xb0:rt:t9:t4 0xb4:rs:a2:a3 0xb4:rt:t9:t4 0xb8:rs:t3:t5 0xb8:rt:t3:t5 0xc0:rs:t3:t5 0xc0:rt:t4:t6 0xc4:rt:t4:t6 0xcc:rt:t5:t7 0xd0:rt:t6:t8 0xd4:rs:t6:t8 0xd4:rt:t6:t8 0xd8:rt:t5:t7 0xe0:rt:t7:t9 0xe4:rt:t6:t8 0xe8:rs:t7:t9 0xe8:rt:t7:t9 0xf0:rt:t7:t9 0xf8:rt:t8:t2 0xfc:rt:t8:t2 0x130:rs:a1:v1 0x130:rt:a1:v1 0x134:rd:t9:t3 0x134:rt:a1:v1 0x138:rd:a1:v1 0x138:rt:t9:t3 0x13c:rs:a1:v1 0x144:rt:a1:v1 0x148:rs:t0:a2 0x14c:rt:t4:t5 0x150:rs:t4:t5 0x150:rt:t4:t5 0x154:rt:t5:t6 0x158:rs:t1:t0 0x158:rt:t4:t5 0x160:rs:t5:t6 0x160:rt:t5:t6 0x164:rd:t6:t8 0x168:rs:t2:t1 0x168:rt:t5:t6 0x170:rs:t6:t8 0x174:rt:t7:t9 0x178:rs:t7:t9 0x178:rt:t7:t2 0x180:rs:t7:t2 0x180:rt:t8:t3 0x184:rt:t8:t3 0x18c:rt:t9:t4 0x190:rt:t9:t4 0x194:rs:a2:a3 0x194:rt:t3:t6 0x198:rt:t5:t8 0x19c:rs:t3:t6 0x19c:rt:t4:t7 0x1a0:rs:a2:a3 0x1a0:rt:t4:t7 0x1a4:rs:t5:t8 0x1a4:rt:t5:t8 0x1ac:rs:t5:t8 0x1ac:rt:t6:t9 0x1b0:rt:t6:t9 0x1b8:rt:t7:t2 0x1bc:rt:t8:t3 0x1c0:rs:t8:t3 0x1c0:rt:t8:t3 0x1c4:rt:t7:t2 0x1cc:rt:t9:t4 0x1d0:rt:t8:t3 0x1d4:rs:t9:t4 0x1d4:rt:t9:t4 0x1dc:rt:t9:t4 0x1e4:rt:t3:t5 0x1e8:rt:t3:t5 0x22c:rt:t4:t6 0x238:rs:t4:t6 0x238:rt:t4:t6 0x23c:rd:t5:t8 0x240:rt:t4:t6 0x248:rs:t5:t8 0x24c:rt:t6:t9 0x250:rs:t6:t9 0x250:rt:t6:t9 0x258:rs:t6:t9 0x268:rt:t7:t2 0x270:rt:t7:t2 0x278:rt:t8:t3 0x27c:rt:t8:t3 0x280:rt:t9:t4 0x284:rs:t9:t4 0x284:rt:t9:t4 0x28c:rs:t9:t4 0x28c:rt:t3:t5 0x290:rt:t3:t5 0x294:rt:t4:t6 0x298:rs:t4:t6 0x298:rt:t4:t7 0x2a0:rs:t4:t7 0x2a0:rt:t5:t8 0x2a4:rt:t5:t8 0x2a8:rt:t6:t9 0x2ac:rs:t6:t9 0x2ac:rt:t6:t9 0x2b4:rt:t7:t2 0x2b8:rt:t6:t9 0x2bc:rs:t7:t2 0x2bc:rt:t7:t2 0x2c4:rt:t7:t2 0x2cc:rt:t8:t3 0x2d0:rt:t8:t3 */
/* @CompileEnv: IDO52_COPYPROP_VAR=-4 IDO52_FORCE_WEB_COLOR=13:a1,0:a0 */
#include "types.h"

extern s32 sub_sub_step_up(void);
extern s32 goto_sub_sub_step(s32);

extern u8 now_basyo;
extern u8 screen_w[];
extern u8 now_year;
extern u8 now_month;
extern u8 byouki_state;
extern u8 ev_flag55;
extern u8 bukatu_flag11;
extern u8 day_flag;
extern u8 ret_step;
extern u8 ret_sub_step;
extern u8 ret_sub_sub_step;
extern u8 basyo_hist;
extern u8 basyo_hist_1;
extern u8 basyo_hist_2;
extern u8 basyo_hist_3;
extern u8 now_step;
extern u8 now_sub_step;
extern s32 rpg_bat_arg;
extern s32 basyo_pack;

s32 date_basyo_flag_check(void)
{
    s32 head = basyo_pack;
    u8 *p;
    u8 *q;
    s32 w;
    s32 seen;
    s16 i;

    p = (u8 *)((u32)screen_w | 0);
    seen = 0;

    for (i = 0; i < 11; i++) {
        q = p + i * 4;
        if (*(u16 *)(q + 1644) != 0) {
            seen = 1;
        }
        w = *(s32 *)(q + 1644);
        if (((u32)(w << 5) >> 30) != 3 && (head << 12) >= 0) {
            continue;
        }
        if ((head << 11) < 0) {
            continue;
        }
        if (now_year != 97) {
            continue;
        }
        if (now_month != 9) {
            continue;
        }
        ev_flag55 = 2;
        p[246] |= 0x10;
        day_flag &= 8;
        rpg_bat_arg = 5;
        ret_step = now_step;
        ret_sub_step = now_sub_step;
        ret_sub_sub_step = 62;
        basyo_hist = 0;
        basyo_hist_1 = 0;
        basyo_hist_2 = 0;
        basyo_hist_3 = 0;
        goto_sub_sub_step(64);
        return 0;
    }

    if (seen == 0 && now_year == 97 && now_month == 9 &&
        (head << 11) >= 0) {
        bukatu_flag11 |= 6;
        ev_flag55 = 2;
        p[246] |= 0x10;
        day_flag &= 8;
        rpg_bat_arg = 5;
        ret_step = now_step;
        ret_sub_step = now_sub_step;
        ret_sub_sub_step = 62;
        basyo_hist = 0;
        basyo_hist_1 = 0;
        basyo_hist_2 = 0;
        basyo_hist_3 = 0;
        goto_sub_sub_step(64);
        return 0;
    }

    if (basyo_hist == now_basyo && basyo_hist_1 == basyo_hist &&
        (head << 11) >= 0 && (u32)now_year >= 96U && now_basyo != 0) {
        rpg_bat_arg = 4;
        ev_flag55 = 1;
        day_flag &= 8;
        byouki_state |= 8;
        ret_step = now_step;
        ret_sub_step = now_sub_step;
        ret_sub_sub_step = 62;
        basyo_hist_1 = 0;
        basyo_hist_2 = 0;
        basyo_hist_3 = 0;
        basyo_hist = 0;
        return goto_sub_sub_step(64);
    }

    return sub_sub_step_up();
}
