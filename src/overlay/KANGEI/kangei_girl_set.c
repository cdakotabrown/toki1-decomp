/* @PostProc: patch_insn_reg_field 0x10:rt:t6:a0 0x14:rs:t6:a0 0x14:rt:t6:a0 0x28:rt:t6:a0 */
/* @SymbolSize: 60 */
/* kangei_girl_set - KANGEI overlay, Address: 0x801325D8, Size: 52 bytes (13 insns) */
#include "types.h"

extern u8 now_girl;
extern void date_name_set(void);
extern void kangei_ptr_tbl_init(void);
extern void kangei_girl_ptr_set(void);
extern u8 girl_no;

void kangei_girl_set(void)
{
    u8 a0 = girl_no;

    now_girl = a0;
    date_name_set();
    kangei_ptr_tbl_init();
    kangei_girl_ptr_set();
}
