/* Address: 0x800425E8 */
/* Size: 100 bytes (25 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* Compiler: IDO 5.2 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern void bzero(void *dst, s32 size);
extern void vblank_counter_event_init(void);
extern void card_ev_set(void);
extern void InitPAD(void *a, s32 b, void *c, s32 d);
extern void StartPAD(void);
extern void ChangeClearPAD(s32 val);

extern u8 album_work;
extern u8 pad1_buf;
extern u8 pad2_buf;

void input_events_init(void)
{
    bzero(&album_work, 0xEE0);
    vblank_counter_event_init();
    card_ev_set();
    InitPAD(&pad1_buf, 8, &pad2_buf, 8);
    StartPAD();
    ChangeClearPAD(0);
}
