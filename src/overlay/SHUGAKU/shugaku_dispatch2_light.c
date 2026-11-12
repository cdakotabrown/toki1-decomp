/* @CompileFlags: -Wc,-pic1 */
/* shugaku_dispatch2_light -- SHUGAKU overlay, Address: 0x80138AC0, Size: 184 bytes (46 insns) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[71];
} JumpTable;

extern void date_light_effect(void);

extern u8 shugaku_place;
extern u8 now_sub_sub_step;
extern JumpTable shugaku_main2_tbl;

void shugaku_dispatch2_light(void)
{
    u8 unused[16];
    JumpTable table;
    u32 v;

    table = shugaku_main2_tbl;

    if (shugaku_place == 1) {
        v = now_sub_sub_step;
        if (v >= 31U && v < 37U) {
            date_light_effect();
        }
    }
    table.ptrs[now_sub_sub_step](128);
}
