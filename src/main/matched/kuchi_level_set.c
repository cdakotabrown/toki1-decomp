/* Function: kuchi_level_set */
/* Address: 0x80067D58 */
/* Size: 428 bytes (107 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: custom_script custom_game_mode_main0.py */
#include "types.h"

extern u32 get_CD_State(void);
extern s32 SD_CDLevelGet(void);

extern u8 voice_level;
extern u32 frame_counter;
extern u8 now_girl;
extern s16 kuchi_level[4];
extern s16 cal_cw_frame;
extern u8 bustup_kuchi_ani_tbl0[];

void kuchi_level_set(void) {
    s32 enabled;
    s32 buttonVal;
    s32 count;
    s32 fv;
    s32 max;
    s32 i;

    enabled = voice_level;

    if (enabled == 0) {
        return;
    }

    if (get_CD_State() & 4) {
        buttonVal = SD_CDLevelGet() + 1;
    } else {
        buttonVal = 0;
    }

    fv = frame_counter;
    kuchi_level[fv & 3] = buttonVal;

    max = 0;
    for (i = 0; i < 4; i++) {
        if (max < kuchi_level[i]) {
            max = kuchi_level[i];
        }
    }

    max = 0;
    for (i = 0; i < 4; i++) {
        if (max < kuchi_level[i]) {
            max = kuchi_level[i];
        }
    }

    count = 0;
    for (i = 0; i < 3; i++) {
        if (bustup_kuchi_ani_tbl0[(u32)now_girl * 3 + i] < (u32)buttonVal) {
            count = i + 1;
        }
    }

    if (count >= 4) {
        count = 3;
    }

    if (cal_cw_frame != 0) {
        if (fv & 1) {
            return;
        }
    }

    if (buttonVal == 0) {
        cal_cw_frame = 0;
        return;
    }

    if (count == 0) {
        cal_cw_frame = 0;
        return;
    }

    if (count == 1) {
        cal_cw_frame = 1;
        return;
    }

    if (count == 2) {
        cal_cw_frame = 2;
        return;
    }

    if (count >= 3) {
        cal_cw_frame = 3;
    }
}
