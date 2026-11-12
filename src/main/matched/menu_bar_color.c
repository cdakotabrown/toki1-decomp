/* Address: 0x80050F20 */
/* Size: 84 bytes (21 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: scheduler_normalizer --address 0x80050F20 --block-aware */
/* @PostProc: rename_register_chain --chain t1:t2:t3:t4 */
#include "types.h"

u32 menu_bar_rgb[48];

void menu_bar_color(s32 a0, u32 a1, u32 a2, u32 a3) {
    if (a0 >= 16) {
        menu_bar_rgb[0] = 0x832F7FAFu;
        menu_bar_rgb[1] = 0x822F7F2Fu;
        return;
    }
    menu_bar_rgb[a0] = ((a2 & 0xfu) << 24) + a1 + (a3 << 28);
}
