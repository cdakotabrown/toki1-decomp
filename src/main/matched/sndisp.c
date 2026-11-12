/* Address: 0x800512AC */
/* Size: 388 bytes (97 instructions) */
/* Region: Game (IDO 5.2) */
/* @CompileFlags: -Wc,-pic1 -cckr */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    u8 b[5];
} Pos5;

extern Pos5 sndisp_tbl0;
extern u8 k_cnt;
extern u8 k_no;
extern u8 k_mode;

void sndisp(str, y, attr)
    u8 *str;
    s32 y;
    void *attr;
{
    s32 flag;
    s32 n;
    s32 col;
    s32 i;
    s32 unused;
    Pos5 pos;
    u8 buf[256];

    pos = sndisp_tbl0;
    strncpy(buf, str, 256);
    flag = 0;
    n = 0;
    col = 0;
    i = 0;
    while (buf[i] != 0) {
        if (buf[i] == ' ' && !(flag & 1)) {
            pos.b[n] = col;
            buf[i] = 0;
            flag = -1;
            n++;
        }
        col++;
        if (buf[i] == 'P' && !(flag & 1)) {
            flag = -1;
        }
        if (buf[i] == 'G' && !(flag & 1)) {
            flag = -1;
        }
        i++;
        flag++;
    }
    k_no = check_set_k();
    sndi(buf, y, attr);
    for (col = 0; col < n; col++) {
        sndi(&buf[pos.b[col]] + 1, y + col + 1, attr);
    }
    k_cnt = check_set_k();
    k_mode = 128;
}
