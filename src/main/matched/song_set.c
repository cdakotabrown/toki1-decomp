/* Function: song_set */
/* Address: 0x800574C4 */
/* Size: 552 bytes (138 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern u8 song_set_tbl1[];
extern u8 song_set_tbl2[];
extern u8 song_set_tbl3[];
extern u8 song_set_tbl4[];
extern s32 song_set_tbl0;
extern s32 movie_main_tbl1;
extern u8 sys_song_no;

extern void k_disp_switch(s32 a0, s32 a1);
extern void k_disp_start(s32 a0);
extern void konami_mark_set(s32 a0);
extern void song_base_set(s32 a0, s32 a1);
extern void text_sys_init(void);

void song_set(s32 arg0) {
    s32 unused[2];
    s32 i;
    u8 *ptr;

    ptr = song_set_tbl1;

    for (i = 0; i != 22; i++, ptr++) {
        if (arg0 < (s32)((u32)(ptr[0] * 80) / 15u)) {
            break;
        }
    }

    if (i != 0) {
        k_disp_switch(i - 1, 0);
        k_disp_start(i + 1);
    }

    if (ptr >= song_set_tbl2 && ptr < song_set_tbl3) {
        konami_mark_set(0);
    } else {
        konami_mark_set(1);
    }

    if (ptr >= song_set_tbl4) {
        konami_mark_set(0);
        song_base_set(0, 1);
        text_sys_init();
        return;
    }

    if (!song_set_tbl0) {
        return;
    }

    if ((s32)((u32)(ptr[0] * 80) / 15u) - arg0 == 4 ||
        (s32)((u32)(ptr[0] * 80) / 15u) - arg0 == 5) {
        song_base_set((movie_main_tbl1 - 1 + sys_song_no) * 14647247, 1);
        return;
    }

    song_base_set((movie_main_tbl1 - 1 + sys_song_no) * 14647247, 129);
}
