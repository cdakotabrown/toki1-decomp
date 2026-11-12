/* Function: _spu_write */
/* Address: 0x80095A8C */
/* Size: 104 bytes (26 instructions) */
/* Region: Library (0x80086000-0x800BFFFF) */
/* Compiler: IDO 5.2 (t6/t7/t8 temporaries, `b` not `j`) */
/* @CompileEnv: IDO52_AS1_KEEP_ANDI=all */
/* @PostProc: convert_or_to_addu */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: patch_stack_complete --old 24 --new 32 --shift-ra */
/* @PostProc: patch_insn_reg_field --patch 0x4:rt:t6:v1 --patch 0x8:rs:t6:v1 --patch 0x8:rt:t6:v1 --patch 0x14:rs:t6:v1 --patch 0x18:rt:t9:t6 --patch 0x1c:rt:t7:v0 --patch 0x20:rs:t7:v0 --patch 0x20:rt:t8:v0 --patch 0x24:rs:t6:v1 --patch 0x24:rt:t8:v0 --patch 0x28:rs:t9:t6 --patch 0x28:rt:t9:t6 --patch 0x30:rs:t9:t6 */
#include "types.h"

typedef struct {
    u8 pad[426];
    u16 field_1AA;
} SomeStruct;

extern SomeStruct *_spu_RXX;
extern s32 spu_transMode;

extern void _spu_w(s32 arg0, s32 arg1);
extern void _spu_writeByIO(s32 arg0, s32 arg1);

s32 _spu_write(s32 arg0, s32 arg1)
{
    SomeStruct *ptr;
    u16 val;

    ptr = _spu_RXX;
    val = ptr->field_1AA;
    val = val & 0xFFCF;
    ptr->field_1AA = val;

    if (spu_transMode == 1) {
        _spu_w(arg0, arg1);
    } else {
        _spu_writeByIO(arg0, arg1);
    }
    return arg1;
}
