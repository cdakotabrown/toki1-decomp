/* @PostProc: patch_stack_complete --old 40 --new 56 */
/* @PostProc: remap_stack_slots --from 36 --to 48 */
/* @PostProc: remap_stack_slots --from 37 --to 49 */
/* @PostProc: remap_stack_slots --from 38 --to 50 */
/* shugaku_byouki_serifu - SHUGAKU overlay, Address: 0x8013793C, Size: 348 bytes */
#include "types.h"

typedef struct {
    u8 b[3];
} C3;

extern C3   shugaku_byouki_serifu_tbl;
extern s32  shugaku_byouki_serifu_str0[];
extern s32  shugaku_byouki_serifu_str1[];
extern s32  shugaku_byouki_serifu_str2[];
extern s32  shugaku_byouki_serifu_str3[];

extern u8   byouki_kind;
extern u8   step_mode;
extern u8 ***ss_serifu_shugaku;
extern s16  now_serifu_shugaku;

extern void func_80138664(void);
extern void set_kanji_string(s32 a0, s32 a1, s32 a2, void *a3, s32 a4);
extern void sub_sub_step_up(void);

void shugaku_byouki_serifu(void)
{
    C3 tmp;

    tmp = shugaku_byouki_serifu_tbl;
    if ((byouki_kind & 0xF) != 0) {
        if (step_mode == 0) {
            now_serifu_shugaku = 2;
            ss_serifu_shugaku[2][0][0] = tmp.b[0];
            ss_serifu_shugaku[2][0][1] = tmp.b[1];
            ss_serifu_shugaku[2][0][2] = tmp.b[2];
        }
        func_80138664();
        if (step_mode != 0) {
            return;
        }
        if (now_serifu_shugaku != 2) {
            return;
        }
        set_kanji_string(-72, 48, 0, shugaku_byouki_serifu_str0, 0);
        set_kanji_string(-72, 64, 0, shugaku_byouki_serifu_str1, 0);
        set_kanji_string(-72, 80, 0, shugaku_byouki_serifu_str2, 0);
        set_kanji_string(-72, 96, 0, shugaku_byouki_serifu_str3, 0);
        step_mode = 1;
        return;
    }
    sub_sub_step_up();
}
