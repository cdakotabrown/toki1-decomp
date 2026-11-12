/* @PostProc: swap_instructions --offset1 0xfc --offset2 0x100 */
/* @PostProc: swap_instructions --offset1 0x108 --offset2 0x10c */
/* @CompileEnv: IDO52_AS1_NO_FALLTHRU_FILL=1 */
/* Address: 0x8013EB80 */
/* Size: 332 bytes (83 instructions) */
#include "types.h"

extern void cd_read_start2(s32, s32, s32);
extern void event_bustup_cd_read(s32, s32, s32, s32);

extern s32 etc_ev_lba_tbl[];
extern u8 etc_ev_cnt_tbl[];
extern s32 etc_bustup_flag;
extern s32 etc_rec_kind;
extern s32 etc_rec_id;
extern s32 etc_rec_girl;
extern s32 etc_rec_season;
extern s32 etc_event_phase;
extern s32 etc_event_flag;

void etc_event_read(s32 arg0)
{
    s32 mode;
    s32 idx;
    s32 val;
    s32 sel;

    event_bustup_cd_read(etc_rec_kind, etc_rec_id, etc_rec_girl, etc_rec_season);
    if (etc_bustup_flag != 0) {
        return;
    }

    mode = etc_rec_kind;
    if (mode != 2) {
        if (mode != 8) {
            return;
        }
    }

    if (mode == 8 && etc_rec_id == 14) {
        cd_read_start2(113, 0x801AE000, 0x8768);
        etc_event_flag = 1;
        etc_event_phase = arg0;
    } else if (mode == 8 && etc_rec_id == 70) {
        cd_read_start2(113, 0x801B0000, 0x85B6);
        etc_event_flag = 1;
        etc_event_phase = arg0;
    } else if (mode == 2) {
        idx = etc_rec_id;
        val = etc_ev_lba_tbl[idx];
        sel = etc_ev_cnt_tbl[idx * 4];
        cd_read_start2(sel, 0x801B0000, val);
        etc_event_flag = 1;
        if (etc_rec_id == 32) {
            etc_event_phase = arg0;
        }
    }
}
