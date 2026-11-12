/* Likely Symbol: don_init_step (from o.bin, offset +164 bytes from 0x800859D0) */
/* Address: 0x80085A74 */
/* Size: 140 bytes */
/* Compiler: IDO 5.2 */
/* Region: IDO (0x80081000-0x800BFFFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "common.h"

extern void cursor_init(s32);
extern void k_reset(s32);
extern void basyo_init(void*);
extern void mes_window_switch(s32);
extern void mascot_disp_switch(s32);

extern s32 work_tarao[32];
extern s16 sound_seek_time;
extern u8 cursor_cw_disp;
extern s32 shared_system_state_reset_jtbl0;

void shared_system_state_reset(void) {
    s16 i;

    for (i = 5; i < 31; i++) {
        work_tarao[i] = 0;
    }

    sound_seek_time = 0;

    cursor_init(0);

    cursor_cw_disp = (u8)(cursor_cw_disp & 0xFF7F);

    k_reset(0);
    basyo_init(&shared_system_state_reset_jtbl0);
    mes_window_switch(0);
    mascot_disp_switch(0);
}
