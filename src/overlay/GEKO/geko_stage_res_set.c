/* geko_stage_res_set -- GEKO overlay, Address: 0x8013C25C, Size: 148 bytes (37 insns) */
#include "types.h"

extern void geko_stage_base_set(void);
extern void palette_upload_enqueue(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4);
extern void date_sprite_init(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
extern void date_object_set(s32 a0, s32 a1, s32 a2, s32 a3);
extern void sub_sub_step_up(void);

extern u32 geko_workbuf_tbl[];

void geko_stage_res_set(void)
{
    geko_stage_base_set();
    palette_upload_enqueue((*(s32 *)&geko_workbuf_tbl[0]), 17, 1, 2, 0);
    date_sprite_init((*(s32 *)&geko_workbuf_tbl[1]), (*(s32 *)&geko_workbuf_tbl[2]), (*(s32 *)&geko_workbuf_tbl[3]), (*(s32 *)&geko_workbuf_tbl[4]),
                  (*(s32 *)&geko_workbuf_tbl[5]), (*(s32 *)&geko_workbuf_tbl[6]));
    date_object_set(0, 0, 0, 0);
    sub_sub_step_up();
}
