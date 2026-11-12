/* name_ent_cdlevel_disp - NAME_ENT overlay, Address: 0x80144670, Size: 476 bytes */
/* @SymbolSize: 476 */
/* @CompileEnv: IDO52_NO_DEADSTORE_GLOBALS=0xfffffff4 IDO52_UGEN_DEST_TEMP=16 */
#include "types.h"
#include "tokimeki/class_w.h"

extern s32 get_CD_State(void);
extern s32 SD_CDLevelGet(void);
extern void class_clear_n(s32 a0);
extern void load_tpage_buf_lock(s32 a0, s32 a1, s32 a2);

extern s32 pad_data;
extern s32 step_state;
extern s32 frame_counter;

extern s16 kuchi_level[];

extern s16 cw98_anim;
extern s16 cw98_frame;
extern s16 cw98_x;
extern s16 cw98_y;
extern CLASS_W class_w[];

void name_ent_cdlevel_disp(void)
{
    s32 sel;
    s32 sum;
    s32 avg;
    s32 *ptr;

    if (get_CD_State() & 4) {
        sel = SD_CDLevelGet();
    } else {
        sel = 0;
        if (((u32)pad_data >> 16) != 0) {
            sel = step_state & 0x3F;
        }
    }

    kuchi_level[frame_counter & 3] = sel;
    sum = kuchi_level[0] + kuchi_level[1] + kuchi_level[2] + kuchi_level[3];
    avg = sum / 4;

    if (get_CD_State() & 4) {
        if ((class_w[98].anim_flag & 1) != 0) {
            if ((class_w[98].disp_flag & 0x80) != 0) {
                return;
            }
        }
        class_clear_n(98);
        class_w[98].prio = 3;
        class_w[98].anim_flag = 17;
        class_w[98].attribute = 0x41000000;
        class_w[98].disp_flag = 172;
        (*(u32 *)&class_w[98].cels) = 0x801B27B0;
        (*(u32 *)&class_w[98].anim_tbl) = 0x801B27D8;
        ptr = (s32 *)0x801B27AC;
        (*(s32 **)&class_w[98].hdr) = ptr;
        cw98_anim = 1;
        class_w[98].cel_mode = 8;
        class_w[98].tpage_sel = 128;
        cw98_x = -160;
        cw98_y = -120;
        load_tpage_buf_lock(5, *ptr, 1);
    } else {
        class_clear_n(98);
        class_w[98].disp_flag = 0;
        class_w[98].anim_flag = 0;
        cw98_frame = 0;
        class_w[98].tick = 0;
    }
}
