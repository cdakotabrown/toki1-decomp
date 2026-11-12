/* @SymbolStart: 0x80137138 */
/* @SymbolSize: 84 */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @CompileEnv: IDO52_LDA_NOINREG_CLASSES=1,4 */
/* @PostProc: patch_stack_complete --old 32 --new 40 */
/* @PostProc: remap_stack_slots --from 20 --to 24 --from 32 --to 36 */
/* @PostProc: swap_instructions --offset1 0x34 --offset2 0x3c */
/* @PostProc: swap_instructions --offset1 0x38 --offset2 0x3c */
/* @PostProc: patch_insn_reg_field 0x30:rt:a0:v1 0x34:rs:a0:v1 0x34:rt:a0:v1 0x38:rt:v0:a0 0x3c:rt:v1:v0 0x48:rs:v0:a0 0x48:rt:a1:t1 0x4c:rs:v0:a0 0x4c:rt:v0:a0 0x50:rs:a0:v1 0x50:rt:a0:v1 0x54:rs:v0:a0 0x54:rt:v1:v0 0x58:rs:a0:v1 0x58:rt:a1:t1 */
#include "types.h"

typedef struct { s32 w[3]; } Rec12;

extern Rec12 taiiku_slot_tbl_set_tbl;
extern u8 taiiku_kari_run[];

void taiiku_slot_tbl_set(void) {
    Rec12 buf;
    s32 *s;
    s32 *e;
    u8 *d;
    s32 t;

    buf = taiiku_slot_tbl_set_tbl;
    s = buf.w;
    e = buf.w + 3;
    d = taiiku_kari_run;
    do {
        t = *s;
        s++;
        d += 36;
        *(s32 *)(d + 16) = t;
    } while (s != e);
}
