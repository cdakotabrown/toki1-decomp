/* @SymbolSize: 204 */
/* @PostProc: patch_insn_reg_field 0x7c:rt:t4:v1 0x80:rs:t4:v1 0x80:rt:t4:v1 0x84:rt:t8:t5 0x88:rd:t5:t4 0x88:rt:t4:v1 0x8c:rt:t5:t4 0x94:rs:t8:t5 0x94:rt:t8:t5 0x98:rs:t8:t5 0x9c:rt:t7:t8 0xa0:rs:t7:t8 0xa0:rt:t7:t8 0xa4:rs:t7:t8 0xa8:rt:t6:t7 0xac:rs:t6:t7 0xac:rt:t6:t7 0xb4:rt:t1:t0 0xb8:rs:t6:t7 0xb8:rt:t0:t6 0xbc:rs:t1:t0 0xbc:rt:t1:t0 0xc0:rt:t0:t6 0xc8:rt:t1:t0 */
/* Address: 0x80140890 */
/* Size: 204 bytes (51 instructions) */
#include "types.h"

typedef struct {
    void (*f[31])(s32);
} FnTable31;

extern FnTable31 shougatu_main4_tbl;
extern u8 now_sub_sub_step;
extern u8 cal_cw_disp;
extern u8 cal_cw_bright;
extern u8 fade_bri;

extern void fade_in_step(s32);
extern void fade_out_step(s32);

void shougatu_case4_fade_step(void)
{
    s32 pad_hi;
    FnTable31 tbl;

    tbl = shougatu_main4_tbl;
    tbl.f[now_sub_sub_step](128);
    if (tbl.f[now_sub_sub_step] == fade_in_step ||
        tbl.f[now_sub_sub_step] == fade_out_step) {
        cal_cw_disp |= 0x80;
        cal_cw_bright = fade_bri;
    }
}
