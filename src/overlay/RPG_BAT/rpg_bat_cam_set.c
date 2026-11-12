/* rpg_bat_cam_set - RPG_BAT overlay, Address: 0x8014E5B4, Size: 152 bytes */
/* absorbed by kagami_target_set (`@SymbolStart: 0x8014E64C`, `@SymbolSize: 304`, */
/* byte-perfect), so the entry is a ghost.  @SymbolSize: 152 windows the whole */
/* @SymbolSize: 152 */
#include "types.h"

extern s32 rpg_bat_target_x;
extern s32 rpg_bat_target_y;

void rpg_bat_cam_set(s32 arg0)
{
    switch (arg0) {
    case 0:
        rpg_bat_target_x = 120;
        rpg_bat_target_y = 104;
        break;
    case 1:
        rpg_bat_target_x = 64;
        rpg_bat_target_y = 88;
        break;
    case 2:
        rpg_bat_target_x = 56;
        rpg_bat_target_y = 136;
        break;
    case 3:
        rpg_bat_target_x = 72;
        rpg_bat_target_y = 96;
        break;
    }
}
