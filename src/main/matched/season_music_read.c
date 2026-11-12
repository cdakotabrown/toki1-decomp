/* Address: 0x80067568 */
/* Size: 120 bytes (30 instructions) */
/* Compiler: IDO 5.2 -O2 -G0 -mips1 + patch_frame_size 32->48 */
/* Region: GCC (0x80041000-0x80080FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "common.h"

extern void device_cmd_send(int arg0);
extern int get_season(void);
extern void device_load_start(int a0, int a1, int a2, int a3, int stack0, int stack1);

extern int season_bgm_set_tbl0[4];
extern int season_bgm_set_tbl1[4];
extern int season_bgm_set_tbl2[4];

void season_music_read(void) {
    int idx;

    device_cmd_send(5);
    device_cmd_send(0x71);
    idx = get_season() & 3;
    device_load_start(0, 0xBFA6, 0xBF87, season_bgm_set_tbl1[idx], season_bgm_set_tbl2[idx], season_bgm_set_tbl0[idx]);
}
