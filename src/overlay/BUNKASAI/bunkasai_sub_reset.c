/* @SymbolSize: 32 */
/* @PostProc: scheduler_normalizer --target-bin assets/overlays/BUNKASAI.EXN --address 0x80142BC8 */
/* @PostProc: patch_insn_reg_field 0x0:rt:t6:at 0x4:rs:t6:at 0x8:rt:t7:at 0xc:rs:t7:at 0x10:rt:t9:at 0x14:rt:t8:t6 0x1c:rs:t9:at 0x1c:rt:t8:t6 */
/* Address: 0x80142BC8 */
/* Size: 32 bytes (8 insns) */
#include "types.h"

#define now_sub_step (*(volatile u8 *)0x800EF869)
#define now_sub_sub_step (*(volatile u8 *)0x800EF86A)
#define bunka_state (*(volatile s32 *)0x8012B3A0)

void bunkasai_sub_reset(void) {
    now_sub_step = 0;
    now_sub_sub_step = 0;
    bunka_state = 8;
}
