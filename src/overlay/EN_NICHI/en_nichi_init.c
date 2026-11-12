/* Address: 0x80132040 */
/* Size: 128 bytes (32 instructions) */
/* @SymbolSize: 128 */
#include "types.h"

extern void * en_nichi_mod_hdr;
extern void * en_nichi_tpage_tbl_ptr;
extern void * en_nichi_cels_ptr;
extern void * en_nichi_anim_ptr;
extern s16 en_nichi_aux_wk;
extern void * en_nichi_pal_ptr;
extern void * en_nichi_srn_ptr;
extern void * en_nichi_mod_src;

void en_nichi_init(void)
{
    en_nichi_mod_hdr = (void *)0x801E0400;
    en_nichi_tpage_tbl_ptr = (void *)0x801E20F8;
    en_nichi_cels_ptr = (void *)0x801E2120;
    en_nichi_anim_ptr = (void *)0x801E21C4;
    en_nichi_aux_wk = *(s16 *)0x801E5A10;
    en_nichi_pal_ptr = (void *)0x801B0000;
    en_nichi_srn_ptr = (void *)0x801E21CC;
    en_nichi_mod_src = (void *)0x801E0408;
}
