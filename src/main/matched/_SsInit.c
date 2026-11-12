/* Address: 0x8008B388 */
/* Size: 424 bytes (106 instructions) */
/* Region: Library (IDO 5.2 codegen, ASPSX-assembled band) */
/* - `@Compiler: ido52-gas` (IDO -S assembled GNU-as style).  IDO's own as1 */
/* @Compiler: ido52-gas */
/* @CompileFlags: -Wc,-pic2 --ido-fill-load-delay --ido-fill-ptr-bump --ido-fill-branch-hop --fill-epilogue --ido-fill-at-next --ido-copy-dead-ft --ido-copy-and-bump --ido-jr-store-slot --ido-b-store-slot */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=4:v1,14:v0,34:a2 IDO52_NO_LOOP_TEST_REPLACEMENT=occ:6 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions --offset1 0x3c --offset2 0x44 */
/* @PostProc: swap_instructions --offset1 0x40 --offset2 0x44 */
/* @PostProc: swap_instructions --offset1 0x44 --offset2 0x48 */
/* @PostProc: swap_instructions --offset1 0x4c --offset2 0x50 */
/* @PostProc: swap_instructions --offset1 0x50 --offset2 0x54 */
/* @PostProc: swap_instructions --offset1 0xa0 --offset2 0xa8 */
/* @PostProc: swap_instructions --offset1 0xa4 --offset2 0xa8 */
/* @PostProc: swap_instructions --offset1 0xa8 --offset2 0xb4 */
/* @PostProc: swap_instructions --offset1 0xb0 --offset2 0xb4 */
/* @PostProc: swap_instructions --offset1 0x100 --offset2 0x108 */
/* @PostProc: swap_instructions --offset1 0x104 --offset2 0x108 */
/* @PostProc: swap_instructions --offset1 0x128 --offset2 0x12c */
/* @PostProc: swap_instructions --offset1 0x124 --offset2 0x128 */
/* @PostProc: swap_instructions --offset1 0x120 --offset2 0x124 */
/* @PostProc: swap_instructions --offset1 0x164 --offset2 0x168 */
/* @PostProc: swap_instructions --offset1 0x190 --offset2 0x19c */
/* @PostProc: swap_instructions --offset1 0x194 --offset2 0x19c */
/* @PostProc: swap_instructions --offset1 0x198 --offset2 0x19c */
#include "types.h"

#define SPU_VOICE_REGS ((s16 *)0x1F801C00)
#define SPU_CTRL_REGS  ((s16 *)0x1F801D80)

extern s16 libsnd_spu_voice_reset_tbl[8];
extern s16 libsnd_spu_ctrl_reset_tbl[16];
extern s32 _SsMarkCallback[32][16];
extern s32 VBLANK_MINUS;
extern s32 _snd_openflag;
extern s32 _snd_ev_flag;
extern s32 _snd_use_vsync_cb;
extern s32 _snd_use_event;
extern s32 _snd_use_interrupt_id;
extern s32 _snd_1per2;
extern s32 _snd_video_mode;
extern s32 _snd_vsync_cb;

extern void ResetCallback(void);
extern void SpuInit(void);
extern void SpuInitHot(void);
extern void SpuVmInit(s32);
extern s32 GetVideoMode(void);

void _SsInit(s32 arg0)
{
    s16 *dst;
    s32 i;
    s32 j;
    s32 n;

    ResetCallback();
    if (arg0 == 0) {
        SpuInit();
    } else {
        SpuInitHot();
    }
    dst = SPU_VOICE_REGS;
    for (j = 0; j < 24; j++) {
        for (i = 0; i < 8; i++) {
            *dst++ = libsnd_spu_voice_reset_tbl[i];
        }
    }
    dst = SPU_CTRL_REGS;
    for (j = 0; j < 16; j++) {
        *dst++ = libsnd_spu_ctrl_reset_tbl[j];
    }
    SpuVmInit(24);
    for (n = 0; n < 32; n++) {
        for (j = 0; j < 16; j++) {
            _SsMarkCallback[n][j] = 0;
        }
    }
    VBLANK_MINUS = 60;
    _snd_openflag = 0;
    _snd_use_vsync_cb = 0;
    _snd_use_interrupt_id = -1;
    _snd_use_event = 0;
    _snd_1per2 = 0;
    _snd_vsync_cb = 0;
    _snd_video_mode = GetVideoMode();
    _snd_ev_flag = 0;
}
