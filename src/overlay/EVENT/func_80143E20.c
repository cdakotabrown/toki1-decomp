/* Address: 0x80143E20 */
/* Size: 256 bytes */
#include "types.h"

typedef struct {
    u32 ptrs[51];
} JumpTable;

extern JumpTable D_801237A8;
extern u8 D_800B1AF6;
extern s32 D_800B1764;
extern s32 D_801234F0;
extern u16 D_80094714;
extern u16 D_80094718;

extern u8 D_8011FA00;
extern u8 D_8011FA0C;
extern u8 D_8011FA10;

typedef void (*func_ptr_t)(s32);

extern void func_80078950();
extern void func_80012D64();

void func_80143E20(void) {
    s32 flag;
    JumpTable localTable;

    localTable = D_801237A8;

    func_80078950(&D_8011FA00, D_800B1AF6, D_80094714, D_80094718);

    ((func_ptr_t)localTable.ptrs[D_800B1AF6])(128);

    flag = D_800B1764;
    if (flag & 0x100) {
        func_80012D64(D_801234F0, 17, 1, 0, 0);
        func_80078950(&D_8011FA0C);
    }

    flag = D_800B1764;
    if (flag & 0x800) {
        func_80012D64(D_801234F0, 17, 1, 2, 0);
        func_80078950(&D_8011FA10);
    }
}
