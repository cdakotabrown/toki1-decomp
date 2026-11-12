/* @SymbolSize: 276 */
/* ending_line_scroll - ENDING overlay, Address: 0x80132F28, Size: 276 bytes (69 insns) */
#include "types.h"

typedef struct {
    u8 pad0[2];
    s16 y;
    u8 pad4[3];
    u8 flag;
} KWork;

extern void get_ksys();
extern s32 get_now_k();
extern KWork *get_k_work();
extern u8 scroll_exist;

void ending_line_scroll(void)
{
    s32 n;
    KWork *p;
    s32 i;
    s32 next;
    s32 v;
    KWork *q;

    get_ksys();
    v = get_now_k();
    n = v & 0xFF;
    p = get_k_work(v);
    next = n + 1;
    get_k_work(next);
    if (p->y >= 104) {
        if (scroll_exist != 0) {
            if ((u32)n >= 2) {
                i = 1;
                do {
                    q = get_k_work(i);
                    i++;
                    if (q->y == 72) {
                        q->y -= 16;
                        q->flag = 0;
                    } else {
                        q->y -= 16;
                    }
                    i = (s16)i;
                } while ((u32)i < (u32)n);
            }
            scroll_exist = 0;
            p->y = 88;
            p->flag = 1;
        }
    }
    q = get_k_work(next);
    if (q->y >= 104) {
        scroll_exist = 1;
        q->flag = 0;
    }
}
