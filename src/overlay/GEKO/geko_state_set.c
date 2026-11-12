/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:4 */
/* @SymbolSize: 216 */
/* @PostProc: patch_stack_complete --old 32 --new 48 */
/* @PostProc: remap_stack_slots --from 8 --to 20 */
/* @PostProc: patch_insn_reg_field 0x20:rt:v1:v0 0x24:rs:v1:v0 0x28:rt:a1:v1 0x2c:rs:a1:v1 0x38:rs:a1:v1 0x44:rs:a1:v1 0x50:rs:a1:v1 0x5c:rs:a1:v1 0x68:rs:a1:v1 0x9c:rs:a1:v1 */
/* geko_state_set -- GEKO overlay, Address: 0x80132A4C */
#include "types.h"

typedef struct {
    s16 h[11];
} Rec22;

extern Rec22 geko_state_set_tbl;
extern u8 geko_serifu_state;
extern u8 now_girl;
extern s32 frame_counter;

s32 geko_state_set(val)
s16 val;
{
    Rec22 buf;
    s32 cur;

    cur = geko_serifu_state;
    buf = geko_state_set_tbl;

    if (cur == 0) {
        return geko_serifu_state = (u8)val;
    }
    if (cur == 10) {
        if ((u32)(frame_counter & 7) >= (u32)buf.h[now_girl]) {
            return geko_serifu_state = (u8)val;
        }
        return cur;
    }
    if (frame_counter & 1) {
        return geko_serifu_state = (u8)val;
    }
    return cur;
}
