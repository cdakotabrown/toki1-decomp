/* @CompileEnv: IDO52_FORCE_WEB_COLOR=8:a0,14:v1 */
/* @PostProc: patch_insn_reg_field 0x13c:rt:v0:v1 0x140:rs:v0:v1 0x140:rt:v0:v1 0x148:rs:v0:v1 0x160:rt:v0:v1 0x164:rs:v0:v1 0x164:rt:v0:v1 0x16c:rs:v0:v1 */
/* Address: 0x80150EC8 */
#include "types.h"

extern void date_light_effect(void);
extern void date_cmd1280_send(void);
extern void date_cmd1283_send(void);

typedef struct {
    void (*f[39])(s32);
} FnTable;

extern FnTable date_girl2_mode3_dispatch_tbl;
extern u8 now_year;
extern u8 now_month;
extern u8 now_day;
extern u8 now_sub_sub_step;
extern u32 frame_counter;
extern u8 fade_bri;
extern s32 date_sprite_on;

void date_girl2_mode3_dispatch(void) {
    u32 evt;
    FnTable local;
    u32 rem;

    evt = now_year;
    local = date_girl2_mode3_dispatch_tbl;

    if ((evt == 95 && now_month == 7U && now_day == 23) ||
        (evt == 96 && now_month == 7U && now_day == 28) ||
        (evt == 97 && now_month == 7U && now_day == 27)) {
        if (fade_bri == 128 && date_sprite_on == 0 &&
            (u32)now_sub_sub_step >= 26U) {
            date_light_effect();
        }
    }

    local.f[now_sub_sub_step](128);

    if ((u32)now_sub_sub_step >= 13U && (u32)now_sub_sub_step < 16U) {
        rem = frame_counter % 180U;
        if (rem == 0) {
            date_cmd1280_send();
            rem = frame_counter % 180U;
        }
        if (rem == 90) {
            date_cmd1283_send();
        }
    }
}
