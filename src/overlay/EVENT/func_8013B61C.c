/* Address: 0x8013B61C */
/* Size: 244 bytes (61 instructions) */
#include "types.h"

typedef struct {
    u32 ptrs[48];
} JumpTable;

extern JumpTable D_80121AF8;

extern u8 D_800B1AF6;

extern volatile s32 ev_flag14;

extern u8 D_800EB02B;

extern volatile u8 D_800EB02F;

extern volatile u8 D_800F19AB;

extern void *D_8011F744;

typedef void (*func_ptr_t)(s32);

extern void func_80078950();
extern void func_8010AFE4(void);

void func_8013B61C(void) {
    u32 charId;
    JumpTable localTable;

    localTable = D_80121AF8;
    func_80078950(&D_8011F744, D_800B1AF6);
    ((func_ptr_t)localTable.ptrs[D_800B1AF6])(128);
    if (ev_flag14 != 0) {
        D_800EB02B |= 0x80;
        D_800EB02F = D_800F19AB;
    } else {
        D_800EB02B &= ~0x80;
    }
    charId = D_800B1AF6;
    if (charId >= 16 && charId < 19) {
        func_8010AFE4();
    }
}
