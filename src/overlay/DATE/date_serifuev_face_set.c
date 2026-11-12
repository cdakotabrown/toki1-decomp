/* @SymbolStart: 0x8014FF44 */
/* @SymbolSize: 124 */
#include "types.h"

extern void bustup_face_change(s32 a);
extern s32 get_g_zyotai_h(s32 a);
extern void func_8014C530(void);

extern u8 now_girl;

extern u16 now_serifuev_in;
extern u8 now_girl;

void date_serifuev_face_set(void)
{
    s32 r;

    if (now_serifuev_in == 5) {
        bustup_face_change(1);
    } else if (now_serifuev_in == 9) {
        bustup_face_change(2);
    } else if (now_serifuev_in == 11) {
        r = get_g_zyotai_h(now_girl);
        bustup_face_change(r & 0xFF);
    }
    func_8014C530();
}
