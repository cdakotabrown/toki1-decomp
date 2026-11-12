/* @SymbolSize: 84 */
/* name_ent_album_disp_init - NAME_ENT overlay, Address: 0x8013B72C, Size: 92 bytes (23 insns) */
#include "types.h"

extern void text_sys_init(void);
extern void name_ent_album_info_disp(void);
extern void k_sub_reset_point_set(void);
extern void k_sub_disp_start(s32 a0);
extern void name_ent_cursor_set(u32 a0);

extern u8 now_album;
extern s32 name_ent_view_page;

void name_ent_album_disp_init(void)
{
    u8 tmp;
    text_sys_init();
    name_ent_album_info_disp();
    k_sub_reset_point_set();
    k_sub_disp_start(0);
    tmp = now_album;
    name_ent_view_page = 1;
    name_ent_cursor_set((u32)(tmp - 1));
}
