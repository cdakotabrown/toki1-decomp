/* Address: 0x801395E8 */
/* Size: 352 bytes (88 instructions) */
#include "types.h"
#include "tokimeki/etc_ev_res.h"

extern void etc_ev2f_res_set(void);
extern void palette_upload_enqueue(s32 a0, s32 a1, s32 a2, s32 a3, s32 stack0);
extern void bg_set6(s32 a0, s32 a1, s32 a2, s32 a3,
                          s32 stack0, s32 stack1);
extern void date_object_set(s32 a0, s32 a1, s32 a2, s16 a3);

extern s32 etc_rec_variant;

typedef struct {
    u8  pad0[3];
    u8  f3;
    u8  pad4[3];
    u8  f7;
    u8  pad8[34];
    s16 f42;
    u8  pad44[24];
} Rec68;

extern Rec68 class_w[];
extern ETC_EV_RES etc_ev2f_res;

void etc_main2f(void)
{
    s32 mode;

    etc_ev2f_res_set();
    mode = etc_rec_variant;

    if (mode == 0) {
        palette_upload_enqueue(etc_ev2f_res.pal, 17, 1, 2, 0);
    } else if (mode == 1) {
        palette_upload_enqueue(etc_ev2f_res.pal, 17, 1, 2, 1);
    } else if (mode == 2) {
        palette_upload_enqueue(etc_ev2f_res.pal, 17, 1, 2, 2);
    }

    bg_set6(etc_ev2f_res.bg[0], etc_ev2f_res.bg[1], etc_ev2f_res.bg[2], etc_ev2f_res.bg[3],
                  etc_ev2f_res.bg[4], etc_ev2f_res.bg[5]);
    date_object_set(etc_ev2f_res.cels, etc_ev2f_res.anim_tbl, etc_ev2f_res.hdr, etc_ev2f_res.scale);

    class_w[96].f3 |= 0x80;
    class_w[96].f7 = 0x80;
    class_w[96].f42 = -101;
    class_w[97].f3 |= 0x80;
    class_w[97].f7 = 0x80;
    class_w[97].f42 = -101;
}
