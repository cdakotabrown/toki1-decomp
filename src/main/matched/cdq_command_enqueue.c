/* Address: 0x8004555C (recorded) / real entry 0x80045534 (the cursor loads, */
/* Size: 252 bytes over the absorbed head */
/* Region: Game (IDO 5.2) */
/* @SymbolStart: 0x80045534 */
/* @SymbolSize: 252 */
/* @CompileFlags: -Wc,-pic1 -cckr */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    u8 type;
    u8 pad1[3];
    s32 arg;
    u8 data[15];
    u8 flag;
} Entry;

extern u8 cdq_head;
extern u8 cdq_tail;
extern Entry cdq_que[48];

cdq_command_enqueue(type, arg, data)
    u8 type;
    s32 arg;
    s32 data;
{
    if ((cdq_tail + 1) % 48 == cdq_head % 48) {
        return -1;
    }
    cdq_que[cdq_tail].type = type;
    cdq_que[cdq_tail].arg = arg;
    cdq_que[cdq_tail].flag = 0;
    if (data == 1) {
        cdq_que[cdq_tail].flag = 255;
    } else if (data != 0) {
        memcpy(cdq_que[cdq_tail].data, data, 16);
    }
    cdq_tail++;
    if (cdq_tail < 48) {
        return 1;
    }
    cdq_tail = 0;
}
