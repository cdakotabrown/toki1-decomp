/* @SymbolStart: 0x80148950 */
/* @SymbolSize: 108 */
/* @PostProc: swap_instructions --offset1 0x20 --offset2 0x24 */
#include "types.h"
#include "tokimeki/class_w.h"
extern CLASS_W class_w[];

typedef struct {
    s16 x;
    u8 pad[66];
} Rec68;

extern s32 Vblnk_Timer_Init();

void taiiku_result_frame_sub(void)
{
    ((Rec68 *)&class_w[100].frame)[0].x -= 4;
    ((Rec68 *)&class_w[100].frame)[1].x -= 3;
    ((Rec68 *)&class_w[100].frame)[2].x -= 2;
    ((Rec68 *)&class_w[100].frame)[3].x -= 1;
    Vblnk_Timer_Init();
}
