/* Address: 0x80049C68 */
/* Size: 108 bytes (27 instructions) */
/* Compiler: GCC-like (0x80040000-0x80080000 region) */
#include "types.h"

extern s32 prim_alloc(s32, s32);
extern void *SetTile1(s32);
extern void AddPrim(void *);

extern s32 activeBuff;
extern u8 ot_buff0[];

typedef struct {
    u8 pad[8];
    s16 field_8;
    s16 field_A;
} AllocObj;

void func_80049C68(s32 arg0) {
    s32 tmp;
    AllocObj *obj;

    tmp = prim_alloc(12, activeBuff);
    obj = (AllocObj *)SetTile1(tmp);

    obj->field_8 = 0;
    obj->field_A = 0;

    AddPrim(&ot_buff0[(activeBuff << 10) + (arg0 << 2)]);
}
