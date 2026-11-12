/* shugaku_girl2_serifu_face - SHUGAKU overlay, Address: 0x8013A184, Size: 160 bytes */
/* @CompileEnv: IDO52_PRECOLOR_WEB=a0:9 */
/* @SymbolSize: 160 */
/* @PostProc: patch_stack_complete --old 32 --new 48 */
/* @PostProc: remap_stack_slots --from 31 --to 43 */
/* @PostProc: patch_insn_reg_field 0x24:rt:v0:v1 0x2c:rs:v0:v1 0x38:rs:v0:v1 */
#include "types.h"

extern void func_80138664(void);
extern s32 get_g_zyotai_h(s32 a0);
extern void bustup_face_change(s32 a0);

extern u8  now_girl;
extern s16 now_serifu_shugaku_in;
extern s32 serifu_no;

void shugaku_girl2_serifu_face(void)
{
    u8 arg;
    u8 v1;

    func_80138664();
    v1 = now_girl;
    if (v1 != 2) {
        return;
    }
    arg = (u8)get_g_zyotai_h((s32)v1);
    if (now_serifu_shugaku_in != 7) goto lbl_8013A1E8;
    if (serifu_no == 0) goto lbl_8013A1E8;
    bustup_face_change(arg);

lbl_8013A1E8:
    if (now_serifu_shugaku_in != 6) {
        return;
    }
    if (serifu_no != 0) {
        return;
    }
    bustup_face_change(arg);
}
