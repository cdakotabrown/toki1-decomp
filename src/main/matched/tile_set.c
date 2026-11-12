/* Function: tile_set */
/* Address: 0x8007D8CC */
/* Size: 240 bytes (60 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s32 activeBuff;
extern u8 ot_buff0[];

extern void *prim_alloc(s32 size, s32 a1);
extern void SetTile(void *a0);
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
    s16 field_C;
    s16 field_E;
} SpriteStruct16;

void tile_set(s16 arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4, u8 arg5, u8 arg6, u8 arg7) {
    SpriteStruct16 *s0;

    s0 = prim_alloc(16, activeBuff);
    SetTile(s0);
    SetSemiTrans(s0, 0);
    SetShadeTex(s0, 0);

    s0->field_4 = arg5;
    s0->field_5 = arg6;
    s0->field_6 = arg7;
    s0->field_8 = arg0;
    s0->field_A = arg1;
    s0->field_C = arg2;
    s0->field_E = arg3;

    if (activeBuff == 0) {
        s0->field_A += 240;
    }

    AddPrim((void *)((activeBuff << 10) + (arg4 * 4) + (u32)ot_buff0), s0);
}
