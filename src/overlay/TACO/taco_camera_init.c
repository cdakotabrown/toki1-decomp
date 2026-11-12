/* Address: 0x80143954 */
/* Size: 128 bytes (32 insns) */
#include "types.h"
#include "tokimeki/libgs_types.h"

extern s32 ref_view;

extern void GsSetRefView2();
extern void GsSetProjection();
extern void GsSetNearClip();
extern void GsSetFarClip();

void taco_camera_init(void)
{
    GsSetProjection(2048);
    ref_view = 0;
    (*(LIBGS_RVIEW2 *)&ref_view).vpy = 0;
    (*(LIBGS_RVIEW2 *)&ref_view).vpz = 2048;
    (*(LIBGS_RVIEW2 *)&ref_view).vrx = 0;
    (*(LIBGS_RVIEW2 *)&ref_view).vry = 0;
    (*(LIBGS_RVIEW2 *)&ref_view).vrz = 0;
    (*(LIBGS_RVIEW2 *)&ref_view).rz = 0;
    (*(s32 *)&(*(LIBGS_RVIEW2 *)&ref_view).super) = 0;
    GsSetRefView2(&ref_view);
    GsSetNearClip(-100);
    GsSetFarClip(32767);
}
