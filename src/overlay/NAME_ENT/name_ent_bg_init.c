/* @CompileFlags: -cckr */
#include "types.h"

extern u32 bg_set_adr;
extern u32 *tpage_set_adr;
extern u32 Bg;

extern u8 sd_proc_on[];
#define G_EF988 ((u32 *)(sd_proc_on + 0x11F4))
#define G_F01E4 (*(u32 *)(sd_proc_on + 0x1A50))
#define G_F01CC (*(u32 *)(sd_proc_on + 0x1A38))

extern void name_ent_res_adr_set();
extern void bg_set();
extern int search_tpage_multi();
extern void palette_upload_enqueue();

void name_ent_bg_init()
{
    u32 base;
    int idx;
    int i;
    u32 addrs[6];

    name_ent_res_adr_set();
    base = bg_set_adr;
    for (i = 0; i < 6; i++) addrs[i] = (0, base) + i * 0x4000;
    bg_set(addrs, 0, 6, 3, 2, 504, -160, -120, 8, 13);
    Bg |= 0x40000000;
    idx = search_tpage_multi(*tpage_set_adr, 2);
    G_EF988[idx] = *tpage_set_adr;
    palette_upload_enqueue(G_F01E4, 24, 1, 2, 0);
    palette_upload_enqueue(G_F01CC, 16, 1, 2, 0);
}
