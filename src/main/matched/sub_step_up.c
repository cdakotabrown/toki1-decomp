/* Address: 0x800429B8 */
/* Size: 68 bytes (17 instructions) */
/* Region: GCC (0x80041000-0x80080FFF) */
typedef unsigned char u8;
typedef signed int s32;

extern u8 now_sub_step;
extern u8 now_sub_sub_step;
extern s32 step_state;
extern s32 step_counter;
extern u8 scene_entry_c;
extern u8 step_mode;

s32 sub_step_up(void)
{
    int *new_var2;
    int new_var;

    now_sub_step = now_sub_step + 1;
    now_sub_sub_step = 0;
    step_state = 0;
    new_var = 0;
    step_counter = new_var;
    scene_entry_c = 0;
    step_mode = *(new_var2 = &new_var);

    if (1) {
        return 0;
    }
}
