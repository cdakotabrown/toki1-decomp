/* @CompileFlags: -Wc,-pic0 */
/* @CompileEnv: IDO52_COPYPROP_VAR=-4 IDO52_FORCE_WEB_COLOR=21:a2,26:v1,38:v0 */
/* @PostProc: patch_insn_reg_field 0x8c:rt:t2:a3 0x90:rs:t2:a3 0x90:rt:t2:a3 0x94:rt:a0:a2 0x98:rd:t3:t2 0x98:rt:t2:a3 0x9c:rs:a0:a2 0xa0:rd:t4:t3 0xa0:rt:t3:t2 0xa4:rs:t4:t3 0xa8:rd:t5:t4 0xac:rt:t5:t4 0xc0:rd:t8:t5 0xc4:rt:t8:t5 0xd8:rd:t6:t7 0xdc:rs:t6:t7 0xe4:rd:t9:t6 0xe8:rt:t9:t6 0xf8:rd:t1:t0 0xfc:rt:t1:t0 */
/* geko_cell_scan -- GEKO overlay, Address: 0x80133024, Size: 280 bytes (70 insns) */
#include "types.h"

extern void geko_cell_event(s32 mode, s32 idx);
extern void girl_date_near_set(void);

extern u8 now_year;
extern u8 now_month;
extern u8 now_day;
typedef struct { s32 w[6]; } Buf6;
extern Buf6 geko_cell_scan_tbl;
extern s8 geko_serifu_state;

void geko_cell_scan(void)
{
    s32 k;
    s32 pad;
    Buf6 buf;
    s16 *h;
    s16 s0;
    s32 v1;
    s32 t4;
    s32 a1;
    u8 a2, a3;

    h = (s16 *)&buf;
    buf = geko_cell_scan_tbl;
    if (now_year == 96) {
        h[1] = 29;
    }
    geko_serifu_state = 0;

    for (s0 = 1; s0 < 20; s0++) {
        a3 = now_month;
        a2 = now_day;
        v1 = h[a3 - 1];
        t4 = (s32)a2 + s0;
        k = s0;
        if ((u32)v1 >= (u32)t4) {
            a1 = (s16)(a2 + k - 1);
            geko_cell_event(0, a1);
        } else {
            a1 = (s16)(a2 + k - v1 - 1);
            geko_cell_event(1, a1);
        }
    }
    girl_date_near_set();
}
