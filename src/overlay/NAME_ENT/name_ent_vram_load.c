/* @CompileFlags: -Wc,-pic0 */
/* @SymbolStart: 0x8013BFA4 */
/* @PostProc: patch_stack_complete --old 56 --new 64 */
/* @PostProc: remap_stack_slots --from 40 --to 52 --from 42 --to 54 --from 44 --to 56 --from 46 --to 58 */
/* @SymbolSize: 348 */
/* @PostProc: scheduler_normalizer */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x1c:rs:v0:v1 0x1c:rt:v0:v1 0x20:rs:v0:v1 0x38:rt:v0:v1 0x40:rt:v0:v1 0x144:rt:v0:v1 0x150:rt:v0:v1 */
#include "types.h"

typedef struct {
    s16 field_00;
    s16 field_02;
    s16 field_04;
    s16 field_06;
} RectStruct;

extern void device_port_init();
extern void cd_read_start2();
extern s32 get_CD_State2();
extern void LoadImage();
extern void palette_upload_enqueue();
extern void sub_sub_step_up();

extern u8 screen_bri;
extern u8 step_mode;

void name_ent_vram_load(void)
{
    s32 state;
    s32 buttons;
    RectStruct rect;

    screen_bri += 4;
    if ((u32)screen_bri >= 128) {
        screen_bri = 128;
    }

    state = step_mode;
    switch (state) {
    case 0:
        device_port_init(0, 0);
        cd_read_start2(5, 0x801E0000, 0x9DBA);
        step_mode = step_mode + 1;
        break;
    case 1:
        buttons = get_CD_State2();
        if (buttons & 0x0001) {
            cd_read_start2(137, 0x80180000, 0x9CB5);
            step_mode = step_mode + 1;
        }
        break;
    case 2:
        buttons = get_CD_State2();
        if (buttons & 0x0001) {
            rect.field_00 = 992;
            rect.field_02 = 256;
            rect.field_06 = 128;
            rect.field_04 = 32;
            LoadImage(&rect, 0x801E0200);
            palette_upload_enqueue(0x801E0000, 24, 1, 2, 0);
            step_mode = 3;
            screen_bri = 128;
        }
        break;
    case 3:
        sub_sub_step_up();
        break;
    }
}
