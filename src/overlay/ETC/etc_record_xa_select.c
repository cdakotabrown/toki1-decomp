/* @SymbolStart: 0x8013F044 */
/* @SymbolSize: 684 */
/* @CompileEnv: IDO52_AS1_XBB_VOLATILE_OK=4 IDO52_AS1_NO_LA_PULL=0,4 IDO52_AS1_NO_FALLTHRU_FILL=occ:6 */
/* @PostProc: swap_branch_operands --offsets 0xC0,0xD8 */
/* Address: 0x8013F054 (window starts 0x8013F044 - absorbed cross-prologue head) */
/* Size: 684 bytes (171 instructions, head included) */
#include "types.h"

extern void etc_xa_step(s32);

extern u8  screen_w[];
extern volatile u8 now_girl;
extern u8  option_on;
extern u8  album_work[];
extern volatile s32 etc_record_index;
extern s32 etc_call_choice;

void etc_record_xa_select(void)
{
    s32 idx;
    s32 kind;
    s32 sub;
    s32 code;
    s32 st;
    s32 v;
    s32 j;

    idx = etc_record_index;
    j = idx;
    code = 0;
    sub = 0;
    kind = 0;
    if (option_on == 0) {
        st = etc_call_choice;
        if (st == 1 || st == 3) {
            kind = screen_w[idx * 8 + 0x756];
            sub = screen_w[idx * 8 + 0x758];
            code = screen_w[idx * 8 + 0x757];
        } else if (st == 0 || st == 2) {
            for (; j >= 0; ) {
                v = screen_w[j * 8 + 0x756];
                j--;
                if (v == 12 && now_girl == 12) {
                    goto hit;
                }
                if (v != 11) {
                    continue;
                }
                if (now_girl != 11) {
                    continue;
                }
            hit:
                sub = screen_w[j * 8 + 0x760];
                code = screen_w[j * 8 + 0x75F];
                kind = v;
                break;
            }
        }
    } else {
        kind = album_work[idx * 8 + 0x50];
        sub = album_work[idx * 8 + 0x52];
        code = album_work[idx * 8 + 0x51];
    }
    if (now_girl == 12 && kind == 12U) {
        if (sub == 4) {
            etc_xa_step(1);
        } else if (sub == 6) {
            etc_xa_step(2);
        } else if (sub == 7) {
            etc_xa_step(3);
        } else if (sub == 2 && code == 79) {
            etc_xa_step(5);
        } else if (sub == 2 && code == 80) {
            etc_xa_step(4);
        } else if (sub == 2 && code == 81) {
            etc_xa_step(6);
        }
    } else if (now_girl == 11 && kind == 11U) {
        if (sub == 2 && code == 74) {
            etc_xa_step(0);
        } else if ((sub == 2 && code == 76) || sub == 11U) {
            etc_xa_step(1);
        } else if (sub == 2 && code == 78) {
            etc_xa_step(2);
        } else if (sub == 18) {
            etc_xa_step(3);
        }
    }
}
