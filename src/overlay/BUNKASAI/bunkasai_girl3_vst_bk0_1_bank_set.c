/* @SymbolSize: 52 */
/* @PostProc: cse_group_lui --keep-all */
/* @PostProc: scheduler_normalizer --target-bin assets/overlays/BUNKASAI.EXN --address 0x8013216c */
/* @PostProc: patch_insn_reg_field 0x20:rt:t0:t7 0x24:rs:t0:t7 0x24:rt:t0:t7 0x2c:rt:t0:t7 */
/* Address: 0x8013216c */
/* Size: 52 bytes (13 insns) */
#include "types.h"

#define bunkasai_serifu_ptr (*(volatile s32 *)0x80160500)
#define bunkasai_serifu_ptr_1 (*(volatile s32 *)0x80160504)
#define bunkasai_serifu_ptr_2 (*(volatile s32 *)0x80160508)

void bunkasai_girl3_vst_bk0_1_bank_set(void) {
    bunkasai_serifu_ptr = 2149510992;
    bunkasai_serifu_ptr_2 = 2149531388;
    bunkasai_serifu_ptr_1 = 2149512752;
}
