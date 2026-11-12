/* Address: 0x80041BBC */
/* Size: 560 bytes (140 instructions) */
/* Region: Game (IDO 5.2) */
/* @CompileFlags: -Wc,-pic1 -cckr */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    u8 pad0[48];
    u16 b30 : 8;
    u16 b31a : 4;
    u16 b31b : 4;
    u16 b32a : 1;
    u16 b32b : 1;
    u16 b32c : 1;
    u16 b32r : 13;
    u16 b34a : 4;
    u16 b34b : 5;
    u16 b34c : 2;
    u16 b34r : 5;
    u8 pad36[4];
    s16 w3a;
    s16 pad3c;
    s16 w3e;
    s16 pad40;
    s16 w42;
    s16 pad44;
    s16 w46;
    s16 pad48;
    s16 w4a;
    s16 pad4c;
    s16 w4e;
    s16 pad50;
    s16 w52;
    s16 pad54;
    s16 w56;
    s16 pad58;
    s16 w5a;
    s8 c[21];
    u8 z[9];
    u8 pad7a[2];
} Stat;

typedef struct {
    u8 pad0[196];
    Stat st;
    Stat save;
} Save;

extern Save screen_w;

void parameter_init()
{
    s32 i;
    Save *sv;

    sv = &screen_w;
    bzero(&sv->st, 124);
    sv->st.b30 = 0;
    sv->st.b31a = 0;
    sv->st.b31b = 11;
    sv->st.b32a = 0;
    sv->st.b32b = 0;
    sv->st.b32c = 0;
    sv->st.b34c = 1;
    sv->st.b34a = 1;
    sv->st.b34b = 1;
    i = 0;
    do {
        sv->st.z[i] = 0;
        i++;
    } while (i < 9);
    sv->st.w3a = 100;
    sv->st.w3e = 40;
    sv->st.w42 = 40;
    sv->st.w46 = 40;
    sv->st.w4a = 40;
    sv->st.w4e = 32;
    sv->st.w52 = 60;
    sv->st.w56 = 5;
    sv->st.w5a = 0;
    sv->st.c[0] = 1;
    sv->st.c[1] = 0;
    sv->st.c[2] = 1;
    sv->st.c[3] = 0;
    sv->st.c[4] = 1;
    sv->st.c[5] = 0;
    sv->st.c[6] = 0;
    sv->st.c[7] = 0;
    sv->st.c[8] = 1;
    sv->st.c[9] = 1;
    sv->st.c[10] = 0;
    sv->st.c[11] = 0;
    sv->st.c[12] = 1;
    sv->st.c[13] = 0;
    sv->st.c[14] = 0;
    sv->st.c[15] = 1;
    sv->st.c[16] = 0;
    sv->st.c[17] = 0;
    sv->st.c[18] = 0;
    sv->st.c[19] = 0;
    sv->st.c[20] = 0;
    memcpy(&sv->save, &sv->st, 124);
}
