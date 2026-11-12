/* Address: 0x80084A54 */
/* Size: 96 bytes */
/* Region: IDO (0x80081000-0x800BFFFF) */
extern s32 D_800FF864;

extern void girl_menu_set(s32 arg0);
extern void girl_menu_select(s32 arg0);
extern void game_reset(void);

void holiday_switch0(void)
{
    s32 state;

    state = D_800FF864;

    if (state == 0) {
        girl_menu_set(0);
    } else if (state == 1) {
        girl_menu_select(0);
    } else {
        game_reset();
    }
}
