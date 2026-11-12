/* @CompileFlags: -Wo,-loopunroll,0 */
/* @SymbolSize: 296 */
/* @PostProc: patch_insn_reg_field 0x20:rt:v1:a0 0x24:rs:v1:a0 0x24:rt:v1:a0 0x2c:rs:v1:a0 0x2c:rt:v0:v1 0x30:rs:v1:a0 0x30:rt:v1:a0 0x34:rs:v0:v1 0x38:rt:v1:a0 0x4c:rd:t9:t0 0x50:rd:t8:t9 0x54:rt:t7:t8 0x58:rs:t8:t9 0x5c:rs:t8:t9 0x5c:rt:t8:t9 0x60:rs:t9:t0 0x64:rs:t8:t9 0x68:rs:t9:t0 0x68:rt:t9:t0 0x6c:rs:t9:t0 0x70:rs:t8:t9 0x74:rs:t8:t9 0x74:rt:t7:t8 0x78:rs:t9:t0 0x7c:rs:t8:t9 0x80:rd:t2:t4 0x84:rs:t9:t0 0x88:rs:t8:t9 0x88:rt:t7:t8 0x8c:rd:t3:t5 0x90:rt:t1:t3 0x94:rs:t9:t0 0x94:rt:t7:t8 0x98:rs:t2:t4 0x9c:rs:t2:t4 0x9c:rt:t2:t4 0xa0:rs:t3:t5 0xa4:rs:t2:t4 0xa8:rs:t3:t5 0xa8:rt:t3:t5 0xac:rs:t3:t5 0xb0:rs:t2:t4 0xb4:rs:t2:t4 0xb4:rt:t1:t3 0xb8:rs:t3:t5 0xbc:rs:t2:t4 0xc0:rt:t4:t7 0xc4:rs:t3:t5 0xc8:rs:t2:t4 0xc8:rt:t1:t3 0xd0:rs:t3:t5 0xd0:rt:t1:t3 0xd4:rt:t4:t7 0xd8:rt:t5:t6 0xdc:rs:t5:t6 0xdc:rt:t5:t6 0xe4:rs:t5:t6 0xe8:rt:t6:t8 0xec:rs:t6:t8 0xec:rt:t6:t8 0xf4:rs:t6:t8 0x108:rt:t7:t9 0x114:rt:t7:t9 */
/* shougatu_serifuev_cw98_step - SHOUGATU overlay, Address: 0x8014272C, 296 bytes (74 insns) */
#include "types.h"
#include "tokimeki/class_w.h"

extern void shougatu_serifuev_run2();
extern s32 check_end_k();

typedef struct { s32 w[17]; } Blk68;

extern s32 step_counter;
extern u16 now_serifuev;
extern u16 now_serifuev_in;
extern s16 cw98_anim;
extern s16 cw98_frame;
extern CLASS_W class_w[];

void shougatu_serifuev_cw98_step(void)
{
    s32 a0;
    s32 f;

    shougatu_serifuev_run2();

    a0 = step_counter;
    f = (u32)a0 < 1U;
    step_counter = a0 + 1;
    if (!f) {
        goto after_copies;
    }
    (*(Blk68 *)&class_w[98]) = (*(Blk68 *)&class_w[96]);
    (*(Blk68 *)&class_w[98]) = (*(Blk68 *)&class_w[96]);
    cw98_anim = 4;

after_copies:
    if (now_serifuev != 7) {
        goto fail;
    }
    if (now_serifuev_in != 1) {
        goto fail;
    }
    if (check_end_k() != 0) {
        goto fail;
    }
    class_w[98].anim_flag = 1;
    goto end;

fail:
    class_w[98].anim_flag = 0;
    cw98_frame = 0;
end:
    return;
}
