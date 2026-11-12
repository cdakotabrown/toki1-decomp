/* @CompileEnv: IDO52_CONST_INREG_VALUES=2,3,1:-1:3:3 */
/* @SymbolSize: 380 */
/* @PostProc: patch_stack_complete --old 96 --new 104 */
/* @PostProc: remap_stack_slots --map '88:92' */
/* @PostProc: patch_insn_reg_field 0x148:rt:s3:s6 */
/* Address: 0x80136FBC */
/* Size: 380 bytes (95 insns) */
/* - @CompileEnv: IDO52_CONST_INREG_VALUES=2,3,1:-1:3:3 builds the target's */
#include "types.h"

typedef struct {
    u16 w[3];
} Tbl6;

extern Tbl6 taiiku_racer_seat_step_tbl;
extern u8 taiiku_kari_run[];

extern s32 rand(void);
extern s32 taiiku_seat_walk(s32);
extern void game_reset(void);

void taiiku_racer_seat_step(void)
{
    Tbl6 buf;
    u8 *p;
    s32 i;
    u32 sel;
    u16 n;
    s32 r;

    buf = taiiku_racer_seat_step_tbl;
    p = taiiku_kari_run;
    for (i = 0; i < 3; i++) {
        if (p[37] != 0) {
            sel = p[41];
            switch (sel) {
            case 0:
                r = rand();
                *(s16 *)(p + 44) = (s16)(buf.w[i] + (r % 120) + 60);
                p[41] = p[41] + 1;
                break;
            case 1:
                if ((u32)(u16)(++*(u16 *)(p + 46)) >= (u32)*(u16 *)(p + 44)) {
                    p[41] = p[41] + 1;
                }
                break;
            case 2:
                if (taiiku_seat_walk(i + 1) != 0) {
                    p[37] = 0;
                    p[36] = 1;
                }
                break;
            default:
                game_reset();
                break;
            }
        }
        p += 36;
    }
}
