/* Address: 0x80049574 */
/* Size: 128 bytes (32 instructions) */
/* Compiler: IDO 5.2 (in GCC region 0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern void *prim_alloc(s32, s32);
extern void SetDrawMode(void *, s32, s32, s32, s32);
extern void AddPrim(u8 *, void *);
extern void safe_env(s32);

extern s32 activeBuff;
extern u8 ot_buff0[];

void dtd_on(s32 arg0) {
    void *obj;

    obj = prim_alloc(12, activeBuff);
    SetDrawMode(obj, 0, 1, 0, 0);
    AddPrim(&ot_buff0[(activeBuff << 10) + (arg0 << 2)], obj);
    safe_env(arg0);
}
