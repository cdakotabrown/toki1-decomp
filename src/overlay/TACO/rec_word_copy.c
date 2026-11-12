/* Size: 68 bytes */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @CompileEnv: IDO52_NO_LOOP_TEST_REPLACEMENT=1 */
/* @SymbolSize: 68 */
/* @PostProc: patch_insn_reg_field 0x8:rt:v0:a2 0xc:rs:v0:a2 0x18:rt:a0:v0 0x1c:rs:a0:v0 0x1c:rt:a0:v0 0x28:rt:v0:a2 0x2c:rs:a0:v0 0x2c:rt:a0:v0 0x38:rs:a0:v0 */
#include "types.h"

extern s32 taco_node_prim_tbl_1[];

void rec_word_copy(s32 *obj)
{
    u32 i;
    u32 n;
    u8 *p;

    n = obj[2] + 1;
    p = (u8 *)obj + 28;
    for (i = 1; i < n; i++) {
        taco_node_prim_tbl_1[i - 1] = *(s32 *)p;
        p += 28;
    }
}
