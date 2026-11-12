/* Address: 0x80056E18 */
/* Size: 148 bytes (37 instructions) */
/* Compiler: IDO 5.2 */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

typedef struct {
    u8 pad0[0x10];
    s16 frame0_x;
    s16 frame0_y;
    u8 pad1[4];
    s16 frame1_x;
    s16 frame1_y;
    u8 pad2[4];
    s32 state;
    s16 out_x;
    s16 out_y;
    u8 pad3[4];
    s32 flag;
} BlurState;

s32 strSync(BlurState *obj, s32 arg1)
{
    s32 counter;
    s32 *cp;

    cp = &counter;
    *cp = 0x800000;

    if (obj->flag != 0) {
        goto done;
    }

    do {
        *cp = *cp - 1;
        if (*cp == 0) {
            obj->flag = 1;
            if (obj->state != 0) {
                obj->state = 0;
            } else {
                obj->state = 1;
            }
            obj->out_x = *(s16 *)((u8 *)obj + obj->state * 8 + 16);
            obj->out_y = *(s16 *)((u8 *)obj + obj->state * 8 + 18);
        }
    } while (obj->flag == 0);

done:
    obj->flag = 0;
}
