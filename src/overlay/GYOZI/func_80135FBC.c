/* @SymbolStart: 0x80135F48 */
/* @SymbolSize: 164 */
/* @PostProc: swap_instructions --offset1 0x48 --offset2 0x50 */
/* @PostProc: swap_instructions --offset1 0x4c --offset2 0x50 */
/* @PostProc: swap_instructions --offset1 0x54 --offset2 0x58 */
/* @PostProc: swap_instructions --offset1 0x60 --offset2 0x68 */
/* @PostProc: swap_instructions --offset1 0x64 --offset2 0x68 */
/* @PostProc: swap_instructions --offset1 0x6c --offset2 0x70 */
/* @PostProc: swap_instructions --offset1 0x74 --offset2 0x78 */
/* @PostProc: swap_instructions --offset1 0x88 --offset2 0x8c */
/* @PostProc: patch_insn_reg_field 0x3c:rt:t9:t5 0x40:rs:t9:t5 0x40:rt:t9:t5 0x48:rt:t2:v0 0x50:rt:t0:t6 0x54:rs:t2:v0 0x54:rt:t2:v0 0x58:rs:t0:t6 0x58:rt:t0:t6 0x5c:rt:t9:t5 0x60:rt:t4:t2 0x68:rt:t1:t7 0x6c:rs:t4:t2 0x6c:rt:t4:t2 0x70:rs:t1:t7 0x70:rt:t1:t7 0x74:rd:t3:t9 0x74:rt:t2:v0 0x78:rt:t0:t6 0x80:rd:t3:t1 0x80:rs:t3:t9 0x80:rt:t2:v0 0x88:rd:t5:v0 0x88:rs:t3:t1 0x88:rt:t4:t2 0x8c:rt:t1:t7 0x94:rs:t5:v0 0x94:rt:t6:t4 0xa0:rt:t6:t4 */
/* func_80135FBC -- GYOZI overlay, Address: 0x80135F48 (prologue 0x80135FBC), */
/* Size: 164 bytes (41 insns).  Saves the three words D_800D9258/5C/60 into */
#include "types.h"

extern void func_8004DE1C(void);

extern volatile s32 D_800D9258;
extern volatile s32 D_800D925C;
extern volatile s32 D_800D9260;
extern s32 D_80146358;
extern s32 D_8014635C;
extern s32 D_80146360;
extern s32 D_8014624C;
extern s32 D_80146280;
extern s32 D_801462B4;
extern u8 D_800F62CF;
extern u8 D_800F53DE;
extern s16 D_800D9248;

void func_80135FBC(void)
{
    D_80146358 = D_800D9258;
    D_8014635C = D_800D925C;
    D_80146360 = D_800D9260;
    D_800D9258 = D_8014624C;
    D_800D925C = D_80146280;
    D_800D9260 = D_801462B4;
    D_800D9248 = D_800F62CF * 3 + D_800F53DE - 96;
    func_8004DE1C();
}
