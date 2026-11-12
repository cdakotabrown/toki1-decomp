/* @SymbolSize: 388 */
/* rpg_bat_manzai_msg_pick - RPG_BAT overlay, Address: 0x80150F84, */
/* Size: 388 bytes (97 insns) */
/* 0x80150F84`, and a `@CompileFlags: -Wc,-pic0` stamp for the invented */
#include "types.h"

typedef struct Tbl {
      s32 e[41];
} Tbl;

extern Tbl rpg_bat_manzai_msg_pick_tbl;
extern u8 rpg_bat_msg_buf[];
extern u8 rpg_bat_msg_buf_1[];
extern u8 rpg_bat_msg_buf_2[];

extern s32 strcpy(u8 *, s32);
extern s32 get_rand_mod(s32);

void rpg_bat_manzai_msg_pick(void)
{
    s32 x;
    s32 y;
    s32 z;
    Tbl t;

    t = rpg_bat_manzai_msg_pick_tbl;
    x = get_rand_mod(41);
    y = get_rand_mod(41);
    if (y == x) {
        y = (y + 1) % 41;
    }
    z = get_rand_mod(41);
    if (z == x) {
        z = (z + 1) % 41;
    }
    if (z == y) {
        z = (z + 1) % 41;
    }
    strcpy(rpg_bat_msg_buf, t.e[x]);
    strcpy(rpg_bat_msg_buf_1, t.e[y]);
    strcpy(rpg_bat_msg_buf_2, t.e[z]);
}
