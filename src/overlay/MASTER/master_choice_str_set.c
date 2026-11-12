/* @SymbolSize: 96 */
/* master_choice_str_set - MASTER overlay, Address: 0x80138E58, Size: 96 bytes (24 insns) */
#include "types.h"

typedef struct {
    s32 entries[3];
} Table3;

extern Table3 master_choice_str_set_tbl;
extern s32 serifu_no;
extern void strcpy(u8 *, s32);
extern u8 aname;

void master_choice_str_set(void) {
    s32 idx;
    Table3 buf;

    buf = master_choice_str_set_tbl;
    idx = serifu_no;
    strcpy(&aname, buf.entries[idx]);
}
