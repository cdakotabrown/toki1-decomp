/* Address: 0x8006A0F0 */
/* Size: 648 bytes (162 instructions) */
/* Region: Game (IDO 5.2) */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=4:v1 */
/* @CompileFlags: -cckr */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_branch_operands --offsets 0xb0 0x10c 0x16c 0x1c8 */
#include "types.h"

typedef struct {
    u8 b0 : 1;
    u8 b1 : 1;
    u8 b2 : 1;
    u8 b3 : 1;
    u8 b4 : 1;
    u8 b5 : 1;
    u8 b6 : 1;
    u8 b7 : 1;
} Bits;

extern u8 now_year;
extern u8 now_month;
extern Bits mouse_pad_st;
typedef union {
    Bits b;
    u8 u;
} Flags;
extern Flags magazine_flag;

magazine_flag_set(a, b)
{
    s32 mode, key;

    mode = now_month;
    if (mode != 2 && mode != 5 && mode != 8 && mode != 11) {
        magazine_flag.b.b6 = 1;
    } else {
        magazine_flag.b.b5 = 1;
    }
    key = now_year;
    if (key == 95 && mode == 4) {
        magazine_flag.b.b5 = 1;
        magazine_flag.b.b6 = 1;
    }
    switch (mode) {
    case 2: case 3: case 4:
        if (key == 95) mouse_pad_st.b0 = 1;
        if (key == 96) mouse_pad_st.b4 = 1;
        if (key == 97) magazine_flag.b.b0 = 1;
        break;
    case 5: case 6: case 7:
        if (key == 95) mouse_pad_st.b1 = 1;
        if (key == 96) mouse_pad_st.b5 = 1;
        if (key == 97) magazine_flag.b.b1 = 1;
        magazine_flag.b.b7 = 1;
        break;
    case 8: case 9: case 10:
        if (key == 95) mouse_pad_st.b2 = 1;
        if (key == 96) mouse_pad_st.b6 = 1;
        if (key == 97) magazine_flag.b.b2 = 1;
        break;
    case 11: case 12:
        if (key == 95) mouse_pad_st.b3 = 1;
        if (key == 96) mouse_pad_st.b7 = 1;
        if (key == 97) magazine_flag.b.b3 = 1;
        break;
    case 1:
        if (key == 96) mouse_pad_st.b3 = 1;
        if (key == 97) mouse_pad_st.b7 = 1;
        if (key == 98) magazine_flag.b.b3 = 1;
        break;
    }
}
