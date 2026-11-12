/* @PostProc: patch_stack_complete --old 248 --new 264 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
/* Address: 0x801447F8 */
/* Size: 240 bytes (60 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[55];
} JumpTable;

extern u8 D_800B1AF6;
extern u8 D_800EB02B;
extern char D_8011FA54;
extern u8 D_80123988;
extern s32 D_8012398C;
extern JumpTable D_80123AA0;

extern void func_80108AD8(void);
extern void func_80108DD4(void);
extern void func_80078950(char *s, u8 a1);

void func_801447F8(void)
{
    JumpTable table;

    table = D_80123AA0;

    table.ptrs[D_800B1AF6](128);

    if (D_80123988 != 0) {
        if ((u32)D_800B1AF6 >= 27U) {
            func_80108AD8();
        }
    }

    if (D_8012398C != 0) {
        if ((u32)D_800B1AF6 >= 27U) {
            func_80108DD4();
        }
    }

    func_80078950(&D_8011FA54, D_800EB02B);
}
