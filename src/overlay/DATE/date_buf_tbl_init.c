/* Function: date_buf_tbl_init */
/* Overlay: DATE */
/* Address: 0x80132284 */
/* Size: 304 bytes (76 instructions) */
#include "types.h"

extern s32 date_effect_res[];

void date_buf_tbl_init(void) {
    date_effect_res[0] = (s32)0x801F2820;
    date_effect_res[1] = (s32)0x801F2830;
    date_effect_res[2] = (s32)0x801F28B4;
    (*(s16 *)&date_effect_res[3]) = (*(s16 *)0x801F28CC);
    date_effect_res[4] = (s32)0x801B0000;
    date_effect_res[5] = (s32)0x801CA000;
    date_effect_res[6] = (s32)0x801E2200;
    date_effect_res[7] = (s32)0x801B2000;
    date_effect_res[8] = (s32)0x801B6000;
    date_effect_res[9] = (s32)0x801BA000;
    date_effect_res[10] = (s32)0x801BE000;
    date_effect_res[11] = (s32)0x801C2000;
    date_effect_res[12] = (s32)0x801C6000;
    date_effect_res[13] = (s32)0x801CA200;
    date_effect_res[14] = (s32)0x801CE200;
    date_effect_res[15] = (s32)0x801D2200;
    date_effect_res[16] = (s32)0x801D6200;
    date_effect_res[17] = (s32)0x801DA200;
    date_effect_res[18] = (s32)0x801DE200;
}
