/* @SymbolStart: 0x8013FFB0 */
/* @SymbolSize: 344 */
/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=2:v1 */
/* name_ent_album_deletee4 - NAME_ENT overlay, Address: 0x8013FFB8, Size: 336 bytes (84 insns) */
#include "types.h"

extern s32 card_file_delete(s32);
extern void printf(u8 *);
extern void card_state_clear(void);
extern void save_flag_clear(void);
extern void goto_sub_step(s32);
extern void card_step_init(void);
extern s32 card_file_write_main(void);

typedef struct {
    u8 char_idx;
    u8 step;
    u8 pad1[7];
    u8 f875;
    u8 pad2[2];
    u8 f878;
    u8 pad3[7];
    s32 f880;
} NameEntWork;
extern NameEntWork scene_entry_c;

extern u8 name_ent_album_deletee4_str0[];
extern u8 album_status[];

void name_ent_album_deletee4(void)
{
    s32 result;

    switch (scene_entry_c.step) {
    case 0:
        result = card_file_delete(scene_entry_c.f878);
        if (result == 1) {
            printf(name_ent_album_deletee4_str0);
            album_status[scene_entry_c.f878] = 0;
            scene_entry_c.step = scene_entry_c.step + 1;
        } else {
            card_state_clear();
            save_flag_clear();
            goto_sub_step(1);
        }
        break;
    case 1:
        card_step_init();
        album_status[34] = scene_entry_c.f878;
        scene_entry_c.f878 = 0;
        scene_entry_c.step = scene_entry_c.step + 1;
        break;
    case 2:
        result = card_file_write_main();
        if (result == 19) {
            card_state_clear();
            save_flag_clear();
            goto_sub_step(1);
            break;
        }
        if ((u32)scene_entry_c.f880 >= 513 || scene_entry_c.f875 == 240) {
            card_state_clear();
            save_flag_clear();
            goto_sub_step(1);
        }
        break;
    }
}
