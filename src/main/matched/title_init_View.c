/* Likely Symbol: title_init_View (from o.bin, offset +28 bytes from 0x8006195C) */
/* Address: 0x80061940 */
/* Size: 128 bytes (32 instructions) */
/* Compiler: GCC-like (game code region) */
/* Region: 0x80041000-0x80080FFF */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"
#include "tokimeki/libgs_types.h"

extern void GsSetProjection(s32);
extern void GsSetRefView2(s32 *);
extern void GsSetNearClip(s32);
extern void GsSetFarClip(s32);

extern s32 ref_view;

void title_init_View(void) {
    GsSetProjection(1000);
    ref_view = 0;
    (*(LIBGS_RVIEW2 *)&ref_view).vpy = 0;
    (*(LIBGS_RVIEW2 *)&ref_view).vpz = 1000;
    (*(LIBGS_RVIEW2 *)&ref_view).vrx = 0;
    (*(LIBGS_RVIEW2 *)&ref_view).vry = 0;
    (*(LIBGS_RVIEW2 *)&ref_view).vrz = 0;
    (*(LIBGS_RVIEW2 *)&ref_view).rz = 0;
    (*(s32 *)&(*(LIBGS_RVIEW2 *)&ref_view).super) = 0;
    GsSetRefView2(&ref_view);
    GsSetNearClip(100);
    GsSetFarClip(0x10000);
}
