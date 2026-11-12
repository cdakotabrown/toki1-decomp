/* Function: sprite_disp */
/* Address: 0x8007D7C4 */
/* Size: 264 bytes (66 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s32 activeBuff;
extern u8 ot_buff0[];

extern void *prim_alloc(s32 size, s32 a1);
extern void SetSprt(void *a0);
extern void SetSemiTrans(void *a0, s32 a1);
extern void SetShadeTex(void *a0, s32 a1);
extern void AddPrim(void *a0, void *a1);

typedef struct {
    u8 pad0[4];
    u8 field_4;
    u8 field_5;
    u8 field_6;
    u8 pad7;
    s16 field_8;
    s16 field_A;
    u8 field_C;
    u8 field_D;
    u8 pad_E[2];
    s16 field_10;
    s16 field_12;
} SpriteStruct20;

void sprite_disp(s16 arg0, s16 arg1, s16 arg2, s16 arg3, u8 arg4, u8 arg5, s16 arg6, u8 arg7, u8 arg8, u8 arg9) {
    SpriteStruct20 *s0;

    s0 = prim_alloc(20, activeBuff);
    SetSprt(s0);
    SetSemiTrans(s0, 0);
    SetShadeTex(s0, 0);

    s0->field_4 = arg7;
    s0->field_5 = arg8;
    s0->field_6 = arg9;
    s0->field_8 = arg0;
    s0->field_A = arg1;
    s0->field_10 = arg2;
    s0->field_12 = arg3;
    s0->field_C = arg4;
    s0->field_D = arg5;

    if (activeBuff == 0) {
        s0->field_A += 240;
    }

    AddPrim((void *)((activeBuff << 10) + (arg6 * 4) + (u32)ot_buff0), s0);
}
