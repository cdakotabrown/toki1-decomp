/* Address: 0x8013E118 */
/* Size: 176 bytes */
#include "types.h"

typedef struct {
    u32 ptrs[39];
} JumpTable;

extern JumpTable D_80122454;
extern u8 D_800B1AF6;
extern u8 D_8011F848;

typedef void (*func_ptr_t)(s32);

extern void func_80078950();
extern void func_8010AFE4(void);

void func_8013E118(void) {
    u8 charId;
    JumpTable localTable;

    localTable = D_80122454;

    func_80078950(&D_8011F848, D_800B1AF6);

    ((func_ptr_t)localTable.ptrs[D_800B1AF6])(128);

    charId = D_800B1AF6;
    if ((u32)charId >= 15 && (u32)charId < 18) {
        func_8010AFE4();
    }
}
