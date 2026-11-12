/* @RodataBase: 0x8015B9F4 */
/* @SymbolSize: 428 */
/* rpg_bat_random_quad_pick - RPG_BAT overlay, Address: 0x80139E88, Size: 428 bytes (107 insns) */
#include "types.h"

typedef struct {
    s32 w[4];
} Quad4;

extern Quad4 rpg_bat_random_quad_pick_tbl0;
extern Quad4 rpg_bat_random_quad_pick_tbl1;
extern s32 rpg_bat_msg_tbl4[];
extern s32 rpg_bat_random_quad_pick_str0[];
extern s32 rpg_bat_seq_work[];

extern s32 get_rand_mod();
extern void strcpy();
extern void strcat();

void
rpg_bat_random_quad_pick(void)
{
    Quad4 sp128;
    s32 i;
    s32 j;
    s32 n;
    Quad4 sp100;
    s32 k;

    sp128 = rpg_bat_random_quad_pick_tbl0;
    sp100 = rpg_bat_random_quad_pick_tbl1;
    i = 0;
    if (get_rand_mod(5) == 0) {
        n = 3;
    } else {
        n = 2;
    }
    for (j = 0; j < n; j++) {
        switch ((u32)get_rand_mod(10)) {
        case 0:
        case 1:
        case 2:
        case 3:
            sp100.w[0] = 1;
            break;
        case 4:
        case 5:
        case 6:
            sp100.w[1] = 1;
            break;
        case 7:
        case 8:
            sp100.w[2] = 1;
            break;
        case 9:
            sp100.w[3] = 1;
            break;
        }
    }
    k = 0;
    do {
        if (sp100.w[k] == 1) {
            strcpy(rpg_bat_msg_tbl4[i + 3], rpg_bat_random_quad_pick_str0);
            strcat(rpg_bat_msg_tbl4[i + 3], sp128.w[k]);
            i++;
        }
        k++;
    } while (k != 4);
    rpg_bat_seq_work[40] = i + 3;
}
