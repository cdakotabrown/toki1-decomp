/* Function: k_scroll_set */
/* Address: 0x8007EF3C */
/* Size: 268 bytes (67 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* Likely Symbol: step_wait (from o.bin) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: patch_stack_complete --old 64 --new 80 */
/* @PostProc: remap_stack_slots --map 40:56,52:72 */
/* @PostProc: strip_trailing_instructions --count 1 --nop-only */
#include "types.h"

typedef struct {
    s16 field_0;
    s16 field_2;
    u8 pad4[3];
    u8 field_7;
} Struct8;

extern u8 scroll_exist;
extern void get_ksys(void);
extern u8 get_now_k(void);
extern Struct8 *get_k_work(s32 idx);

void k_scroll_set(void) {
    s16 var_s0;
    u8 var_s2;
    s32 var_s1;
    Struct8 *sp48;
    s32 sp38;
    Struct8 *temp;

    get_ksys();
    var_s2 = get_now_k();

    sp48 = get_k_work(var_s2);
    sp38 = var_s2 + 1;
    get_k_work(sp38);

    if (sp48->field_2 >= 96) {
        if (scroll_exist != 0) {
            if ((u32)var_s2 >= 2) {
                var_s1 = 48;
                var_s0 = 1;
                do {
                    temp = get_k_work(var_s0);
                    if (temp->field_2 == var_s1) {
                        temp->field_7 = 0;
                    } else {
                        temp->field_2 -= 16;
                    }
                    var_s0 += 1;
                } while ((u32)var_s0 < (u32)var_s2);
            }
            scroll_exist = 0;
            sp48->field_2 = 80;
            sp48->field_7 = 1;
        }
    }

    temp = get_k_work(sp38);
    if (temp->field_2 >= 96) {
        scroll_exist = 1;
        temp->field_7 = 0;
    }
}
