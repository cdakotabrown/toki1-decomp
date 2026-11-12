/* Address: 0x800494D4 */
/* Size: 160 bytes (40 instructions) */
/* Compiler: IDO 5.2 (in GCC region 0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern void *prim_alloc(s32, s32);
extern void *GetTPage(s32, s32, s32, s32);
extern void SetDrawMode(void *, s32, s32, void *, s32);
extern void AddPrim(u8 *, void *);
extern void safe_env(s32);

extern s32 activeBuff;
extern u8 ot_buff0[];

void dtd_on_tpage(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    void *obj;

    obj = prim_alloc(12, activeBuff);
    SetDrawMode(obj, 0, 1, GetTPage(arg4, arg3, arg0, arg1), 0);
    AddPrim(&ot_buff0[(activeBuff << 10) + (arg2 << 2)], obj);
    safe_env(arg2);
}
