/* @CompileEnv: IDO52_FORCE_WEB_COLOR_NOSPLIT=118 IDO52_UGEN_DEST_TEMP=9,32 IDO52_UGEN_TEMP_REG=6:v1,20:v1 IDO52_UGEN_TEMP_REG_INPLACE=6,20 IDO52_FORCE_WEB_COLOR=32:v1,54:v0 IDO52_COPYPROP_VAR=-24:2 */
/* @CompileFlags: -cckr */
/* @SymbolSize: 540 */
/* Address: 0x8013BD84 */
/* Size: 540 bytes (135 insns) */
#include "types.h"

extern unsigned int get_h_tokimeki();
extern unsigned int get_h_yuukou();

extern unsigned char now_year;
extern unsigned char girl0_basyo;
extern short girl0_tokimeki;
extern short girl0_yuukou;
extern unsigned char screen_w[];
extern int bunka_girl;

int bunkaken_pick_judge()
{
    int best;
    short bestVal;
    int i;
    short sum;
    unsigned int n;
    unsigned int x;
    unsigned char *ent;
    int one;
    int loc96;

    best = 0;
    if (get_h_yuukou(0) >= 50 && get_h_yuukou(0) >= 80) {
        bestVal = girl0_tokimeki + girl0_yuukou;
    } else {
        bestVal = 0;
    }
    i = 1;
    one = 1;
    loc96 = 96;
    if (loc96 == (int)now_year) {
        x = girl0_basyo;
        n = x >> 4;
        if (n == 4 || one == n) {
            bestVal = 0;
        }
    }
    for (; i != 11; i++) {
        if (get_h_tokimeki(i) < 50) {
            continue;
        }
        if (get_h_yuukou(i) < 80) {
            continue;
        }
        ent = screen_w + i * 56;
        if (((unsigned int)(*(int *)(ent + 456) << 30) >> 31) != one) {
            continue;
        }
        sum = *(short *)(ent + 450) + *(short *)(ent + 446);
        if (loc96 == (int)now_year) {
            x = ent[458];
            x = x >> 4;
            if (x == 4 || one == x) {
                sum = 0;
            }
        }
        if (bestVal < sum) {
            best = i;
            bestVal = sum;
        }
    }
    if (get_h_tokimeki(best) < 50) {
        goto Lfail;
    }
    if (get_h_yuukou(best) < 80) {
        goto Lfail;
    }
    if (best == 0 && loc96 == (int)now_year) {
        x = girl0_basyo;
        n = x >> 4;
        if (n == 4 || one == n) {
            return 0;
        }
    }
    x = girl0_basyo;
    if (best == 0 && (x >> 4) >= 6) {
        best = 11;
    }
    bunka_girl = best;
    return 1;
Lfail:
    return 0;
}
