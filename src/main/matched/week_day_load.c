/* Function: week_day_load */
/* Address: 0x80074270 */
/* Size: 448 bytes (112 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @SymbolStart: 0x8007426C */
/* @SymbolSize: 452 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: swap_registers v1 v0 */
#include "types.h"

extern u8 step_mode;
extern u8 byouki_kind;
extern u16 byouki_state;
extern s32 sched_icon;
extern u8 schedule_cursor;
extern s32 work_fuku;

typedef struct {
    u8 type;
    u8 pad[3];
} CharInfo;
extern CharInfo schedule_kind[];

extern s32 week_day_load_tbl0[];
extern s32 holiday_girl_init_tbl0[];

extern void week_day_bustup_set(void);
extern void cd_read_start2(s32, s32, s32);
extern s32 get_CD_State2(void);
extern s32 block_checksum_check(s32, s32);
extern void sub_sub_step_up(void);

s32 week_day_load(void) {
    u8 mode = step_mode;

    if (mode == 0) {
        week_day_bustup_set();
        if ((byouki_kind & 0xF) == 3 && (byouki_state & 1) == 0 && (sched_icon & 0xF) == 7) {
            cd_read_start2(9, 0x801E0000, 32138);
            step_mode = 1;
        } else if (schedule_kind[schedule_cursor].type != 4) {
            cd_read_start2(41, 0x801E0000, week_day_load_tbl0[work_fuku]);
            step_mode = 1;
        } else {
            cd_read_start2(41, 0x801E0000, holiday_girl_init_tbl0[work_fuku]);
            step_mode = 1;
        }
    } else if (mode == 1) {
        if (get_CD_State2() & 1) {
            if ((byouki_kind & 0xF) == 3 && (byouki_state & 1) == 0 && (sched_icon & 0xF) == 7) {
                if (block_checksum_check(9, 0x801E0000) != 0) {
                    sub_sub_step_up();
                } else {
                    step_mode = 0;
                    return 0;
                }
            } else {
                if (block_checksum_check(41, 0x801E0000) != 0) {
                    sub_sub_step_up();
                } else {
                    step_mode = 0;
                    return 0;
                }
            }
        }
    }
}
