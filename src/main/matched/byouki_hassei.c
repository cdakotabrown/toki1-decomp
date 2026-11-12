/* Address: 0x80072E30 */
/* Size: 1044 bytes (261 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @CompileFlags: -cckr -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    u32 b0 : 1;
    u32 met : 1;
    u32 rest : 30;
    u8 pad[52];
} Rec;

extern volatile s16 para_kon;
extern u8 byouki_kind;
extern u8 byouki_state;
extern s16 para_tai;
extern s16 para_str;
extern u32 album_work;
extern u8 now_day;
extern u8 cal_day_kind[];
extern u8 girl_no;
extern Rec girl0_flag[];

void byouki_hassei()
{
    s32 pa, pb, pc, pd;
    s32 flag;
    s32 n;
    s32 lim;
    s32 i;
    s32 j;
    s32 k;

    flag = 0;
    lim = para_kon / 8 + 204;
    if (lim > 254) {
        lim = 254;
    }
    switch (byouki_kind & 0xf) {
    case 0:
        n = 0;
        if ((byouki_state >> 5) == 0) {
            if (para_tai < 20 && lim < (rand() & 0xff)) {
                n = 1;
                flag = 1;
            } else if (para_tai == 0) {
                n = 1;
                flag = 1;
            }
            if (para_str > 80 && lim < (album_work & 0xff)) {
                n += 2;
                flag = 1;
            }
        }
        if (n == 1) {
            goto_sub_sub_step(1);
        }
        if (n == 2) {
            goto_sub_sub_step(2);
        }
        if (n == 3) {
            goto_sub_sub_step(3);
        }
        break;
    case 1:
        n = 1;
        if (para_str > 80 && lim < (album_work & 0xff)) {
            n = 3;
            flag = 1;
        }
        if (para_tai > 30) {
            n = 0;
            flag = 1;
        }
        if (n == 3) {
            goto_sub_sub_step(3);
        }
        if (n == 0) {
            goto_sub_sub_step(5);
        }
        break;
    case 2:
        n = 2;
        if (para_tai < 20 && lim < (rand() & 0xff)) {
            n = 3;
            flag = 1;
        } else if (para_tai == 0) {
            n = 3;
            flag = 1;
        }
        if (para_str < 70) {
            n = 0;
            flag = 1;
        }
        if (n == 3) {
            goto_sub_sub_step(3);
        }
        if (n == 0) {
            goto_sub_sub_step(6);
        }
        break;
    case 3:
        n = 3;
        if (para_str < 70 && para_tai > 30) {
            n = 0;
            flag = 1;
        }
        if (n == 0) {
            goto_sub_sub_step(7);
        } else if (cal_day_kind[now_day] == 0) {
            k = rand() % 11;
            for (j = 0; j < 11; j++) {
                i = (k + j) % 11;
                if (girl0_flag[i].met && (u32)get_h_tokimeki(i) >= 51 && (rand() & 0xff) < 63) {
                    girl_no = i;
                    goto_step(98);
                    return;
                }
            }
        }
        break;
    default:
        byouki_kind &= ~0xf;
        break;
    }
    if (flag == 0) {
        goto_sub_step(255);
    }
}
