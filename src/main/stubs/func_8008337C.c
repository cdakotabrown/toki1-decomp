/* Address: 0x8008337C */
/* Size: 96 bytes */
/* Region: IDO (0x80081000-0x800BFFFF) */
extern s32 D_800FF864;

extern void menu2_init(void);
extern void menu_sel(void);
extern void game_reset(void);

void menu2_main(void)
{
    s32 state;

    state = D_800FF864;

    if (state == 0) {
        menu2_init();
    } else if (state == 1) {
        menu_sel();
    } else {
        game_reset();
    }
}
