/* @SymbolSize: 260 */
/* @CompileEnv: IDO52_UGEN_TEMP_REG_INPLACE=occ:9 IDO52_UGEN_TEMP_REG=9:a0,10:t4,11:t5,12:t8,13:t7,14:t6,15:t0,16:t1 */
/* @PostProc: swap_instructions --offset1 0xbc --offset2 0xc0 */
/* @PostProc: swap_instructions --offset1 0xb8 --offset2 0xbc */
/* @PostProc: swap_branch_operands --offsets 0xa4,0xdc */
/* Address: 0x80134B84 */
/* Size: 260 bytes (65 insns) */
#include "types.h"
#include "tokimeki/class_w.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    s32 data[41];
} CopyStruct;

extern u8 now_sub_sub_step;
extern CopyStruct shugaku_maina_dispatch2_tbl;

extern u8 cal_cw_bright;
extern CLASS_W class_w[];

extern void date_fade_in(s32);
extern void date_fade_out(s32);

void shugaku_maina_dispatch2(void)
{
    s32 pad;
    CopyStruct local;
    s32 fn_ptr;
    u8 b;

    local = shugaku_maina_dispatch2_tbl;
    ((FuncPtr)local.data[now_sub_sub_step])(128);
    fn_ptr = local.data[now_sub_sub_step];
    if ((s32)date_fade_in == fn_ptr) {
        class_w[98].disp_flag = class_w[98].disp_flag | 0x80;
        class_w[98].bright = cal_cw_bright;
    }
    b = cal_cw_bright;
    if ((s32)date_fade_out == fn_ptr) {
        class_w[98].bright = b;
        if ((u32)b < 9U) {
            class_w[98].disp_flag = class_w[98].disp_flag & 0xFF7F;
        }
    }
}
