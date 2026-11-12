/* Address: 0x8005525C */
/* Size: 340 bytes (85 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* Compiler: IDO 5.2 */
/* @CompileEnv: IDO52_UGEN_TEMP_REG=14:t0,15:t1,16:t2,17:t9 */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: remap_stack_slots --map '32:48,36:52' */
/* @PostProc: swap_instructions --offset1 0xf0 --offset2 0xf4 */
/* @PostProc: swap_instructions --offset1 0x100 --offset2 0x104 */
#include "types.h"

extern void memcpy(void *, void *, s32);

extern volatile u8 last_album;
extern u8 now_year;
extern u8 now_month;
extern u8 now_day;
extern u8 album_work[];
extern u8 p_sei[];
extern u8 p_mei[];
extern u8 p_adana[];
extern u8 screen_w[];
extern u16 basyo_pack;

void save_state_set(void) {
    s32 idx;
    s32 i;
    u8 pad[80];

    (void)pad;
    idx = last_album & 0xF;

    album_work[idx + 2136] = now_year;
    album_work[idx + 2152] = now_month;
    album_work[idx + 2168] = now_day;

    memcpy(&album_work[idx * 8 + 2184], p_sei, 8);
    memcpy(&album_work[idx * 8 + 2312], p_mei, 8);
    memcpy(&album_work[(idx << 4) + 2440], p_adana, 16);

    for (i = 0; i < 9; i++) {
        *(u32 *)&album_work[idx * 4 + i * 64 + 2696] =
            *(u32 *)&screen_w[i * 4 + 252];
    }

    for (i = 0; i < 4; i++) {
        album_work[idx * 16 + 3272 + i * 4 + 1] = screen_w[i * 4 + 1305];
        album_work[idx * 16 + 3272 + i * 4 + 2] = screen_w[i * 4 + 1306];
        album_work[idx * 16 + 3272 + i * 4 + 3] = screen_w[i * 4 + 1307];
        album_work[idx * 16 + 3272 + i * 4 + 0] = screen_w[i * 4 + 1304];
    }

    album_work[idx + 3784] = (u8)((u32)basyo_pack >> 12);
}
