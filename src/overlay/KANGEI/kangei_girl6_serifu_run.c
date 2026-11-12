/* @SymbolStart: 0x801353A0 */
/* @SymbolSize: 168 */
/* @PostProc: scheduler_normalizer */
/* @PostProc: patch_insn_reg_field 0x68:rt:v0:v1 0x6c:rs:v0:v1 0x6c:rt:v0:v1 0x74:rs:v0:v1 0x74:rt:t1:v0 0x78:rs:v0:v1 0x78:rt:t0:v1 0x7c:rs:t1:v0 0x80:rt:t0:v1 0x84:rt:v0:t0 0x88:rs:v0:t0 0x88:rt:v0:t0 0x90:rs:v0:t0 */
#include "types.h"

extern void func_80133F08(void);
extern void device_cmd_send(s32);
extern void don_init(void);

extern volatile u8 now_girl;
extern volatile u8 kangei_serifu_mode;
extern volatile s16 kangei_now_s_in;
extern volatile s32 step_counter;

void kangei_girl6_serifu_run(void)
{
    u8 cur_char;
    s32 counter;
    s32 lt;

    if (kangei_now_s_in == 2 && now_girl == 6) {
        kangei_serifu_mode = 2;
    } else {
        kangei_serifu_mode = 0;
    }

    func_80133F08();

    if (kangei_now_s_in != 3) {
        return;
    }

    counter = step_counter;
    lt = (s32)((u32)counter < 1);
    step_counter = counter + 1;
    if (lt != 0) {
        cur_char = now_girl;
        if (cur_char == 6) {
            device_cmd_send(1283);
            don_init();
        }
    }
}
