/* @CompileEnv: IDO52_AS1_TIE_REJECT=2218,2233,2234,2235,2239,2240,2245 */
/* Address: 0x801324AC, Size: 1420 bytes (355 instructions) */
#include "types.h"

typedef struct {
    u8 b0;
    u8 b1;
    u8 b2;
    u8 flags;
    u8 b4;
    u8 b5;
    u8 p6[6];
    u32 w12;
    u32 w16;
    u8 p20[4];
    s16 h24;
    u8 p26[12];
    s16 h38;
    u8 p40[2];
    s16 h42;
    u8 p44[8];
    u32 w52;
    u32 w56;
    u8 p60[7];
    u8 b67;
} Ent;
typedef struct { u8 pad[6460]; Ent ent[64]; } World;
extern World class_w;
extern u32 tpage_set_adr;
extern u32 img_set_adr;
extern u32 anim_set_adr;
extern void name_ent_res_adr_set(void);
extern void class_clear_n(s32);

void name_ent_layout_init(void)
{
    s32 i;

    name_ent_res_adr_set();

    for (i = 0; i < 32;) {
        class_clear_n(i + 96);
        class_w.ent[i + 1].b1 = 2;
        class_w.ent[i + 1].b2 = 64;
        class_w.ent[i + 1].w56 = 0;
        class_w.ent[i + 1].flags = 132;
        class_w.ent[i + 1].w12 = img_set_adr;
        class_w.ent[i + 1].w16 = anim_set_adr;
        i++;
        class_w.ent[i].w52 = tpage_set_adr;
        class_w.ent[i].b4 = 8;
        class_w.ent[i].b5 = 129;
        class_w.ent[i].h24 = 0;
        class_w.ent[i].b67 = 16;
        class_w.ent[i].h38 = 400;
        class_w.ent[i].h42 = 0;
    }
    class_w.ent[1].h24 = 3;
    class_w.ent[1].h38 = -112;
    class_w.ent[1].h42 = -60;
    class_w.ent[2].h24 = 0;
    class_w.ent[2].h38 = 0;
    class_w.ent[2].h42 = 0;
    class_w.ent[3].h24 = 13;
    class_w.ent[3].h38 = -96;
    class_w.ent[3].h42 = -74;
    class_w.ent[4].h24 = 13;
    class_w.ent[4].h38 = -80;
    class_w.ent[4].h42 = -74;
    class_w.ent[5].h24 = 13;
    class_w.ent[5].h38 = -64;
    class_w.ent[5].h42 = -74;
    class_w.ent[6].h24 = 13;
    class_w.ent[6].h38 = -40;
    class_w.ent[6].h42 = -74;
    class_w.ent[7].h24 = 13;
    class_w.ent[7].h38 = -24;
    class_w.ent[7].h42 = -74;
    class_w.ent[8].h24 = 13;
    class_w.ent[8].h38 = -8;
    class_w.ent[8].h42 = -74;
    class_w.ent[9].h24 = 13;
    class_w.ent[9].h38 = 80;
    class_w.ent[9].h42 = -74;
    class_w.ent[10].h24 = 13;
    class_w.ent[10].h38 = 96;
    class_w.ent[10].h42 = -74;
    class_w.ent[11].h24 = 13;
    class_w.ent[11].h38 = 112;
    class_w.ent[11].h42 = -74;
    class_w.ent[12].h24 = 13;
    class_w.ent[12].h38 = 128;
    class_w.ent[12].h42 = -74;
    class_w.ent[13].h24 = 13;
    class_w.ent[13].h38 = 144;
    class_w.ent[13].h42 = -74;
    class_w.ent[14].h24 = 15;
    class_w.ent[14].h38 = -143;
    class_w.ent[14].h42 = -40;
    class_w.ent[15].h24 = 17;
    class_w.ent[15].h38 = -143;
    class_w.ent[15].h42 = -8;
    class_w.ent[16].h24 = 19;
    class_w.ent[16].h38 = -143;
    class_w.ent[16].h42 = 24;
    class_w.ent[17].h24 = 21;
    class_w.ent[17].h38 = -143;
    class_w.ent[17].h42 = 56;
    class_w.ent[18].h24 = 23;
    class_w.ent[18].h38 = -143;
    class_w.ent[18].h42 = 88;
    class_w.ent[19].h24 = 7;
    class_w.ent[19].h38 = -143;
    class_w.ent[19].h42 = -40;
    class_w.ent[20].h24 = 7;
    class_w.ent[20].h38 = -143;
    class_w.ent[20].h42 = -8;
    class_w.ent[21].h24 = 7;
    class_w.ent[21].h38 = -143;
    class_w.ent[21].h42 = 24;
    class_w.ent[22].h24 = 7;
    class_w.ent[22].h38 = -143;
    class_w.ent[22].h42 = 56;
    class_w.ent[23].h24 = 7;
    class_w.ent[23].h38 = -143;
    class_w.ent[23].h42 = 88;

    for (i = 0; i < 5; i++) {
        class_w.ent[i + 19].flags &= 0x7F;
    }

    class_w.ent[24].h24 = 9;
    class_w.ent[24].h38 = -143;
    class_w.ent[24].h42 = -40;
    class_w.ent[24].b1 = 1;
    class_w.ent[25].h24 = 4;
    class_w.ent[25].h38 = 256;
    class_w.ent[25].h42 = 256;
    class_w.ent[25].b1 = 3;
    class_w.ent[26].h24 = 25;
    class_w.ent[26].h38 = 136;
    class_w.ent[26].h42 = -40;
    class_w.ent[26].flags = 0;
    class_w.ent[27].h24 = 27;
    class_w.ent[27].h38 = 136;
    class_w.ent[27].h42 = 24;
    class_w.ent[27].flags = 0;
    class_w.ent[28].h24 = 8;
    class_w.ent[28].h38 = 136;
    class_w.ent[28].h42 = -40;
    class_w.ent[28].flags = 0;
    class_w.ent[29].h24 = 8;
    class_w.ent[29].h38 = 136;
    class_w.ent[29].h42 = 24;
    class_w.ent[29].flags = 0;
}
