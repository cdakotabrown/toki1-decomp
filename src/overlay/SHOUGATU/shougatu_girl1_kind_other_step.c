/* @CompileEnv: IDO52_AS1_NO_FALLTHRU_FILL=1 */
/* @PostProc: patch_stack_complete --old 184 --new 200 */
/* @PostProc: remap_stack_slots --from 36 --to 48 */
/* Address: 0x80139BA4 */
/* Size: 372 bytes (93 instructions) */
#include "types.h"
#include "tokimeki/etc_cg_res.h"

typedef void (*FnPtr)(void);

typedef struct {
    FnPtr f[37];
} FnTable;

extern FnTable shougatu_subsub_jump8_tbl;
extern u8 now_sub_sub_step;
extern volatile s32 pad_data;
extern volatile u8 now_month;
extern u8 shougatu_subsub_jump8_str0[];
extern u8 shougatu_subsub_jump8_str1[];
extern ETC_CG_RES shougatu_girl1_res;

extern void palette_upload_enqueue(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
extern void printf(u8 *arg0);

void shougatu_girl1_kind_other_step(void)
{
    FnTable fns;
    u32 sceneId;

    fns = shougatu_subsub_jump8_tbl;
    fns.f[now_sub_sub_step]();

    if (pad_data & 0x100) {
        sceneId = (u32)now_month;
        if (sceneId >= 6U && sceneId < 9U) {
            palette_upload_enqueue(shougatu_girl1_res.pal[0], 17, 1, 0, 0);
        } else {
            palette_upload_enqueue(shougatu_girl1_res.pal[1], 17, 1, 0, 0);
        }
        printf(shougatu_subsub_jump8_str0);
    }

    if (pad_data & 0x800) {
        sceneId = (u32)now_month;
        if (sceneId >= 6U && sceneId < 9U) {
            palette_upload_enqueue(shougatu_girl1_res.pal[0], 17, 1, 2, 0);
        } else {
            palette_upload_enqueue(shougatu_girl1_res.pal[1], 17, 1, 2, 0);
        }
        printf(shougatu_subsub_jump8_str1);
    }
}
