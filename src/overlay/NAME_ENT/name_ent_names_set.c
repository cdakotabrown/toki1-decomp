/* @SymbolStart: 0x80132000 */
/* @SymbolSize: 156 */
/* @PostProc: patch_stack_complete --old 40 --new 56 */
/* @PostProc: remap_stack_slots --map 24:44,28:48 */
#include "types.h"

extern void strncpy();

extern u8 name_ent_sei_dflt_tbl[];
extern u8 name_ent_mei_dflt_tbl[];
extern u8 name_ent_adana_dflt_tbl[];
extern u8 name_ent_sei[];
extern u8 name_ent_mei[];
extern u8 name_ent_adana[];

void name_ent_names_set(s32 arg)
{
    s32 v1;
    s32 v0;

    v1 = arg % 15;
    v0 = v1 * 8;
    strncpy(name_ent_sei, name_ent_sei_dflt_tbl + v0, 8);
    strncpy(name_ent_mei, name_ent_mei_dflt_tbl + v0, 8);
    strncpy(name_ent_adana, name_ent_adana_dflt_tbl + v1 * 12, 12);
}
