/* Address: 0x80049468, Size: 108 bytes (27 instructions) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: scheduler_normalizer */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s32 activeBuff;
extern u8  ot_buff0[];

extern void *prim_alloc(s32 size, s32 buf_idx);
extern void SetTile1(u8 *p);
extern void AddPrim(u8 *ot_entry, void *prim);

void safe_env(s32 arg0) {
    u8 *obj;

    obj = (u8 *)prim_alloc(12, activeBuff);
    SetTile1(obj);
    *(s16 *)(obj + 8)  = 0;
    *(s16 *)(obj + 10) = 0;

    AddPrim(&ot_buff0[(activeBuff << 10) + (arg0 << 2)], obj);
}
