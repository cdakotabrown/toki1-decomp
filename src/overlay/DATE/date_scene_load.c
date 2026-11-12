/* @SymbolSize: 200 */
/* Function: date_scene_load */
/* Overlay: DATE */
/* Address: 0x8013CA78 */
#include "types.h"

extern void sub_sub_step_up(void);
extern s32 cd_read_start2(s32, s32, s32);
extern void strcpy(void *, void *);
extern void date_buf_tbl4_init(void);

extern u8 now_basyo;
extern s16 now_serifu;
extern s16 now_serifu_in;
extern s32 ss_serifu;
extern s32 ss_serifu_kind;
extern s32 sss_sound;
extern u8 g_name;
extern s32 kuchi_stop;
extern u8 now_girl;
extern s32 date_scene_load_ptr[];
extern u8 date_scene_load_str0;

void date_scene_load(void)
{
    cd_read_start2(1, 0x801B0000, 0xAFCA);
    date_buf_tbl4_init();

    if (now_basyo == 0 || now_basyo == 1) {
        now_serifu = 0;
    } else {
        now_serifu = 1;
    }

    now_serifu_in = 0;
    ss_serifu = date_scene_load_ptr[0];
    ss_serifu_kind = date_scene_load_ptr[1];
    sss_sound = date_scene_load_ptr[2];
    kuchi_stop = 1;
    now_girl = 14;
    strcpy(&g_name, &date_scene_load_str0);
    sub_sub_step_up();
}
