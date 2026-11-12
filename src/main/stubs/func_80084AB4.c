/* Address: 0x80084AB4 */
/* Size: 96 bytes */
/* Region: IDO (0x80081000-0x800BFFFF) */
extern s32 D_800FF864;

extern void girl_menu_set(s32 arg0);
extern void girl_menu_select(s32 arg0);
extern void game_reset(void);

void holiday_switch1(void)
{
    s32 state;

    state = D_800FF864;

    if (state == 0) {
        girl_menu_set(1);
    } else if (state == 1) {
        girl_menu_select(1);
    } else {
        game_reset();
    }
}
