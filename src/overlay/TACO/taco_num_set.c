/* @CompileEnv: IDO52_COPYPROP_VAR=-24 IDO52_COPYPROP_SITE=lod:1:deny */
/* @RodataBase: 0x8015D848 */
/* Address: 0x80137274 */
#include "types.h"

typedef struct {
    u8 b[11];
} TacoStr11;

extern TacoStr11 taco_num_set_tbl;

extern u8 taco_num_pad_str[];
extern u8 taco_num_set_str0[];
extern u8 taco_num_set_str1[];
extern u8 taco_num_set_str2[];
extern u8 taco_num_set_str3[];
extern u8 taco_num_set_str4[];
extern u8 taco_num_set_str5[];
extern u8 taco_num_set_str6[];
extern u8 taco_num_set_str7[];
extern u8 taco_num_set_str8[];
extern u8 taco_num_set_str9[];
extern u8 taco_num_set_str10[];
extern u8 taco_num_set_str11[];
extern u8 taco_num_set_str12[];
extern u8 taco_num_set_str13[];
extern u8 taco_num_set_str14[];
extern u8 taco_num_set_str15[];
extern u8 taco_num_set_str16[];
extern u8 taco_num_set_str17[];

extern void strcpy();
extern void strcat();

void
taco_num_set(s32 ctx, s32 val)
{
    s32 place;
    TacoStr11 tmp;
    s32 ten;
    s32 d;

    tmp = taco_num_set_tbl;

    if (val >= 100000000) {
        val = 99999999;
    }

    if (val >= 10000000) {
        strcpy(ctx, taco_num_pad_str);
    } else if (val >= 1000000) {
        strcpy(ctx, taco_num_set_str0);
    } else if (val >= 100000) {
        strcpy(ctx, taco_num_set_str1);
    } else if (val >= 10000) {
        strcpy(ctx, taco_num_set_str2);
    } else if (val >= 1000) {
        strcpy(ctx, taco_num_set_str3);
    } else if (val >= 100) {
        strcpy(ctx, taco_num_set_str4);
    } else if (val >= 10) {
        strcpy(ctx, taco_num_set_str5);
    } else {
        strcpy(ctx, taco_num_set_str6);
    }

    if (val == 0) {
        strcat(ctx, taco_num_set_str7);
    }

    place = 10000000;
    ten = 10;
    do {
        d = val / place;
        if (d != 0) {
            switch ((u32)d % (u32)ten) {
            case 0:
                strcat(ctx, taco_num_set_str8);
                break;
            case 1:
                strcat(ctx, taco_num_set_str9);
                break;
            case 2:
                strcat(ctx, taco_num_set_str10);
                break;
            case 3:
                strcat(ctx, taco_num_set_str11);
                break;
            case 4:
                strcat(ctx, taco_num_set_str12);
                break;
            case 5:
                strcat(ctx, taco_num_set_str13);
                break;
            case 6:
                strcat(ctx, taco_num_set_str14);
                break;
            case 7:
                strcat(ctx, taco_num_set_str15);
                break;
            case 8:
                strcat(ctx, taco_num_set_str16);
                break;
            case 9:
                strcat(ctx, taco_num_set_str17);
                break;
            }
        }
        place = place / ten;
    } while (place > 0);
}
