/* Address: 0x80143BB0 */
/* Size: 264 bytes */
#include "types.h"

typedef struct {
    u32 ptrs[55];
} JumpTable;

extern JumpTable D_8012362C;
extern u8 D_800B1AF6;
extern s32 D_800B1764;
extern s32 D_80123498;
extern s32 ev_flag04;

extern u8 D_8011F9E4;
extern u8 D_8011F9EC;
extern u8 D_8011F9F0;

typedef void (*func_ptr_t)(s32);

extern void func_80078950();
extern void func_80012D64();

void func_80143BB0(void) {
    s32 flag;
    JumpTable localTable;

    localTable = D_8012362C;

    func_80078950(&D_8011F9E4, D_800B1AF6, ev_flag04);

    ((func_ptr_t)localTable.ptrs[D_800B1AF6])(128);

    flag = D_800B1764;
    if (flag & 0x100) {
        func_80012D64(D_80123498, 17, 1, 0, 0);
        func_80078950(&D_8011F9EC);
    }

    flag = D_800B1764;
    if (flag & 0x800) {
        func_80012D64(D_80123498, 17, 1, 2, 0);
        func_80078950(&D_8011F9F0);
    }
}
