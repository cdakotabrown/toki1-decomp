/* @CompileFlags: -cckr -Wo,-loopunroll,0 */
#include "types.h"

typedef void (*Fn)(s32);
typedef struct { Fn f[67]; } Tab1;
typedef struct { Fn f[68]; } Tab2;

extern Tab1 kangei_table_pick_tbl0;
extern Tab2 kangei_table_pick_tbl1;
extern u8 now_sub_sub_step;
extern u8 girl0_basyo;
extern u16 basyo_pack;

void kangei_table_pick(void)
{
    s32 unused;
    Tab1 t1;
    Tab2 t2;

    t1 = kangei_table_pick_tbl0;
    t2 = kangei_table_pick_tbl1;
    if ((girl0_basyo >> 4) == (basyo_pack >> 12)) {
        t1.f[now_sub_sub_step](128);
    } else {
        t2.f[now_sub_sub_step](128);
    }
}
