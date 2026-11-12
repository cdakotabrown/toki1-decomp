/* @PostProc: patch_stack_complete --old 152 --new 176 */
/* @PostProc: remap_stack_slots --from 24 --to 44 */
/* Address: 0x8013A090 */
/* Size: 112 bytes (28 instructions) */
#include "types.h"

typedef struct {
    s32 words[32];
} JumpTable;

extern char D_8011F72C;
extern u8 D_800B1AF6;
extern JumpTable D_801218E8;

extern void func_80078950(char *s, u8 idx);

void func_8013A090(void) {
    JumpTable table;

    table = D_801218E8;
    func_80078950(&D_8011F72C, D_800B1AF6);
}
