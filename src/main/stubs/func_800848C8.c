/* Address: 0x800848C8 */
/* Size: 120 bytes */
/* Region: IDO (0x80081000-0x800BFFFF) */
extern s32 k_wait_cnt;

extern s32 pad_data;

extern void get_k_speed(void);
extern void k_speed_set(s32 arg0);
extern void k_disp_inc(void);

void func_800848C8(void)
{
    s32 counter;
    s32 temp;

    counter = k_wait_cnt;
    k_wait_cnt = counter - 1;

    if (counter < 1) {
        k_wait_cnt = 0;
        temp = get_k_speed();

        if ((pad_data & 0x00600060) != 0) {
            k_speed_set(0);
        }

        k_disp_inc();
        k_speed_set(temp);
    }
}
