/* Address: 0x80146120, Size: 116 bytes (29 instructions) */
#include "types.h"

extern void *date_workbuf_tbl[];

void date_workbuf_init(void)
{
    date_workbuf_tbl[0] = (void *)0x801C6000;
    date_workbuf_tbl[1] = (void *)0x801AE000;
    date_workbuf_tbl[2] = (void *)0x801B2000;
    date_workbuf_tbl[3] = (void *)0x801B6000;
    date_workbuf_tbl[4] = (void *)0x801BA000;
    date_workbuf_tbl[5] = (void *)0x801BE000;
    date_workbuf_tbl[6] = (void *)0x801C2000;
}
