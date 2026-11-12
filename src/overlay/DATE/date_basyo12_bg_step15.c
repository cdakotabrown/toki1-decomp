/* @PostProc: swap_instructions --offset1 0x9c --offset2 0xa0 */
/* @PostProc: swap_instructions --offset1 0xb0 --offset2 0xc0 */
/* @PostProc: swap_instructions --offset1 0xc0 --offset2 0xc4 */
/* @PostProc: swap_instructions --offset1 0xc4 --offset2 0xc8 */
/* @PostProc: patch_insn_reg_field 0x44:rt:a1:a0 0x50:rs:a1:a0 0x50:rt:a1:a0 0x58:rs:a1:a0 0x74:rd:v1:v0 0x78:rt:v1:v0 0x80:rt:v1:v0 0x84:rt:v0:t4 0x88:rs:v0:t4 0x88:rt:v0:t4 0x98:rd:t4:t5 0x98:rt:v0:t4 0x9c:rd:t5:t6 0x9c:rs:a1:a0 0x9c:rt:t4:t5 0xa0:rt:v1:v0 0xa4:rs:t5:t6 0xa4:rt:t6:t7 0xac:rs:t6:t7 0xb0:rt:v0:v1 0xb4:rs:v0:t4 0xb4:rt:t7:t8 0xbc:rt:t7:t8 0xc4:rt:v0:v1 0xd0:rs:v0:v1 */
/* @SymbolSize: 220 */
/* Function: date_basyo12_bg_step15 */
/* Overlay: DATE */
/* Address: 0x80137D88 (55 insns = 220 bytes; .sym over-sizes to 228 = */
#include "types.h"

extern s32 sub_sub_step_up();
extern s32 device_cmd_send();
extern s32 dec_bg_cache_step();
extern s32 event_log_add();
extern s32 strcpy();

extern u8 ba_string;
extern u8 date_basyo12_bg_step15_str0;
extern u8 now_girl;
extern u8 rec_idx;
extern u8 screen_w[];
extern s16 now_serifu;
extern u8 date_pattern;
extern s32 now_date_spot;

s32 date_basyo12_bg_step15(void)
{
    s32 v0;
    s32 *p;
    s32 v1;
    s32 i1;
    s32 i2;

    device_cmd_send(519);
    strcpy(&ba_string, &date_basyo12_bg_step15_str0);
    dec_bg_cache_step(17268);

    i1 = now_girl;
    if ((screen_w[i1 * 56 + 484] & 1) == 0) {
        v0 = 0;
    } else {
        v0 = 1;
    }
    now_serifu = now_serifu + (v0 << 1);
    date_pattern = (u8)v0;

    i2 = rec_idx;
    v1 = 20;
    if (screen_w[i2 * 8 + 1880] == 1) {
        rec_idx = (u8)(i2 - 1);
    }
    p = (s32 *)&now_date_spot;
    *p = v1;
    event_log_add(0, *p & 0xFF);
    return sub_sub_step_up();
}
