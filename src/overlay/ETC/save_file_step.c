/* @SymbolStart: 0x801458F4 */
/* @SymbolSize: 224 */
/* Address: 0x801458FC */
/* Size: 216 bytes (54 instructions) */
#include "types.h"

extern void save_file_card_check(void);
extern void save_file_slot_init(void);
extern void save_file_album_check(void);
extern void save_file_slot(void);
extern void save_file_write(void);
extern void save_card_notfound(void);
extern void save_card_full(void);
extern void save_card_format(void);

extern u8 now_sub_sub_step;

void save_file_step(void)
{
    s32 v0 = now_sub_sub_step;

    switch (v0) {
    case 0:
        save_file_card_check();
        break;
    case 1:
        save_file_slot_init();
        break;
    case 2:
        save_file_slot();
        break;
    case 3:
        save_file_write();
        break;
    case 160:
        save_card_format();
        break;
    case 192:
        save_file_album_check();
        break;
    case 240:
        save_card_full();
        break;
    case 255:
        save_card_notfound();
        break;
    }
}
