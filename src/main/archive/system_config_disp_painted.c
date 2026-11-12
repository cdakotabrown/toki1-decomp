/* Function: system_config_disp */
/* Address: 0x8005E0E0 */
/* Size: 376 bytes (94 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: custom_script custom_func_8005E0E0.py */
#include "types.h"

extern void set_kanji_string(s32, s32, s32, void*, s32);
extern s32 get_mascot_count(void);

extern s8 system_config_disp_str0;
extern s8 system_config_disp_str1;
extern s8 system_config_disp_str2;
extern s8 system_config_disp_str3;
extern s8 system_config_disp_str4;
extern s8 system_config_disp_str5;
extern s8 system_config_disp_str6;
extern s8 system_config_disp_str7;

void system_config_disp(s32 selectedIdx)
{
    s16 yPos[8];
    s16 xPos[8];
    s16 sndA[8];
    s16 sndB[8];
    u8 attr[8];
    u8 *p;
    u8 *end;
    u8 *sel;
    s16 *py;
    s16 *px;
    s16 *pa;
    s16 *pb;
    s32 x;

    p = attr;
    sel = p + selectedIdx;
    selectedIdx = 3;
    end = &attr[8];
    py = yPos;
    px = xPos;
    pa = sndA;
    pb = sndB;
    x = -64;

    do {
        *py = -124;
        *px = x;
        *pa = 96;
        *pb = 12;
        if (p == sel) {
            *p = 4;
        } else {
            *p = selectedIdx;
        }
        p++;
        py++;
        px++;
        x += 12;
        pa++;
        pb++;
    } while (p != end);

    set_kanji_string(yPos[0], xPos[0], attr[0], &system_config_disp_str0, 0);
    set_kanji_string(yPos[1], xPos[1], attr[1], &system_config_disp_str1, 0);
    set_kanji_string(yPos[2], xPos[2], attr[2], &system_config_disp_str2, 0);
    set_kanji_string(yPos[3], xPos[3], attr[3], &system_config_disp_str3, 0);
    set_kanji_string(yPos[4], xPos[4], attr[4], &system_config_disp_str4, 0);
    set_kanji_string(yPos[5], xPos[5], attr[5], &system_config_disp_str5, 0);
    if (get_mascot_count() != 0) {
        set_kanji_string(yPos[6], xPos[6], attr[6], &system_config_disp_str6, 0);
    }
    set_kanji_string(yPos[7], xPos[7], attr[7], &system_config_disp_str7, 0);
}
