/* Address: 0x80055DD0 */
/* Size: 196 bytes (49 instructions) */
/* Region: Game */
/* @CompileEnv: IDO52_AS1_TIE_REJECT=16,19 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    u32 chk;
    u8 data[3836];
} SaveRec;

typedef struct {
    u8 hdr[512];
    SaveRec rec[2];
} SaveArea;

extern SaveArea album_chr;
extern u8 album_slot;

s32 save_check(s32 arg) {
    s32 j;
    s32 i;
    s32 count;
    u32 sum;
    u32 b;
    u8 *p;

    j = 0;
    do {
        count = 0;
        p = album_chr.rec[j].data;
        sum = 0;
        i = 4;
        do {
            b = *p;
            i++;
            sum += b;
            if (b != 0) {
                count++;
            }
            p++;
        } while (i < 3840);
        if (sum == album_chr.rec[j].chk && count != 0) {
            album_slot = j;
            return 1;
        }
        j++;
    } while (j != 2);
    return -1;
}
