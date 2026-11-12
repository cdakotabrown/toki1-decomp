#include "types.h"

extern void bustup_modes20_step(void);
extern void func_8014C530(void);
extern u16 now_serifuev_in;
extern u8 bustup_cd_read_tbl0;
extern u8 now_sub_sub_step;

void date_serifuev_modes20(void) {
    func_8014C530();
    if (now_serifuev_in == 4) {
        bustup_cd_read_tbl0 = 1;
        bustup_modes20_step();
        now_sub_sub_step -= 1;
    }
}
