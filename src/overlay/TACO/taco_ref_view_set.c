/* @SymbolStart: 0x80143FA4 */
/* @SymbolSize: 160 */
/* Address: 0x80143FAC */
/* Size: 144 bytes (36 insns) */
#include "types.h"
#include "tokimeki/libgs_types.h"

extern s16 * taco_actor_rec;

extern s32 ref_view;

extern void GsSetRefView2(s32 *p);

void taco_ref_view_set(void)
{
    s16 *rec = taco_actor_rec;

    ref_view = rec[58];
    (*(LIBGS_RVIEW2 *)&ref_view).vpy = rec[59];
    (*(LIBGS_RVIEW2 *)&ref_view).vpz = rec[60];
    (*(LIBGS_RVIEW2 *)&ref_view).vrx = rec[58];
    (*(LIBGS_RVIEW2 *)&ref_view).vry = rec[59];
    (*(LIBGS_RVIEW2 *)&ref_view).vrz = 0;
    (*(LIBGS_RVIEW2 *)&ref_view).rz = rec[52] * 360;
    (*(s32 *)&(*(LIBGS_RVIEW2 *)&ref_view).super) = 0;
    GsSetRefView2(&ref_view);
}
