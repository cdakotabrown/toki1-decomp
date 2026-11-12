/* Address: 0x800493C0 */
/* Size: 168 bytes (42 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s32 activeBuff;
extern u8 ot_buff0[];
extern s16 GetTPage(s32, s32, s32, s32);
extern s32 prim_alloc(s32, s32);
extern void SetDrawMode(s32, s32, s32, u16, s32);
extern void AddPrim(s32 *, s32);
extern void safe_env(s32);

void goto_tpage(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 result2;
    u16 result1;
    s32 *addr;

    result1 = GetTPage(arg4, arg3, arg0, arg1);
    result2 = prim_alloc(12, activeBuff);
    SetDrawMode(result2, 0, 0, result1, 0);
    addr = (s32 *)((s32)ot_buff0 + (activeBuff << 10) + (arg2 << 2));
    AddPrim(addr, result2);
    safe_env(arg2);
}
