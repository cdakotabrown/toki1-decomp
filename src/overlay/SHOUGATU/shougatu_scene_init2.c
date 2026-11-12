/* @SymbolStart: 0x80143468 */
/* @SymbolSize: 316 */
/* @PostProc: swap_instructions --offset1 0x108 --offset2 0x114 */
/* @PostProc: swap_instructions --offset1 0x110 --offset2 0x114 */
/* Address: 0x80143470 (true start 0x80143468: the -1 const + %hi base are */
/* Size: 308 bytes (77 insns) + 8 hoisted = 316 bytes (79 insns). */
#include "types.h"
#include "tokimeki/class_w.h"

extern void sub_sub_step_up();
extern void palette_upload_enqueue();
extern void date_sprite_init();
extern void date_object_set();
extern void shougatu_buf_init3();


extern s32 shougatu_ev42_res;
extern s32 shougatu_ev42_res_cels;
extern s32 shougatu_ev42_res_anim_tbl;
extern s16 shougatu_ev42_res_scale;
extern s32 shougatu_ev42_res_pal;
extern s32 shougatu_ev42_res_bg0;
extern s32 shougatu_ev42_res_bg1;
extern s32 shougatu_ev42_res_bg2;
extern s32 shougatu_ev42_res_bg3;
extern s32 shougatu_ev42_res_bg4;
extern s32 shougatu_ev42_res_bg5;

extern u8 cal_cw_flag4;
extern u8 cw97_cel_mode;
extern s16 cw98_anim;
extern s16 cw98_frame;

typedef struct { s32 w[17]; } Blk68;
extern Blk68 cw96_active;
extern Blk68 cw98_active;
extern s32 tpage_buf[];
extern CLASS_W class_w[];

void shougatu_scene_init2(void)
{
    tpage_buf[26] = -1;
    tpage_buf[58] = -1;
    shougatu_buf_init3();
    palette_upload_enqueue(shougatu_ev42_res_pal, 17, 1, 2, 0);
    date_sprite_init(shougatu_ev42_res_bg0, shougatu_ev42_res_bg1, shougatu_ev42_res_bg2, shougatu_ev42_res_bg3, shougatu_ev42_res_bg4,
                  shougatu_ev42_res_bg5);
    date_object_set(shougatu_ev42_res_cels, shougatu_ev42_res_anim_tbl, shougatu_ev42_res, shougatu_ev42_res_scale);
    cw97_cel_mode = 8;
    cal_cw_flag4 = 8;
    cw98_active = cw96_active;
    cw98_anim = 4;
    cw98_frame = 0;
    class_w[98].tick = 0;
    class_w[98].anim_flag = 0;
    sub_sub_step_up();
}
