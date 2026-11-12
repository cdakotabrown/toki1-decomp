/* @PostProc: swap_instructions --offset1 0x24 --offset2 0x28 */
/* @PostProc: patch_insn_reg_field 0x28:rt:a0:a1 0x2c:rt:a1:a0 0x38:rs:a1:a0 0x48:rt:a0:a1 0x60:rd:t9:t8 0x64:rs:t9:t8 0x64:rt:t0:t9 0x68:rs:t0:t9 0x78:rt:t1:t0 0x84:rd:t2:t1 0x84:rs:t1:t0 0x88:rt:t2:t1 0x94:rt:t3:t2 0x9c:rt:t3:t2 0xa4:rt:t4:t3 0xac:rt:t4:t3 */
#include "types.h"
#include "tokimeki/class_w.h"

extern s32 Vblnk_Timer_Init(void);

extern u8 taiiku_spoon_rank[4];
typedef struct { u8 pad[7108]; s32 f; } Blk271B0;
extern Blk271B0 class_w;

s32 taiiku_result_slide()
{
    s32 i;
    s32 found_idx;
    u8 *ptr;
    s32 one, lim;
    s32 val;

    ptr = taiiku_spoon_rank;
    one = 1;
    lim = 4;
    i = 0;

    do {
        if (one == *ptr) {
            found_idx = i;
        }
        i++;
        ptr++;
    } while (i != lim);

    val = (found_idx * 3) << 4;
    val = val - 64;

    if (val < ((CLASS_W *)&class_w)[104].x.half[1]) {
        volatile Blk271B0 *p = &class_w;
        p->f = p->f - 98304;
        return 0;
    } else {
        ((CLASS_W *)&class_w)[104].anim_flag = 64;
        ((CLASS_W *)&class_w)[104].frame = 61;
        Vblnk_Timer_Init();
        return 1;
    }
}
