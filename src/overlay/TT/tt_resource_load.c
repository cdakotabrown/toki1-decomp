/* @SymbolSize: 1180 */
/* @PostProc: patch_stack_complete --old 32 --new 56 */
/* @PostProc: remap_stack_slots --from 24 --to 48 --from 26 --to 50 --from 28 --to 52 --from 30 --to 54 */
#include "types.h"

extern void tt_cd_read_sync(s32 a0, s32 a1, s32 a2);
extern void tt_buf_clear_load(s32 a0, s32 a1, s32 a2);
extern void tt_tex_load(s32 a0, s32 a1, s32 a2);
extern void LoadImage(void *a0, s32 a1);
extern void DrawSync(s32 a0);

extern u8 tt_clut_dat[];

typedef struct { s16 a, b, c, d; } Rect;

void tt_resource_load(void)
{
    Rect r;

    tt_cd_read_sync(0x80162000, 0xBED8, 0x00054900);
    tt_buf_clear_load(0x80162000, 0, 240);
    tt_buf_clear_load(0x80164000, 256, 240);
    tt_tex_load(0x80166000, 640, 0);
    tt_tex_load(0x80168000, 640, 128);
    tt_tex_load(0x8016A000, 672, 0);
    tt_tex_load(0x8016C000, 672, 128);
    tt_tex_load(0x8016E000, 704, 0);
    tt_tex_load(0x80170000, 704, 128);
    tt_tex_load(0x80172000, 736, 0);
    tt_tex_load(0x80174000, 896, 0);
    tt_tex_load(0x80176000, 896, 128);
    tt_tex_load(0x80178000, 0, 256);
    tt_tex_load(0x8017A000, 0, 384);
    tt_tex_load(0x8017C000, 32, 256);
    tt_tex_load(0x8017E000, 32, 384);
    tt_tex_load(0x80180000, 64, 256);
    tt_tex_load(0x80182000, 64, 384);
    tt_tex_load(0x80184000, 96, 256);
    tt_tex_load(0x80186000, 96, 384);
    tt_tex_load(0x80188000, 128, 256);
    tt_tex_load(0x8018A000, 128, 384);
    tt_tex_load(0x8018C000, 160, 256);
    tt_tex_load(0x8018E000, 160, 384);
    tt_tex_load(0x80190000, 192, 256);
    tt_tex_load(0x80192000, 192, 384);
    tt_tex_load(0x80194000, 224, 256);
    tt_tex_load(0x80196000, 224, 384);
    tt_tex_load(0x80198000, 256, 256);
    tt_tex_load(0x8019A000, 256, 384);
    tt_tex_load(0x8019C000, 288, 256);
    tt_tex_load(0x8019E000, 288, 384);
    tt_tex_load(0x801A0000, 320, 256);
    tt_tex_load(0x801A2000, 320, 384);
    tt_tex_load(0x801A4000, 512, 256);
    tt_tex_load(0x801A6000, 512, 384);
    tt_tex_load(0x801A8000, 608, 256);
    tt_tex_load(0x801AA000, 608, 384);
    tt_tex_load(0x801AC000, 640, 256);
    tt_tex_load(0x801AE000, 640, 384);
    tt_tex_load(0x801B0000, 672, 256);
    tt_tex_load(0x801B2000, 576, 256);
    tt_tex_load(0x801B4000, 576, 384);

    r.a = 960; r.b = 384; r.c = 24; r.d = 48;
    LoadImage(&r, 0x801B6000);
    r.a = 960; r.b = 511; r.c = 16; r.d = 1;
    LoadImage(&r, (s32)tt_clut_dat);
    DrawSync(0);

    tt_cd_read_sync(0x80010000, 0xC099, 3616);
    tt_cd_read_sync(0x80162000, 0xC079, 0xFED0);
    tt_cd_read_sync(0x80012000, 0xD6E2, 9248);
    tt_cd_read_sync(0x8001C000, 0xD686, 27108);
    tt_cd_read_sync(0x80172000, 0xD693, 0x00027D60);
    tt_cd_read_sync(0x80016000, 0xC8B6, 9248);
    tt_cd_read_sync(0x8002E800, 0xC865, 27108);
    tt_cd_read_sync(0x8019A000, 0xC869, 0x00027D60);
    tt_cd_read_sync(0x8001A000, 0xC075, 3616);
    tt_cd_read_sync(0x801C2000, 0xC040, 0x0001A680);
}
