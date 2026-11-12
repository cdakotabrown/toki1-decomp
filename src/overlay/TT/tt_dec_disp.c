/* @CompileEnv: IDO52_NO_LOOP_TEST_REPLACEMENT=1 */
/* @CompileFlags: -cckr -Wo,-loopunroll,0 -Wc,-pic0 */
/* tt_dec_disp - TT overlay, Address: 0x801489F4, Size: 360 bytes (90 insns) */
#include "types.h"

extern void tt_str_disp();

void tt_dec_disp(x, y, value, digits)
short x;
short y;
int value;
unsigned char digits;
{
    char buf[24];
    char *q;
    unsigned int i;
    int scale;
    int ten;

    scale = 1;
    if (digits == 0 || digits >= 11) {
        digits = 10;
    }
    ten = 10;
    for (i = 1; i < digits; i++) {
        scale = scale * ten;
    }
    if (value < 0) {
        buf[0] = '-';
        q = &buf[1];
        value = -value;
    } else {
        buf[0] = '+';
        q = &buf[1];
    }
    value = value % (scale * ten);
    for (i = 0; i < digits; i++) {
        *q++ = value / scale + '0';
        value = value % scale;
        scale = scale / ten;
    }
    *q = 0;
    tt_str_disp(x, y, buf);
}
