/* Address: 0x80133D98 */
/* Size: 152 bytes (38 instructions) */
/* @CompileEnv: IDO52_UGEN_TEMP_REG=5:t1 */
/* @SymbolSize: 152 */
#include "types.h"

extern void strcpy(void *, void *);
extern void sub_sub_step_up(void);

extern u8 ba_string;
extern u8 shougatu_serifu_base_set_str0;
extern u8 shougatu_alone_flag;
extern u8 shougatu_serifu6_flag;
extern s16 shougatu_meet_now_serifu;
extern volatile s16 shougatu_meet_now_serifu_v;
extern u8 now_girl;

void shougatu_serifu_base_set(void)
{
    s16 v0;
    s32 v1;

    strcpy(&ba_string, &shougatu_serifu_base_set_str0);
    v0 = (shougatu_alone_flag != 0) ? 0 : 15;
    v1 = shougatu_serifu6_flag;
    if (v1 == 0) {
        v0 += 2;
    }
    shougatu_meet_now_serifu = v0;
    if (v1 == 0) {
        shougatu_meet_now_serifu_v = v0;
        if (now_girl == 5) {
            shougatu_meet_now_serifu_v = v0 + 2;
        }
    }
    sub_sub_step_up();
}
