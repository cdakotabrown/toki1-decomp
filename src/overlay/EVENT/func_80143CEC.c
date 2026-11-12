/* Address: 0x80143CEC */
/* Size: 252 bytes */
#include "types.h"

typedef struct {
    u32 ptrs[40];
} JumpTable;

extern JumpTable D_80123708;
extern u8 D_800B1AF6;
extern s32 D_800B1764;
extern s32 D_801234C4;

extern u8 D_8011F9F4;
extern u8 D_8011F9F8;
extern u8 D_8011F9FC;

typedef void (*func_ptr_t)(s32);

extern void func_80078950();
extern void func_80012D64();

void func_80143CEC(void) {
    s32 flag;
    JumpTable localTable;

    localTable = D_80123708;

    func_80078950(&D_8011F9F4, D_800B1AF6);

    ((func_ptr_t)localTable.ptrs[D_800B1AF6])(128);

    flag = D_800B1764;
    if (flag & 0x100) {
        func_80012D64(D_801234C4, 17, 1, 0, 0);
        func_80078950(&D_8011F9F8);
    }

    flag = D_800B1764;
    if (flag & 0x800) {
        func_80012D64(D_801234C4, 17, 1, 2, 0);
        func_80078950(&D_8011F9FC);
    }
}
