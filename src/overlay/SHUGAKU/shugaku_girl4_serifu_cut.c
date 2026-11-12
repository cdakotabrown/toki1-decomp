/* @SymbolSize: 112 */
/* @PostProc: patch_insn_reg_field 0x50:rt:v0:t3 0x54:rs:v0:t3 0x54:rt:v0:t3 0x5c:rd:t3:t4 0x5c:rt:v0:t3 0x60:rd:t4:t5 0x60:rt:t3:t4 0x64:rs:t4:t5 0x64:rt:t5:t6 0x6c:rs:t5:t6 */
#include "types.h"

extern void func_80138664(void);

extern u8  now_girl;
extern u16 basyo_pack;
extern u8  girl4_basyo;
extern volatile s16 now_serifu_shugaku;
extern volatile s32 *ss_serifu_shugaku_kind;

void shugaku_girl4_serifu_cut(void)
{
    s32 idx;
    s32 *ptr;

    func_80138664();
    if (now_girl != 4) return;
    if (((u32)girl4_basyo >> 4) != ((u32)basyo_pack >> 12)) return;
    idx = now_serifu_shugaku;
    ptr = ss_serifu_shugaku_kind;
    *(u8 *)((u8 *)ptr[idx] + 3) = 255;
}
