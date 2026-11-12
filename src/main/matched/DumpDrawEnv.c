/* Address: 0x800A3104, Size: 368 bytes (92 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 */
/* @CompileEnv: IDO52_AS1_XBB_SKIP_CAND=1 IDO52_UGEN_TEMP_REG=17:t6,18:t5,19:t8,20:t3,21:t4 */
/* @CompileFlags: -Wc,-pic0 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
/* @PostProc: patch_insn_reg_field 0x10:rt:t9:t7 0x14:rs:t9:t7 0x14:rt:t9:t7 0x30:rs:t9:t7 0x38:rt:t9:t8 0x3c:rs:t9:t8 0x3c:rt:t9:t8 0x4c:rs:t9:t8 0x54:rt:t9:t0 0x58:rs:t9:t0 0x58:rt:t9:t0 0x5c:rt:t7:t9 0x74:rs:t9:t0 0x78:rt:t7:t9 0x7c:rt:t9:t1 0x80:rs:t9:t1 0x80:rt:t9:t1 0x8c:rs:t9:t1 0x94:rt:t9:t2 0x98:rs:t9:t2 0x98:rt:t9:t2 0xa4:rs:t9:t2 0xd8:rt:t9:t5 0xdc:rs:t9:t5 0xdc:rt:t9:t5 0xe0:rd:t5:t3 0xe4:rs:t5:t3 0xe4:rt:t6:t4 0xec:rd:t8:a1 0xf0:rd:t1:a2 0xf4:rd:t3:a3 0xf8:rs:t3:a3 0xfc:rs:t1:a2 0x100:rs:t8:a1 0x108:rs:t9:t5 0x10c:rt:t6:t4 0x11c:rt:t9:t1 0x128:rs:t9:t1 0x128:rt:t9:t1 0x12c:rt:t3:t9 0x130:rt:t5:t7 0x134:rs:t5:t7 0x134:rt:t3:t9 0x134:rd:t4:t0 0x13c:rd:t7:a1 0x140:rd:t0:a2 0x144:rd:t2:a3 0x148:rs:t2:a3 0x14c:rs:t0:a2 0x150:rs:t7:a1 0x158:rs:t9:t1 0x15c:rt:t4:t0 */
/* 1. @CompileFlags: -Wc,-pic0 -- library region stays pic0 (the target frame */
#include "types.h"

typedef struct {
    s16 f0;
    s16 f2;
    s16 f4;
    s16 f6;
    s16 f8;
    s16 f10;
    s16 f12;
    s16 f14;
    s16 f16;
    s16 f18;
    u16 f20;
    u8 b22;
    u8 b23;
} Rec_800A3104;

extern void (*GPU_printf)(const char *, ...);
extern char libgpu_dump_clip_fmt[];
extern char libgpu_dump_ofs_fmt[];
extern char libgpu_dump_tw_fmt[];
extern char libgpu_dump_dtd_fmt[];
extern char libgpu_dump_dfe_fmt[];
extern char libgpu_dumpenv_tpage_type12_fmt[];
extern char libgpu_dumpenv_tpage_other_fmt[];
extern s32 GetGraphType(void);

void DumpDrawEnv(Rec_800A3104 *p)
{
    u32 v;

    GPU_printf(libgpu_dump_clip_fmt, p->f0, p->f2, p->f4, p->f6);
    GPU_printf(libgpu_dump_ofs_fmt, p->f8, p->f10);
    GPU_printf(libgpu_dump_tw_fmt, p->f12, p->f14, p->f16, p->f18);
    GPU_printf(libgpu_dump_dtd_fmt, p->b22);
    GPU_printf(libgpu_dump_dfe_fmt, p->b23);

    if (GetGraphType() == 1 || GetGraphType() == 2) {
        v = p->f20; GPU_printf(libgpu_dumpenv_tpage_type12_fmt, (u8)((v >> 9) & 3), (u8)((v >> 7) & 3), (u16)((v << 6) & 0x7C0), (v << 3) & 0x300);
    } else {
        v = p->f20; GPU_printf(libgpu_dumpenv_tpage_other_fmt, (u8)((v >> 7) & 3), (u8)((v >> 5) & 3), (u16)((v << 6) & 0x7C0), ((v << 4) & 0x100) + ((v >> 2) & 0x200));
    }
}
