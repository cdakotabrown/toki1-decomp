/* Function: cdq_command_enqueue */
/* Address: 0x8004555C */
/* Size: 212 bytes (51 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: patch_stack_complete --old 24 --new 40 */
/* @PostProc: custom_script custom_func_8004555C.py */
#include "types.h"

extern u8 cdq_head;
extern u8 cdq_tail;
extern void *memcpy(void *dest, const void *src, s32 n);

typedef struct {
    u8 field_0;
    u8 pad1[3];
    s32 field_4;
    u8 data[16];
} QueueEntry;

extern QueueEntry cdq_que[];

s32 cdq_command_enqueue(u8 arg0, s32 arg1, void *arg2) {
    u32 head;
    u32 tail;
    QueueEntry *entry;
    s32 v;

    head = cdq_head;
    tail = cdq_tail;
    entry = &cdq_que[tail];

    if (head % 48 == (tail + 1) % 48) {
        return -1;
    }

    entry->field_0 = arg0;
    entry->field_4 = arg1;
    entry->data[15] = 0;

    if ((s32)arg2 == 1) {
        entry->data[15] = 255;
    } else if (arg2 != 0) {
        memcpy(entry->data, arg2, 16);
    }

    v = (cdq_tail + 1) & 0xFF;
    if ((u32)v < 48) {
        cdq_tail = v;
        return 1;
    }
    cdq_tail = 0;
}
