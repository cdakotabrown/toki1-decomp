/* @SymbolSize: 212 */
/* Address: 0x80145FE4 */
/* Size: 212 bytes (53 instructions) */
#include "types.h"

extern void back_clear_switch();
extern void device_cmd_send();
extern s32 get_CD_State();
extern void set_XAsector();
extern u8 step_mode;

void etc_xa_load(void)
{
    back_clear_switch(1);
    switch (step_mode) {
    case 0:
        if (get_CD_State() & 4) {
            step_mode = step_mode + 1;
        }
        break;
    case 1:
        if (get_CD_State() & 2) {
            step_mode = step_mode + 1;
        }
        break;
    case 2:
        set_XAsector(0x1B000000, 0x01000004, 14);
        device_cmd_send(768);
        step_mode = step_mode + 1;
        break;
    case 3:
        break;
    }
}
