/* Address: 0x80042B10 */
/* Size: 148 bytes (37 instructions) */
/* Region: Game */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:0 */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern u8 now_step;
extern u8 prev_step;

s32 step_state_save_and_redirect(void) {
    u32 state;

    state = now_step;

    switch (state) {
    case 17:
    case 19:
    case 20:
    case 21:
    case 32:
    case 33:
    case 34:
    case 53:
    case 54:
    case 55:
    case 58:
    case 67:
    case 69:
    case 80:
    case 81:
    case 82:
    case 83:
    case 84:
    case 85:
    case 86:
    case 87:
    case 88:
    case 89:
    case 90:
    case 91:
    case 92:
    case 93:
    case 94:
    case 95:
    case 96:
    case 97:
    case 98:
    case 99:
    case 112:
    case 113:
    case 114:
    case 115:
    case 116:
    case 117:
    case 118:
    case 128:
    case 129:
    case 130:
    case 144:
    case 145:
    case 194:
    case 195:
    case 196:
        prev_step = now_step;
        now_step = 2;
        break;
    }
    return state;
}
