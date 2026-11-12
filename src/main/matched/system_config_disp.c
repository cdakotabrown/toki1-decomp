/* Address: 0x8005E0E0 */
/* Size: 376 bytes (94 instructions) */
/* Region: Game (0x80041000-0x80086000) */
/* Compiler: IDO 5.2 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: remap_stack_slots --map '88:84,89:85,90:86,91:87,92:88,93:89,94:90,95:91,96:92,112:108,128:124,130:126,132:128,134:130,136:132,138:134,140:136,142:138,144:140,146:142,148:144,150:146,152:148,154:150,156:152,158:154' */
#include "types.h"

extern u8 system_config_disp_str0[];
extern u8 system_config_disp_str1[];
extern u8 system_config_disp_str2[];
extern u8 system_config_disp_str3[];
extern u8 system_config_disp_str4[];
extern u8 system_config_disp_str5[];
extern u8 system_config_disp_str6[];
extern u8 system_config_disp_str7[];

extern void set_kanji_string(s16 x, s16 y, s16 mode, void *str, s32 arg4);
extern s32 get_mascot_count(void);

void system_config_disp(s32 sel)
{
    s16 x[8];
    s16 y[8];
    s16 h[8];
    s16 w[8];
    u8 mode[8];
    s32 i;

    for (i = 0; i < 8; i++) {
        x[i] = -124;
        y[i] = (s16)(-64 + i * 12);
        h[i] = 96;
        w[i] = 12;
        if (i == sel) {
            mode[i] = 4;
        } else {
            mode[i] = 3;
        }
    }

    set_kanji_string(x[0], y[0], mode[0], system_config_disp_str0, 0);
    set_kanji_string(x[1], y[1], mode[1], system_config_disp_str1, 0);
    set_kanji_string(x[2], y[2], mode[2], system_config_disp_str2, 0);
    set_kanji_string(x[3], y[3], mode[3], system_config_disp_str3, 0);
    set_kanji_string(x[4], y[4], mode[4], system_config_disp_str4, 0);
    set_kanji_string(x[5], y[5], mode[5], system_config_disp_str5, 0);
    if (get_mascot_count() != 0) {
        set_kanji_string(x[6], y[6], mode[6], system_config_disp_str6, 0);
    }
    set_kanji_string(x[7], y[7], mode[7], system_config_disp_str7, 0);
}
