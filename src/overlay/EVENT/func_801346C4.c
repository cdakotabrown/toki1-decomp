/* Address: 0x801346C4 */
/* Size: 192 bytes */
#include "types.h"

typedef struct {
    u32 ptrs[35];
} JumpTable;

extern JumpTable D_8012092C;
extern u8 D_800B1AF6;
extern s32 ev_flag04;
extern u8 D_8011F5AC;

typedef void (*func_ptr_t)(s32);

extern void func_80078950();
extern void func_800F8878(void);
extern void func_800F8970(void);

void func_801346C4(void) {
    s32 pad_hi;
    JumpTable localTable;

    localTable = D_8012092C;

    func_80078950(&D_8011F5AC, D_800B1AF6, ev_flag04);

    ((func_ptr_t)localTable.ptrs[D_800B1AF6])(128);

    func_800F8878();

    if (D_800B1AF6 == 14) {
        func_800F8970();
    }
}
