/* Function: season_bgm_set */
/* Address: 0x800675E0 */
/* Size: 352 bytes (88 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s32 device_cmd_send(s32);
extern s32 get_season(void);
extern void device_load_start(s32, s32, s32, s32, s32, s32);

extern u8 now_year;
extern u8 season_no;
extern u8 now_month;
extern u8 now_day;

extern s32 season_bgm_set_tbl2[4];
extern s32 season_bgm_set_tbl0[4];
extern s32 season_bgm_set_tbl1[4];

void season_bgm_set(void) {
    s32 v1;

    device_cmd_send(5);
    device_cmd_send(113);

    v1 = season_no;

    if (now_year == 96) {
        if (now_month == 9) {
            if (now_day == 1) {
                v1 = 1;
            }
        }
    }

    switch (v1) {
    case 0:
        v1 = get_season() & 3;
        device_load_start(0, 0xBFA6, 0xBF87, season_bgm_set_tbl1[v1], season_bgm_set_tbl2[v1], season_bgm_set_tbl0[v1]);
        break;
    case 1:
        device_load_start(0, 0xBFA6, 0xBF87, 0xD7DF, 0xD7BD, 0xD7B2);
        break;
    case 2:
        device_load_start(0, 0xBFA6, 0xBF87, 0xD74D, 0xD72C, 0xD725);
        break;
    case 3:
        device_load_start(0, 0xBFA6, 0xBF87, 0xD78E, 0xD760, 0xD751);
        break;
    }
}
