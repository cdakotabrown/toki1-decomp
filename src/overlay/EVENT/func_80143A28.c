/* Address: 0x80143A28 */
/* Size: 252 bytes (63 instructions) */
#include "types.h"

typedef struct {
    u32 ptrs[37];
} JumpTable;

extern JumpTable D_80123598;
extern u8 D_800B1AF6;
extern s32 D_800B1764;
extern s32 D_8012346C;

extern u8 D_8011F9D8;
extern u8 D_8011F9DC;
extern u8 D_8011F9E0;

typedef void (*func_ptr_t)(void);

extern void func_80078950();
extern void func_80012D64();

void func_80143A28(void) {
    s32 flag;
    JumpTable localTable;

    localTable = D_80123598;

    func_80078950(&D_8011F9D8, D_800B1AF6);

    ((func_ptr_t)localTable.ptrs[D_800B1AF6])();

    flag = D_800B1764;
    if (flag & 0x100) {
        func_80012D64(D_8012346C, 17, 1, 0, 0);
        func_80078950(&D_8011F9DC);
    }

    flag = D_800B1764;
    if (flag & 0x800) {
        func_80012D64(D_8012346C, 17, 1, 2, 0);
        func_80078950(&D_8011F9E0);
    }
}
