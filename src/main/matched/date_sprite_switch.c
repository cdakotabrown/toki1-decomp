/* Address: 0x8008543C */
/* Size: 308 bytes (77 instructions) */
/* Region: Library (mixed) */
/* @CompileEnv: IDO52_UGEN_TEMP_REG=3:t9,4:t1,5:t2,6:t3,7:t4,8:t5,10:t7,11:t9,12:t1,13:t2,14:t3,15:t4,16:t5 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions 0xf4 0xf8 */
#include "types.h"

extern u8  cal_cw_disp;
extern u8  cw97_disp;
extern u8  date_object_set_tbl0;
extern u8  bustup_cd_read_tbl0;
extern s32 Sprites[];

void date_sprite_switch(s32 mute) {
    s32 v;
    s32 f;

    if ((mute & 0xFF) != 0) {
        if (cal_cw_disp != 0) {
            u8 tmp = bustup_cd_read_tbl0;
            bustup_cd_read_tbl0 = date_object_set_tbl0;
            date_object_set_tbl0 = tmp;
            cal_cw_disp |= 0x80;
        }
        if (cw97_disp != 0) {
            cw97_disp |= 0x80;
        }
        v = 58;
        do {
            s32 *slot = (s32 *)((u8 *)Sprites + v * 36);
            *slot &= 0x7FFFFFFF;
            v = (s16)(v + 1);
        } while (v < 64);
    } else {
        f = cal_cw_disp;
        bustup_cd_read_tbl0 = date_object_set_tbl0;
        if (f != 0) {
            cal_cw_disp = f & 0xFF7F;
        }
        if (cw97_disp != 0) {
            cw97_disp &= 0xFF7F;
        }
        v = 58;
        do {
            s32 *slot = (s32 *)((u8 *)Sprites + v * 36);
            *slot |= (s32)0x80000000;
            v = (s16)(v + 1);
        } while (v < 64);
    }
}
