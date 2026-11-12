/* Function: model_slot_set */
/* Address: 0x8005931C */
/* Size: 172 bytes (43 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: patch_stack_complete --old 40 --new 56 */
/* @PostProc: custom_script func_8005931C_fix.py */
#include "types.h"

typedef struct {
    s32 field_0x00;
    s32 field_0x04;
    u8 pad[8];
} Entry16;

typedef struct {
    s32 field_0x00;
    u8 pad[76];
} Entry80;

extern Entry16 object[];
extern Entry80 coord_buf[];

extern void GsMapModelingData(s32 *a0);
extern void GsLinkObject4(s32 *a0, Entry16 *a1, u32 a2);

s32 model_slot_set(s32 *a0, s32 a1, u32 a2) {
    s32 *ptr;
    u32 count;

    (void)&a0;

    ptr = a0 + 1;
    GsMapModelingData(ptr);

    count = (u32)ptr[1];
    if (count >= 17) {
        count = 15;
    }
    if (a2 < count) {
        GsLinkObject4(ptr + 2, &object[a1], a2);
        object[a1].field_0x04 = (s32)&coord_buf[a1];
        object[a1].field_0x00 = 0;
        return 1;
    }
    return -1;
}
