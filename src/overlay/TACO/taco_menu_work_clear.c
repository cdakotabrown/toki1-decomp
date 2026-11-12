/* Address: 0x80135730 */
/* Size: 36 bytes (9 insns) */
#include "types.h"

extern s32 taco_submenu_sel;
extern s32 taco_menu_edit_flag;
extern s32 taco_menu_val;
extern s32 taco_menu_anim_cnt;

void taco_menu_work_clear(void)
{
    taco_submenu_sel = 0;
    taco_menu_edit_flag = 0;
    taco_menu_val = 0;
    taco_menu_anim_cnt = 0;
}
