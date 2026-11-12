/* Address: 0x800A0180 */
/* Size: 144 bytes (36 instructions) */
/* Region: Library (0x80086000-0x800BFFFF) */
/* Compiler: IDO 5.2 */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=0:v1 */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR_NOSPLIT=0 */
/* @CompileEnv: IDO52_UGEN_TEMP_REG_INPLACE=8 */
/* @CompileEnv: IDO52_UGEN_TEMP_REG=8:v0 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: patch_stack_complete --old 24 --new 32 --shift-ra */
/* @PostProc: convert_or_to_addu */
/* @PostProc: patch_insn_reg_field 0x64:rt:t9:t3 0x78:rs:t9:t3 */
#include "types.h"

typedef struct {
    u8 pad0[8];
    void (*send)(s32, void *, s32, s32);
    u8 pad1[12];
    s32 handle;
} Dev;

extern Dev *graph_jtb;

s32 libgpu_moveimage_pkt_head[2] = {0, 0};
s32 libgpu_moveimage_pkt_src_xy = 0;
u8  libgpu_moveimage_name_str[8] = {0, 0, 0, 0, 0, 0, 0, 0};
s32 libgpu_moveimage_pkt_dst[2] = {0, 0};

extern void checkRECT(void *dst, s32 *src);

void MoveImage(s32 *src, s32 arg1, s32 arg2)
{
    checkRECT(libgpu_moveimage_name_str, src);
    libgpu_moveimage_pkt_src_xy = src[0];
    libgpu_moveimage_pkt_dst[0] = (arg2 << 16) | (arg1 & 0xFFFF);
    libgpu_moveimage_pkt_dst[1] = src[1];
    graph_jtb->send(graph_jtb->handle, libgpu_moveimage_pkt_head, 20, 0);
}
