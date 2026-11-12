/* @SymbolSize: 240 */
/* @PostProc: patch_stack_complete --old 32 --new 48 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
/* @PostProc: patch_insn_reg_field 0x54:rt:a2:a1 0x60:rs:a2:a1 0x6c:rs:a2:a1 0x78:rs:a2:a1 0x98:rs:a2:a1 0xb8:rs:a2:a1 0xc4:rs:a2:a1 0xd0:rs:a2:a1 */
#include "types.h"

extern void *prim_alloc(s32, s32);
extern void SetTile(void *);
extern void SetSemiTrans(void *, s32);
extern void AddPrim(s32);

extern volatile s32 activeBuff;
extern s16 screen_w[];
extern u8 ot_buff0[];

void kangei_sprite_disp(s32 a0, s32 a1, s32 a2, s32 a3, u8 arg5, u8 arg6,
                   u8 arg7, s32 arg8, s32 arg9)
{
    u8 *node;

    node = (u8 *)prim_alloc(16, activeBuff);
    SetTile(node);
    SetSemiTrans(node, arg9);

    node[4] = arg5;
    node[5] = arg6;
    node[6] = arg7;
    *(s16 *)(node + 8) =
        (s16)(*(s16 *)((u8 *)screen_w + activeBuff * 2 + 20) + a0);
    *(s16 *)(node + 10) =
        (s16)(*(s16 *)((u8 *)screen_w + activeBuff * 2 + 24) + a1);
    *(s16 *)(node + 12) = (s16)a2;
    *(s16 *)(node + 14) = (s16)a3;
    AddPrim((s32)(ot_buff0 + activeBuff * 1024 + arg8 * 4));
}
