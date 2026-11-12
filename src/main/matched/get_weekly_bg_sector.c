/* Function: get_weekly_bg_sector */
/* Address: 0x80073A94 */
/* Size: 96 bytes (24 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: swap_registers v1 a0 */
/* @PostProc: swap_registers t6 v1 */
/* @PostProc: swap_registers t7 t6 */
/* @PostProc: swap_registers t8 t7 */
/* @PostProc: swap_registers t9 t8 */
/* @PostProc: swap_registers t0 t9 */
/* @PostProc: swap_registers t1 t0 */
#include "types.h"

extern u8 schedule_cursor;
extern u8 schedule_kind;
extern s32 weekly_bg_sector_tbl[];
extern u16 basyo_pack;
extern s32 basyo_bg_sector_tbl[];

s32 get_weekly_bg_sector(void) {
    u8 idx;
    u8 val;
    u32 t;

    idx = schedule_cursor;
    val = *(&schedule_kind + idx * 4);
    if (val != 4) {
        return weekly_bg_sector_tbl[val];
    }
    t = (u32)basyo_pack;
    return basyo_bg_sector_tbl[t >> 12];
}
