/* @SymbolSize: 192 */
/* ending_desc_init - ENDING overlay, Address: 0x8013B6D0, Size: 192 bytes (48 insns) */
#include "types.h"

extern s32 ending_scene_ptr_tbl[];

void ending_desc_init(void)
{
    ending_scene_ptr_tbl[0] = 0x801C22CC;
    ending_scene_ptr_tbl[1] = 0x801C22D4;
    ending_scene_ptr_tbl[2] = 0x801C22F0;
    (*(s16 *)&ending_scene_ptr_tbl[3]) = *(s16 *)0x801C2300;
    ending_scene_ptr_tbl[4] = 0x801A0000;
    ending_scene_ptr_tbl[5] = 0x801C2000;
    ending_scene_ptr_tbl[6] = 0x801AA000;
    ending_scene_ptr_tbl[7] = 0x801AE000;
    ending_scene_ptr_tbl[8] = 0x801B2000;
    ending_scene_ptr_tbl[9] = 0x801B6000;
    ending_scene_ptr_tbl[10] = 0x801BA000;
    ending_scene_ptr_tbl[11] = 0x801BE000;
}
