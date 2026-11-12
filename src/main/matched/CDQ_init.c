/* Address: 0x800454F4 */
/* Size: 64 bytes (16 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

typedef struct {
    u8 f0;
    u8 f1;
    u8 f2;
    s8 f3;
} Block8DA0;

typedef struct {
    u32 f0;
    u8 f4;
    u8 f5;
} Block9228;

Block8DA0 cdq_head = { 0, 0, 0, 0 };
Block9228 cdq_result;
u8 cdq_footer_chk = 0;
u32 last_sector = 0;

void CDQ_init(void) {
    cdq_head.f0 = 0;
    cdq_head.f1 = 0;
    cdq_head.f2 = 2;
    cdq_head.f3 = -1;
    cdq_result.f0 = 0;
    cdq_result.f4 = 0;
    cdq_result.f5 = 0;
    cdq_footer_chk = 0;
    last_sector = 0;
}
