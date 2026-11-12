/* @SymbolStart: 0x801369CC */
/* @SymbolSize: 440 */
/* @CompileEnv: IDO52_AS1_NO_LA_PULL=0 IDO52_AS1_XBB_VOLATILE_OK=0 */
/* Address: 0x801369D4, Size: 432 bytes (108 instructions) */
#include "types.h"

typedef struct {
    u8 pad0[446];
    s16 unk1BE;
    s16 unk1C0;
    s16 unk1C2;
    s16 unk1C4;
    s16 unk1C6;
} ShogEnt;

extern void sub_sub_step_up(void);
extern void bustup_face_change(u8);
extern void param_clamp(void);
extern u8 screen_w[];
extern volatile u8 now_girl;
extern s32 serifu_no;

void shougatu_girl_para_set3(void)
{
    ShogEnt *p;

    bustup_face_change((u8)(serifu_no + 1));
    if (serifu_no == 0) {
        p = (ShogEnt *)(screen_w + now_girl * 56);
        p->unk1BE = p->unk1BE + 3;
        p = (ShogEnt *)(screen_w + now_girl * 56);
        p->unk1C2 = p->unk1C2 + 2;
        p = (ShogEnt *)(screen_w + now_girl * 56);
        p->unk1C6 = p->unk1C6 - 15;
    } else if (serifu_no == 1) {
        p = (ShogEnt *)(screen_w + now_girl * 56);
        p->unk1BE = p->unk1BE + 2;
        p = (ShogEnt *)(screen_w + now_girl * 56);
        p->unk1C2 = p->unk1C2 + 1;
        p = (ShogEnt *)(screen_w + now_girl * 56);
        p->unk1C6 = p->unk1C6 - 10;
    } else {
        p = (ShogEnt *)(screen_w + now_girl * 56);
        p->unk1C2 = p->unk1C2 + 1;
        p = (ShogEnt *)(screen_w + now_girl * 56);
        p->unk1C6 = p->unk1C6 + 10;
    }
    param_clamp();
    sub_sub_step_up();
}
