/* @CompileFlags: -Wo,-varref,25 */
/* @CompileEnv: IDO52_COPYPROP_VAR=-12 IDO52_CONST_INREG_VALUES=8:-1:1:2 IDO52_FORCE_WEB_COLOR=54:s1,57:s8,143:s2,140:s3,144:s4,134:s5,135:s6,136:s7,131:v1 */
/* Address: 0x80142E64 */
/* Size: 716 bytes (179 instructions) */
#include "types.h"
#include "tokimeki/date_obj_res_ex.h"
#include "tokimeki/class_w.h"

typedef struct {
    s32 unk00;
    s16 unk04;
    s16 unk06;
    s16 unk08;
    s16 unk0A;
    s16 unk0C;
    u8  unk0E;
    u8  unk0F;
    s16 unk10;
    s16 unk12;
    u8  unk14;
    u8  unk15;
    u8  unk16;
    u8  unk17;
    s16 unk18;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
    s32 unk20;
} ShougatuObj;

typedef struct {
    s16 x;
    s16 y;
} ShougatuPt;

extern void sub_sub_step_up(void);
extern void palette_upload_enqueue(s32, s32, s32, s32, s32);
extern void bustup_ptr_init(void);
extern void date_name_set(s32);
extern void date_sprite_init(s32, s32, s32, s32, s32, s32);
extern void date_object_set(s32, s32, s32, s32);
extern s32 rand(void);
extern void strcpy(u8 *, u8 *);
extern void shougatu_ptrs3_set(void);
extern void shougatu_buf_init2(void);

extern s16 now_serifuev;
extern s16 now_serifuev_in;
extern u8 ba_string[];
extern u8 now_girl;
extern u8 sprite_state[];
extern u8 cal_cw_flag4;
extern ShougatuObj Sprites[];
extern u8 shougatu_crowd_init_str0[];
extern s32 shougatu_ss_serifuev;
extern s32 shougatu_ss_serifuev_kind;
extern s32 shougatu_sss_soundev;
extern s32 shougatu_ev34_res;
extern s32 shougatu_ev34_res_cels;
extern s32 shougatu_ev34_res_anim_tbl;
extern s16 shougatu_ev34_res_scale;
extern s32 shougatu_ev34_res_pal;
extern s32 shougatu_ev34_res_bg0;
extern s32 shougatu_ev34_res_bg1;
extern s32 shougatu_ev34_res_bg2;
extern s32 shougatu_ev34_res_bg3;
extern s32 shougatu_ev34_res_bg4;
extern s32 shougatu_ev34_res_bg5;
extern s32 shougatu_ev42_res_bg3[];
extern ShougatuPt shougatu_crowd_pos[];
extern s32 shougatu_crowd_pos_46[];
extern s16 shougatu_crowd_cnt;
extern s32 shougatu_crowd_timer;
extern CLASS_W class_w[];
extern s32 shougatu_serifuev_tbl[];
extern s32 shougatu_serifuev_kind_tbl[];
extern s32 shougatu_soundev_tbl[];
extern s32 shougatu_crowd_goal[];
extern DATE_OBJ_RES_EX shougatu_ev06_res;

void shougatu_crowd_init(void)
{
    ShougatuObj *p;
    s16 i;
    s32 w;

    now_serifuev = 0;
    now_serifuev_in = 0;
    bustup_ptr_init();
    shougatu_ptrs3_set();
    date_name_set(now_girl);
    shougatu_buf_init2();
    palette_upload_enqueue(shougatu_ev34_res_pal, 17, 1, 2, 0);
    date_sprite_init(shougatu_ev34_res_bg0, shougatu_ev34_res_bg1, shougatu_ev34_res_bg2, shougatu_ev34_res_bg3,
                  shougatu_ev34_res_bg4, shougatu_ev34_res_bg5);
    date_object_set(shougatu_ev34_res_cels, shougatu_ev34_res_anim_tbl, shougatu_ev34_res, shougatu_ev34_res_scale);
    class_w[97].cel_mode = cal_cw_flag4 = 8;
    shougatu_ptrs3_set();
    shougatu_ss_serifuev = shougatu_serifuev_tbl[36];
    shougatu_ss_serifuev_kind = shougatu_serifuev_kind_tbl[36];
    shougatu_sss_soundev = shougatu_soundev_tbl[36];
    strcpy(ba_string, shougatu_crowd_init_str0);
    palette_upload_enqueue(shougatu_ev06_res.pal2, 18, 1, 3, 0);

    for (i = 4, w = 320; i < 54; i++) {
        p = &Sprites[i];
        p->unk00 = (s32)0xC1000000;
        p->unk04 = rand() % w - 159;
        shougatu_crowd_pos[i - 4].x = 0;
        shougatu_crowd_pos[i - 4].y = p->unk04;
        shougatu_crowd_pos_46[i] = *(s32 *)&shougatu_crowd_pos[i - 4];
        p->unk06 = -120;
        p->unk08 = 8;
        p->unk0A = 8;
        p->unk0C = 10;
        p->unk0E = 0;
        p->unk0F = 0;
        p->unk10 = 0;
        p->unk12 = 498;
        p->unk14 = 128;
        p->unk15 = 128;
        p->unk16 = 128;
        p->unk18 = 0;
        p->unk1A = 0;
        p->unk1C = 4096;
        p->unk1E = 4096;
        p->unk20 = 0;
        sprite_state[i] = 9;
        ((s16 *)&shougatu_crowd_goal[48])[i] = 2;
        shougatu_ev42_res_bg3[i] = 0x8000;
    }

    shougatu_crowd_cnt = 0;
    shougatu_crowd_timer = 0;
    sub_sub_step_up();
}
