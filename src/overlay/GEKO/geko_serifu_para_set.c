/* @CompileEnv: IDO52_AS1_XBB_SKIP_CAND=0 IDO52_AS1_XBB_VOLATILE_OK=0 */
/* @SymbolStart: 0x80137640 */
/* @SymbolSize: 440 */
/* @CompileFlags: -cckr */
/* geko_serifu_para_set -- GEKO overlay, Address: 0x80137648 (true start 0x80137640, */
#include "types.h"

extern int serifu_no;
extern volatile unsigned char now_girl;
extern short screen_w[][28];

void geko_serifu_para_set()
{
    short *s;

    bustup_face_change((unsigned char)(serifu_no + 1));
    if (serifu_no == 0) {
        s = screen_w[now_girl]; s[223] += 3;
        s = screen_w[now_girl]; s[225] += 2;
        s = screen_w[now_girl]; s[227] -= 15;
    } else if (serifu_no == 1) {
        s = screen_w[now_girl]; s[223] += 2;
        s = screen_w[now_girl]; s[225] += 1;
        s = screen_w[now_girl]; s[227] -= 10;
    } else {
        s = screen_w[now_girl]; s[225] += 1;
        s = screen_w[now_girl]; s[227] += 10;
    }
    param_clamp();
    sub_sub_step_up();
}
