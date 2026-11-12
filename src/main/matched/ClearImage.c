/* Address: 0x800A0070 */
/* Size: 112 bytes (28 instructions) */
/* Region: Library (>= 0x80086000) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: patch_stack_complete --old 24 --new 40 --shift-ra */
/* @PostProc: rename_register_chain --chain t6:v0 --chain v0:t2 --chain t2:t1 --chain t1:t0 --chain t0:t9 --chain t9:t8 --chain t8:t7 --chain t7:t6 */
/* @PostProc: convert_or_to_addu */
#include "types.h"

typedef struct {
    s32 field_00;
    s32 field_04;
    s32 (*func_ptr)();
    s32 field_0C;
} SomeStruct_A0070;

extern SomeStruct_A0070 *graph_jtb;
extern s32 libgpu_clearimage_name_str;

extern void checkRECT(s32 *ptr, s32 val);

void ClearImage(s32 a0, u8 a1, u8 a2, u8 a3) {
    SomeStruct_A0070 *ptr;
    s32 (*fptr)();

    checkRECT(&libgpu_clearimage_name_str, a0);
    ptr = graph_jtb;
    fptr = ptr->func_ptr;
    fptr(ptr->field_0C, a0, 8, (a3 << 16) | (a2 << 8) | a1);
}
