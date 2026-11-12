/* @SymbolStart: 0x8014B9D8 */
/* @RodataBase: 0x8015E780 */
/* Address: 0x8014B9E0 */
/* Size: 1536 bytes (384 insns) */
/* @SymbolSize: 1544 */
/* @PostProc: patch_insn_reg_field 0x504:rt:at:v1 0x508:rt:at:v1 0x3d0:rt:v0:v1 0x3d8:rs:v0:v1 0x3f8:rt:v0:v1 0x414:rt:v0:v1 0x41c:rs:v0:v1 0x43c:rt:v0:v1 0x458:rt:v0:v1 0x460:rs:v0:v1 0x480:rt:v0:v1 0x574:rt:v0:v1 0x57c:rs:v0:v1 */
#include "types.h"

extern void sub_sub_step_up(void);
extern void goto_step(s32);
extern void goto_sub_sub_step(s32);
extern s32 get_CD_State2(void);
extern void cd_read_start2(s32, s32, s32);
extern void game_reset(void);
extern void indexed_byte_lsb_set(s32, s32);
extern void value_cache_find_or_load(s32, s32);
extern s32 dec_bg_init(s32);
extern void bustup_cd_read(s32);
extern void bustup_ptr_init(void);
extern void fade_up_main(void);
extern void fade_down_main(void);
extern void fade_in_step(void);
extern void fade_in(void);
extern void fade_out_step(void);
extern void fade_out(void);
extern void zyotai_sprite_set_chk(void);
extern void serifu_run(void);
extern s32 get_g_zyotai_low(s32);
extern void bustup_load_step(s32);
extern void bunkasai_sub_reset(void);
extern void bunkasai_basyo_init(void);
extern void bunkasai_girl5_srf_bank_set(void);
extern s16 now_serifu;
extern u8 now_year;
extern u16 basyo_pack;
extern u8 now_girl;
extern u8 now_sub_step;
extern s32 bunka_sd_kind;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;
extern s16 mes_base_y;
extern u8 now_sub_sub_step;
void bunkasai_girl5_club2_club3_97_scene(void) {
  s32 v0;
  unsigned int n;
  switch (now_sub_sub_step)
  {
    case 0:
      if ((((u32) basyo_pack) >> 12) == 2)
    {
      value_cache_find_or_load(16636, 1);
    }
    else
    {
      value_cache_find_or_load(16645, 1);
    }
      sub_sub_step_up();
      break;

    case 1:
      fade_out_step();
      break;

    case 2:
      if ((get_CD_State2() & 1) != 0U)
    {
      bunkasai_basyo_init();
      v0 = dec_bg_init(1);
      indexed_byte_lsb_set(1, v0);
      bustup_cd_read(24503);
      sub_sub_step_up();
    }
      break;

    case 3:
      fade_in_step();
      break;

    case 4:
      if (get_CD_State2() & 1)
    {
      bustup_ptr_init();
      now_girl = 5;
      zyotai_sprite_set_chk();
      now_serifu = 41;
    }
      break;

    case 5:
      serifu_run();
      break;

    case 6:
      cd_read_start2(13, 0x801E8000, 19045);
      sub_sub_step_up();
      break;

    case 7:
      fade_up_main();
      break;

    case 8:
      if (get_CD_State2() & 1)
    {
      v0 = get_g_zyotai_low(5);
      switch (v0)
      {

          break;

        case 0: case 1:
          now_serifu = 5;
          break;

        case 2:
          now_serifu = 2;
          break;

        case 3:
          now_serifu = 3;
          break;

        case 4:
          now_serifu = 7;
          break;

      }

      bunkasai_girl5_srf_bank_set();
    }
      break;

    case 9:
      serifu_run();
      break;

    case 10:
      now_serifu = 36;
      sub_sub_step_up();
      break;

    case 11:
      serifu_run();
      break;

    case 12:
      v0 = get_g_zyotai_low(5);
      switch (v0)
    {

        break;

      case 0: case 1:
        now_serifu = 37;
        break;

      case 2:
        now_serifu = 38;
        break;

      case 3:
        now_serifu = 39;
        break;

      case 4:
        now_serifu = 40;
        break;

    }

      sub_sub_step_up();
      break;

    case 13:
      serifu_run();
      break;

    case 14:
      fade_out();
      break;

    case 15:
      bunka_sd_kind = 0;
      bunka_ret_sub_step = now_sub_step;
      bunka_ret_sub_sub_step = now_sub_sub_step + 1;
      goto_step(128);
      break;

    case 16:
      bustup_load_step(24503);
      break;

    case 17:
      bunkasai_basyo_init();
      mes_base_y = 192;
      bustup_ptr_init();
      now_girl = 5;
      zyotai_sprite_set_chk();
      cd_read_start2(13, 0x801E8000, 19045);
      break;

    case 18:
      v0 = get_g_zyotai_low(5);
      if (v0 == 4)
    {
      fade_in_step();
      break;
    }
      fade_in();
      break;

    case 19:
      if (get_CD_State2() & 1)
    {
      v0 = get_g_zyotai_low(5);
      switch (v0)
      {

          break;

        case 0: case 1:
          n = 3;
          if ((((u32) basyo_pack) >> 12) == n)
        {
          now_serifu = 50;
        }
        else
        {
          now_serifu = (now_year * n) - 244;
        }
          break;

        case 2:
          n = 3;
          if ((((u32) basyo_pack) >> 12) == n)
        {
          now_serifu = 51;
        }
        else
        {
          now_serifu = (now_year * n) - 243;
        }
          break;

        case 3:
          n = 3;
          if ((((u32) basyo_pack) >> 12) == n)
        {
          now_serifu = 52;
        }
        else
        {
          now_serifu = (now_year * n) - 242;
        }
          break;

        case 4:
          now_serifu = 15;
          break;

      }

      bunkasai_girl5_srf_bank_set();
    }
      break;

    case 20:
      serifu_run();
      break;

    case 21:
      v0 = get_g_zyotai_low(5);
      if (v0 == 4)
    {
      goto_sub_sub_step(26);
      break;
    }
      v0 = get_g_zyotai_low(5);
      switch (v0)
    {
      case 0:

      case 1:
        now_serifu = 8;
        break;

      case 2:
        now_serifu = 9;
        break;

      case 3:
        now_serifu = 10;
        break;

    }

      sub_sub_step_up();
      break;

    case 22:
      serifu_run();
      break;

    case 23:
      fade_down_main();
      break;

    case 24:
      if ((((u32) basyo_pack) >> 12) == 3)
    {
      now_serifu = 53;
    }
    else
    {
      now_serifu = 111;
    }
      sub_sub_step_up();
      break;

    case 25:
      serifu_run();
      break;

    case 26:
      fade_out_step();
      break;

    case 27:
      now_serifu = 0;
      sub_sub_step_up();
      break;

    case 28:
      serifu_run();
      break;

    case 29:
      bunkasai_sub_reset();
      break;

    default:
      game_reset();
      break;

  }

}
