/* @SymbolSize: 752 */
#include "types.h"

typedef struct {
    u8  b0;
    u8  b1;
    u8  b2;
    u8  b3;
    u8  b4;
    u8  b5;
    u8  pad6[6];
    s32 w12;
    s32 w16;
    u8  pad20[4];
    s16 h24;
    u8  pad26[12];
    s16 h38;
    u8  pad40[2];
    s16 h42;
    u8  pad44[8];
    s32 w52;
    s32 w56;
    u8  pad60[7];
    u8  b67;
} Actor;

extern Actor class_w[];

extern s32 tpage_set_adr;
extern s32 img_set_adr;
extern s32 anim_set_adr;

extern void class_clear_f(void);

void name_ent_key_init(void)
{
    s32 i;

    class_clear_f();

    for (i = 0; i < 12; i++) {
        class_w[96 + i].b1 = 2;
        class_w[96 + i].b2 = 64;
        class_w[96 + i].w56 = 0;
        class_w[96 + i].b3 = 132;
        class_w[96 + i].w12 = img_set_adr;
        class_w[96 + i].w16 = anim_set_adr;
        class_w[96 + i].w52 = tpage_set_adr;
        class_w[96 + i].b4 = 8;
        class_w[96 + i].b5 = 129;
        class_w[96 + i].b67 = 16;
        class_w[96 + i].h38 = 400;
    }

    class_w[96].h24 = 1;
    class_w[96].h38 = 0;
    class_w[96].h42 = 0;

    class_w[97].h24 = 13;
    class_w[97].h38 = -72;
    class_w[97].h42 = -74;

    class_w[98].h24 = 13;
    class_w[98].h38 = -56;
    class_w[98].h42 = -74;

    class_w[99].h24 = 13;
    class_w[99].h38 = -24;
    class_w[99].h42 = -74;

    class_w[100].h24 = 13;
    class_w[100].h38 = -8;
    class_w[100].h42 = -74;

    class_w[101].h24 = 13;
    class_w[101].h38 = 104;
    class_w[101].h42 = -74;

    class_w[102].h24 = 13;
    class_w[102].h38 = 120;
    class_w[102].h42 = -74;

    class_w[103].h24 = 9;
    class_w[103].h38 = -143;
    class_w[103].h42 = 88;
    class_w[103].b1 = 1;

    class_w[104].h24 = 5;
    class_w[104].h38 = 0;
    class_w[104].h42 = 0;
    class_w[104].b1 = 3;

    class_w[105].h24 = 23;
    class_w[105].h38 = -143;
    class_w[105].h42 = 88;

    class_w[106].h24 = 7;
    class_w[106].h38 = -143;
    class_w[106].h42 = 88;
    class_w[106].b3 &= 0x7F;

    class_w[107].h24 = 9;
    class_w[107].h38 = -143;
    class_w[107].h42 = -40;
    class_w[107].b1 = 1;
}
