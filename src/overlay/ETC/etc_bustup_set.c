/* Address: 0x8013ED28 */
/* Size: 84 bytes (21 insns) */
#include "types.h"

extern s32 etc_bustup_res;
extern s32 etc_bustup_res_img;
extern s32 etc_bustup_res_anim_tbl;
extern s16 etc_bustup_res_aux;
extern s32 etc_bustup_res_pal;

void etc_bustup_set(void) {
    etc_bustup_res = 0x80180084;
    etc_bustup_res_img = 0x80180098;
    etc_bustup_res_anim_tbl = 0x8018009C;
    etc_bustup_res_aux = *(s16 *)0x801960AC;
    etc_bustup_res_pal = 0x801800AC;
}
