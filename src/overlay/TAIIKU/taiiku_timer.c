/* @SymbolSize: 192 */
#include "types.h"

extern s32 Vblnk_Timer_Init(void);
extern s32 Vblnk_Timer(void);

u16 taiiku_tama_push_tick[2] = {0, 0};
u16 taiiku_tama_pfrm_tick[2] = {0, 0};
extern u16 taiiku_tama_gfrm_tick[];
extern u8  taiiku_tama_ctick;
extern s16 taiiku_tama_spd_tick;

void taiiku_timer(void) {
    u32 v0;

    v0 = Vblnk_Timer();
    if (v0 >= 2u) {
        taiiku_tama_gfrm_tick[1]++;
        Vblnk_Timer_Init();
    }
    taiiku_tama_ctick++;
    taiiku_tama_push_tick[0]++;
    taiiku_tama_push_tick[1]++;
    taiiku_tama_spd_tick++;
    taiiku_tama_pfrm_tick[0]++;
    taiiku_tama_pfrm_tick[1]++;
    taiiku_tama_gfrm_tick[0]++;
}
