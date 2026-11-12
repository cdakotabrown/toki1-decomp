/* @CompileEnv: IDO52_FORCE_WEB_COLOR=19:a3 */
/* Address: 0x80148234, Size: 540 bytes (135 insns) */
/* @PostProc: swap_instructions --offset1 0x16c --offset2 0x170 */
/* @PostProc: swap_instructions --offset1 0x1fc --offset2 0x200 */
/* @PostProc: swap_instructions --offset1 0x204 --offset2 0x208 */
/* @PostProc: patch_insn_reg_field 0xd4:rt:t3:t2 0xdc:rs:t3:t2 0xe4:rs:t3:t2 0x114:rs:t3:t2 0x19c:rs:t8:v0 0x19c:rt:v0:t8 0x1a0:rd:t1:t6 0x1a4:rs:t1:t6 0x1a4:rt:t6:t7 0x1ac:rd:t7:t8 0x1ac:rs:t6:t7 0x1b0:rd:t8:t9 0x1b0:rs:t7:t8 0x1b4:rd:t9:t6 0x1b4:rt:t8:t9 0x1b8:rd:t6:t7 0x1b8:rt:t9:t6 0x1bc:rd:t7:t8 0x1bc:rs:t6:t7 0x1c0:rt:t7:t8 0x1c4:rt:t8:t9 0x1c8:rd:t7:t8 0x1cc:rd:t9:t6 0x1cc:rt:t8:t9 0x1d0:rd:t6:t7 0x1d0:rs:t9:v0 0x1d0:rt:v0:t6 0x1d4:rs:t6:t7 0x1e0:rd:t8:t9 0x1e0:rs:t7:t8 0x1e4:rd:t9:t6 0x1e4:rt:t8:t9 0x1e8:rd:t6:t7 0x1e8:rt:t9:t6 0x1ec:rd:t7:t8 0x1ec:rs:t6:t7 0x1f0:rt:t7:t8 0x200:rt:t8:t7 0x208:rs:t8:t7 */
#include "types.h"

typedef struct {
    u32 addr : 24;
    u32 len : 8;
} P_TAG;

extern u8 *tt_str_ptr_tbl[];
extern u8 *tt_step_wk;
extern u8 *tt_disp_wk;
extern u8 *tt_db_buf;

void tt_qstr_disp(u8 *req)
{
    u8 *db;
    u8 *str;
    u8 *p;
    u8 *q;
    P_TAG *otp;
    s32 ch;
    s16 x;
    u16 n;
    u16 f;

    db = tt_db_buf + *(s32 *)(tt_disp_wk + 8) * 32952;
    str = tt_str_ptr_tbl[*(u16 *)(req + 14)];
    ch = str[0];
    x = *(s16 *)(req + 10);
    n = 0;
    str += 1;
    if (ch == 255) {
        return;
    }

    do {
        if (req[1] != 0) {
            if ((u32)n >= (u32)*(u16 *)(req + 18)) {
                return;
            }
        }
        if (ch != 63) {
            p = *(u8 **)(db + 4);
            *(u8 **)(db + 4) = *(u8 **)(db + 4) + 16;
            f = *(u16 *)(req + 6);
            if ((f & 4) != 0) {
                p[3] = 3;
                p[7] = 116;
                p[4] = req[3];
                p[5] = req[4];
                p[6] = req[5];
            } else if ((f & 8) != 0) {
                q = tt_step_wk;
                p[7] = 116;
                p[3] = 3;
                p[4] = q[29];
                p[5] = q[30];
                p[6] = q[31];
            } else {
                p[3] = 3;
                p[7] = 117;
            }
            *(s16 *)(p + 8) = x;
            *(s16 *)(p + 10) = *(s16 *)(req + 12);
            p[12] = (u8)((ch & 15) << 3);
            p[13] = (u8)(((u32)ch >> 4) << 3);
            *(s16 *)(p + 14) = (s16)((req[2] & 15) + 15360);

            otp = (P_TAG *)(db + *(u16 *)(req + 16) * 4 + 32768);
            ((P_TAG *)p)->addr = otp[2].addr;
            otp = (P_TAG *)(db + *(u16 *)(req + 16) * 4 + 32768);
            otp[2].addr = (u32)p;
        }
        n += 1;
        ch = str[0];
        x = (s16)(x + 8);
        str += 1;
    } while (ch != 255);
}
