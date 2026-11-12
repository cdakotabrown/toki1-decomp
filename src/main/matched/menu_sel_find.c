/* Address: 0x80050744 */
/* Size: 228 bytes (57 instructions) */
/* Region: Game */
/* @CompileEnv: IDO52_FORM_SITE=lda:0:24:0,lda:0:58:0 IDO52_FORCE_WEB_COLOR=62:a1,3:a2,21:a2 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: patch_insn_reg_field 0x0:rt:a3:a2 0x4:rs:a3:a2 0x4:rt:a3:a2 0xc:rs:a3:a2 0x10:rs:a3:a2 0x78:rs:a3:a2 */
#include "types.h"

extern u8 screen_w[];

s32 menu_sel_find(s32 a0, s32 a1) {
    u8 *base = screen_w;
    u8 *countRow;
    s32 i;
    u8 *rec;

    if (a1 != 0) {

        countRow = base + a0;
        i = countRow[4240] - 1;
        if (i < 0) {
            goto fail;
        }
        rec = &screen_w[(i << 4) + (a0 << 3)];
        do {
            if (rec[3988] != 0 && rec[3989] != 0) {
                countRow[4243] = (u8)i;
                return i;
            }
            i--;
            rec -= 16;
        } while (i >= 0);
        goto fail;
    } else {

        countRow = base + a0;
        i = 0;
        if (countRow[4240] == 0) {
            goto fail;
        }
        rec = &screen_w[a0 << 3];
        do {
            if (rec[3988] != 0 && rec[3989] != 0) {
                countRow[4243] = (u8)i;
                return i;
            }
            i++;
            rec += 16;
        } while ((u32)i < (base + 4240)[a0]);
    }
fail:
    return -1;
}
