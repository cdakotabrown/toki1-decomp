/* @SymbolSize: 172 */
/* @PostProc: remap_stack_slots --map '40:44' */
/* @PostProc: patch_insn_reg_field 0x48:rt:a1:t8 0x54:rt:a3:t0 0x6c:rt:a1:t8 0x70:rt:a3:t0 0x84:rt:t8:t9 0x88:rt:t0:t1 0x8c:rt:t8:t9 0x90:rt:t0:t1 */
#include "types.h"

typedef struct {
    s32 w;
} W;

extern u8 *tt_bell_buf;

extern u8 *tt_obj_alloc(u8 *a0, u8 *a1);
extern void tt_obj_rec_init(u8 *a0);

void tt_obj_make(u8 *src)
{
    u8 *base;
    u8 *obj;
    s16 x;
    s16 y;

    base = tt_bell_buf;
    obj = tt_obj_alloc(base, base + 3456);

    *(W *)(obj + 32) = *(W *)(src + 32);
    x = *(s16 *)(obj + 34);
    *(W *)(obj + 36) = *(W *)(src + 36);
    y = *(s16 *)(obj + 38);
    *(W *)(obj + 40) = *(W *)(src + 40);
    *(W *)(obj + 44) = *(W *)(src + 44);
    *(s32 *)(obj + 48) = 0;
    *(s32 *)(obj + 52) = 0;
    obj[89] = 0;
    *(s16 *)(obj + 20) = x;
    *(s16 *)(obj + 22) = y;

    tt_obj_rec_init(obj);

    *(s16 *)(obj + 80) = 1;
    *(s16 *)(obj + 4) = 32;
    *(s16 *)(obj + 10) = 0;
    *(s16 *)(obj + 8) = 0;
    *(s16 *)(obj + 6) = 0;
    obj[15] = 0;
    obj[14] = 0;
    obj[13] = 0;
}
