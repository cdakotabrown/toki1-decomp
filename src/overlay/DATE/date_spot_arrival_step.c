/* @RodataBase: 0x8015B050 */
/* @PostProc: patch_stack_complete --old 344 --new 368 */
/* @PostProc: remap_stack_slots --from 40 --to 44 */
/* @PostProc: swap_instructions --offset1 0x22C --offset2 0x230 */
/* @PostProc: patch_insn_reg_field 0x14:rt:v0:v1 0x24:rs:v0:v1 0x24:rt:v0:v1 0x5c:rt:v0:v1 0x60:rd:v1:v0 0x68:rs:v1:v0 0xb8:rs:v1:v0 0xc8:rs:v1:v0 0xd8:rs:v1:v0 0xe8:rs:v1:v0 0xf8:rs:v1:v0 0x108:rs:v1:v0 0x118:rs:v1:v0 0x128:rs:v1:v0 0x138:rs:v1:v0 0x148:rs:v1:v0 0x158:rs:v1:v0 0x168:rs:v1:v0 0x178:rs:v1:v0 0x188:rs:v1:v0 0x198:rs:v1:v0 0x1a8:rs:v1:v0 0x1b8:rs:v1:v0 0x1c8:rs:v1:v0 0x1d8:rs:v1:v0 0x1e4:rs:v1:v0 0x1f0:rs:v0:v1 0x204:rt:a1:a2 0x208:rs:a1:a2 0x208:rt:a1:a2 0x210:rt:a1:a2 0x214:rt:a2:a1 0x218:rs:a2:a1 0x218:rt:a2:a1 0x220:rs:a2:a1 0x22c:rs:a1:a2 0x22c:rt:a1:a2 0x230:rs:a2:a1 0x230:rt:a2:a1 0x240:rt:a1:a2 0x248:rs:a2:a1 0x258:rt:a1:a2 0x260:rs:a2:a1 0x280:rt:v0:v1 0x284:rs:v0:v1 0x284:rt:v0:v1 0x294:rt:v0:v1 0x298:rd:v1:v0 0x2c0:rs:v0:a0 0x2c0:rt:a0:v1 0x2c8:rs:v0:v1 0x2d8:rt:v0:v1 0x2dc:rs:v0:v1 0x2dc:rt:v0:v1 0x2ec:rt:v0:v1 0x2f0:rd:v1:v0 0x31c:rs:v0:a0 0x31c:rt:a0:v1 0x324:rs:v0:v1 0x334:rt:v0:v1 0x338:rs:v0:v1 0x338:rt:v0:v1 0x348:rt:v0:v1 0x34c:rd:v1:v0 0x358:rs:v0:a0 0x358:rt:a0:v1 0x378:rt:v0:v1 0x37c:rs:v0:v1 0x37c:rt:v0:v1 0x38c:rt:v0:v1 0x390:rd:v1:v0 0x39c:rs:v0:a0 0x39c:rt:a0:v1 0x3bc:rt:v0:v1 0x3c0:rs:v0:v1 0x3c0:rt:v0:v1 0x3d0:rt:v0:v1 0x3d4:rd:v1:v0 0x3dc:rs:v1:v0 0x3fc:rt:v0:v1 0x400:rs:v0:v1 0x400:rt:v0:v1 0x408:rs:v0:v1 0x410:rs:v0:v1 */
/* Address: 0x8014494C, Size: 1100 bytes (275 insns) */
/* @SymbolSize: 1100 */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[76];
} HandlerTable;

extern u8 date2_menu_select_tbl1;
extern u8 now_basyo;
extern u8 date_pattern;
extern u8 now_year;
extern u8 now_month;
extern u8 now_day;
extern u8 machiawase_mode;
extern u8 now_sub_sub_step;
extern s32 date_disp_flag;
extern u8 fade_bri;
extern HandlerTable date_main0_tbl;

extern void device_cmd_send(s32);
extern void date_light_effect(void);
extern void serifu_default_init(s32);
extern void date_movie_effect_step(s32);
extern void date_voice_set(s32);
extern void date_zoo_voice_set(s32);
extern void date_shopping_choice_step(s32);
extern void date_planetarium_voice_set(s32);
extern void date_event_set(s32);
extern void date_library_voice_set(s32);
extern void date_game_center_voice_set(s32);
extern void date_movie_serifu_step(s32);
extern void date_talk_step(s32);
extern void date_walk_step(s32);
extern void date_skating_step(s32);
extern void date_aquarium_step(s32);
extern void func_80135F0C(s32);
extern void date_zoo_step(s32);
extern void date_planetarium_step(s32);
extern void date_basyo17_init(s32);
extern void date_basyo18_init(s32);
extern void date_basyo19_init(s32);
extern void date_basyo20_init(s32);

void date_spot_arrival_step(void)
{
    HandlerTable table;
    u32 cmd;
    u32 com;

    cmd = now_sub_sub_step;
    table = date_main0_tbl;
    if (table.ptrs[cmd] == serifu_default_init) {
        switch (now_basyo) {
        case 1:
            table.ptrs[cmd] = date_movie_effect_step;
            break;
        case 2:
            table.ptrs[cmd] = date_skating_step;
            break;
        case 3:
            table.ptrs[cmd] = date_voice_set;
            break;
        case 4:
            table.ptrs[cmd] = date_zoo_voice_set;
            break;
        case 5:
            table.ptrs[cmd] = date_shopping_choice_step;
            break;
        case 6:
            table.ptrs[cmd] = date_planetarium_voice_set;
            break;
        case 7:
            table.ptrs[cmd] = date_event_set;
            break;
        case 8:
            table.ptrs[cmd] = date_library_voice_set;
            break;
        case 9:
            table.ptrs[cmd] = date_game_center_voice_set;
            break;
        case 10:
            table.ptrs[cmd] = date_movie_serifu_step;
            break;
        case 11:
            table.ptrs[cmd] = date_talk_step;
            break;
        case 12:
            table.ptrs[cmd] = date_walk_step;
            break;
        case 13:
            table.ptrs[cmd] = date_aquarium_step;
            break;
        case 14:
            table.ptrs[cmd] = func_80135F0C;
            break;
        case 15:
            table.ptrs[cmd] = date_zoo_step;
            break;
        case 16:
            table.ptrs[cmd] = date_planetarium_step;
            break;
        case 17:
            table.ptrs[cmd] = date_basyo17_init;
            break;
        case 18:
            table.ptrs[cmd] = date_basyo18_init;
            break;
        case 19:
            table.ptrs[cmd] = date_basyo19_init;
            break;
        case 20:
            table.ptrs[cmd] = date_basyo20_init;
            break;
        }
    }

    com = now_basyo;
    if (cmd != 0) {
        if ((now_year == 95 && now_month == 7 && now_day == 23)
            || (now_year == 96 && now_month == 7 && now_day == 28)
            || (now_year == 97 && now_month == 7 && now_day == 27)) {
            if (fade_bri == 128) {
                date_light_effect();
                date_disp_flag = 1;
            }
        } else if (com == 17U && fade_bri == 128
                   && cmd != 20U && cmd != 21) {
            date_light_effect();
            date_disp_flag = 1;
        } else if (com == 16U && fade_bri == 128
                   && cmd != 20U && cmd != 21) {
            date_light_effect();
            date_disp_flag = 1;
        } else if (cmd == 20U && machiawase_mode == 17U) {
            date_light_effect();
            date_disp_flag = 1;
        } else if (cmd == 20U && machiawase_mode == 16U) {
            date_light_effect();
            date_disp_flag = 1;
        }
    }

    table.ptrs[now_sub_sub_step](128);

    if (now_basyo == 11 && now_sub_sub_step >= 44U && now_sub_sub_step < 48U
        && date2_menu_select_tbl1 == 1 && date_pattern != 2) {
        date_pattern = 2;
        device_cmd_send(0x601);
    }
}
