/* name_ent_kuchi_anie - NAME_ENT overlay, Address: 0x801494A0, Size: 360 bytes (90 insns) */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=19:v1 */
#include "types.h"

extern s32 get_CD_State();
extern s32 SD_CDLevelGet();
extern void class_clear_n();

extern s32 frame_counter;
extern s32 name_ent_kuchi_now;
extern s32 name_ent_kuchi_level_buf[];

extern u8 cw97_prio;
extern u8 cw97_mode;
extern u8 cw97_disp;
extern u8 cw97_sel;
extern s32 cw97_attr;
extern s32 cw97_hdr;
extern s32 cw97_img;
extern s32 cw97_anim_tbl;
extern s16 cw97_frame;
extern s16 cw97_x;
extern s16 cw97_y;

void name_ent_kuchi_anie(void)
{
    s32 buttons;
    s32 result;
    s32 sum;
    s32 avg;
    s32 idx;

    buttons = get_CD_State();
    if (buttons & 0x0004) {
        result = SD_CDLevelGet();
    } else {
        result = 0;
    }

    idx = frame_counter & 0x0003;
    name_ent_kuchi_level_buf[idx] = result;

    sum = name_ent_kuchi_level_buf[0] + name_ent_kuchi_level_buf[1] + name_ent_kuchi_level_buf[2] + name_ent_kuchi_level_buf[3];
    sum = sum / 4;
    avg = sum / 10;
    name_ent_kuchi_now = result;

    if (avg >= 4) {
        avg = 3;
    }

    class_clear_n(97);
    buttons = get_CD_State();
    if (buttons & 0x0004) {
        cw97_prio = 8;
        cw97_mode = 64;
        cw97_attr = 0x41000000;
        cw97_disp = 164;
        cw97_img = 0x80180294;
        cw97_anim_tbl = 0x801802C8;
        cw97_hdr = 0x80180284;
        cw97_frame = avg + 5;
        cw97_sel = 128;
        cw97_x = -160;
        cw97_y = -120;
    }
}
