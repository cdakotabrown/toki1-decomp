# Naming conventions of the 1994 KCET source (reverse-engineered from o.bin)

Written 2026-09-27. Every count below comes from
`python3 tools/analysis/naming_census.py style` (read-only; `--json OUT` for the raw
numbers, `tokens` for the full token table). If a count here disagrees with the tool,
the tool is right and this page is stale.

This is the style guide for every name the project adds, whether it comes from o.bin
or not. The goal it serves is a tree with no
`func_XXXXXXXX` and no meaningless `D_XXXXXXXX`, named so that a Konami programmer
of 1994 would recognise the names as house style.

> 86% of Konami's function names are lower `snake_case`, and there is no `g_`
> prefix anywhere in o.bin, so names here are never `PascalCase`, `camelCase` or
> `g_PascalCase`. Formatting is C89, `/* */` comments, 4-space indent, 80 columns.

---

## 0. The corpus

`disks/o.bin` is the linked debug build of the tiny `olh` overlay. Its external symbol
table lists every main-binary global that overlay's link could see: **2,132 names
(1,559 functions, 573 data)**, split at the debug text end 0x800C1480. It also holds
the overlay's own **5 procedures** (`olh_main`, `olh_init`, `olh`, `olh_exit`,
`move_menu_title`) and **9 data symbols** (`menu0`..`menu7`, `olh_menu`). It has no
types, locals or line numbers. File-scope `static` symbols were never externs, so
they aren't in it either. A large share of retail's `D_` objects are probably statics
that o.bin never named.

The debug link order puts the names into four bands. o.bin order is link order, but
the debug addresses do NOT carry over to retail:

| band | o.bin range | functions | data | who wrote it |
|---|---|---:|---:|---|
| **K-game** | `warm_reset_init` .. `twinbee` | 697 | 442 | Konami game core |
| **K-sd** | `SD_Call_SD` .. `cdread_callback` | 62 | 11 (`gSD*`) | Konami sound driver ("SD") |
| **K-date** | `normal_date_bg_suddenin` .. `pcm_read_myu` | 122 | — | Konami date/event engine |
| **Sony** | `DecDCTReset` .. `ChangeClearRCnt` | 678 | 120 | Psy-Q libs, libc, BIOS stubs |

"Konami names" below means K-game + K-sd + K-date: **881 functions + 453 data =
1,334 names.** The Sony data split uses an explicit name list (`SONY_DATA_*` in the
tool), because the debug data layout interleaves the two owners.

---

## 1. The ten rules that matter most

| # | rule | measure |
|---|---|---|
| 1 | Konami code is **lower `snake_case`** | 762 / 881 functions (86%); K-game alone 648 / 697 (93%); data 405 / 442 (92%) |
| 2 | **Sony names are Sony's, verbatim**, never invented | 678 functions + 120 data; `Gs*` 113, `Ss*` 69, `Set*` 67, `Spu*` 41, `Cd*` 24 |
| 3 | **Subject first, role last** (`hizuke_show`, not `show_hizuke`) | only 151 / 762 snake functions (20%) start with a verb |
| 4 | **Lifecycle suffixes `_init` / `_main` / `_exit`**, and the bare stem is the per-frame body | last token `init` 100, `main` 47, `exit` 28; 86 stems carry one of them; 20 full `X_init`/`X`/`X_exit` triplets |
| 5 | **Display vocabulary: `_disp`, `_show`, `_disp_switch`** | `show` 25, `disp` 9, `_disp_switch` 10, `X_init`+`X_show` pairs 6 |
| 6 | **Queries and decisions: `get_*`, `*_check`, `*_judge`** | `get_` 40 (36 as first token), `check` 26, `judge` 3 |
| 7 | **Ordinals are glued on with no underscore, and step numbers are in hex** | 115 glued decimals, 8 hex (`file_select_main4a`..`4e`), only 6 `_N` |
| 8 | **Leading underscore = the internal worker or variant** | 19 Konami functions; 6 have a public twin (`_schedule_init`/`schedule_init`) |
| 9 | **Game concepts are romanized Japanese**; keep o.bin's spelling of a concept | 251 / 1,334 names (19%) carry a Japanese token |
| 10 | **Short clipped abbreviations** for recurring nouns | `sd` 82, `bg` 37, `k` 21, `cd` 16, `dec` 16, `vram` 12, `seq` 12, `cal` 10, `para` 10, `xa` 10 |

Lengths: Konami functions median **14 characters / 3 tokens** (max 36 / 6); Konami
data median **10 characters / 2 tokens** (max 23 / 5). A new name longer than about
24 characters is out of style. The long o.bin names are all
`system_config_<item>_<phase>` menu handlers.

---

## 2. Function rules in detail

Each rule has its count and five real o.bin examples.

### R1 — lower snake_case, with named islands of other casing

| band | snake_lower | other casings |
|---|---:|---|
| K-game | 648 | CamelCase 13, lowerCamel 11, Prefix_CamelCase 9, mixed 16 |
| K-sd | 6 | **Prefix_CamelCase 54** (`SD_PlaySE`) |
| K-date | 108 | **Title_Snake 7**, Prefix_CamelCase 4, Capitalized 2 |

Examples: `warm_reset_init`, `draw_and_disp_env_init`, `game_system_init`,
`goto_step`, `normal_date_bg_in`.

The non-snake names are **islands, and each island has an owner**. Don't treat them
as a licence to mix styles:

- **SD driver** (`SD_Call_SD`, `SD_HandleVBLXA`, `SD_PlaySE`, `SD_FadeSeq`,
  `SD_GetCDLevel`): the sound driver is a separate module with its own
  `SD_` + CamelCase style and Hungarian `gSD*` data. Name new sound-driver symbols in
  that style and nowhere else.
- **Date-engine Title_Snake** (`Graph_Init`, `Vblnk_Timer_Init`, `Fade_Out_Main`,
  `Fade_In_Init`, `Shiori_Case_Of_Mine`, `Ev_Class_W_Switch`, `Get_Serifu`): one
  programmer's corner of K-date. Use it only for code demonstrably in that TU.
- **Code copied from Sony samples**: `strInit`, `strSetDefDecEnv`, `strCallback`,
  `strNext`, `strKickCD` (the MDEC movie-player sample); `InitMouse`, `MouseState`,
  `RangeMouse`, `SenseMouse`, `MouseRead` (the mouse sample); `LoadSquare`,
  `MoveSquare`, `StoreSquare`; `PadDataRead`, `MyPadRead`. These carry the sample's
  own names, and can be identified by T2 shape matching against the sample source.
- **Mixed-case acronym names**: `Control_CD`, `get_CD_State`, `get_XAsector`,
  `vab_State`, `CDQ_init`. Acronyms (`CD`, `XA`, `CDQ`) keep their capitals inside
  otherwise snake names. That's 16 names; don't generalise it beyond acronyms.

**New Konami names are snake_case.** Use CamelCase only when the symbol provably
belongs to one of the islands above.

### R2 — Sony SDK names are exact

678 Sony functions, 120 Sony data: `DecDCTReset`, `CdSearchFile`, `SsSeqPlay`,
`GsSortSprite`, `SetPolyFT4`; data `GsDRAWENV`, `_svm_vab_vh`, `_spu_keystat`,
`rsin_tbl`, `PadIdentifier`.

- Casing follows Sony's library convention: CamelCase with a library prefix
  (`Gs`, `Ss`, `Spu`, `Cd`, `St`, `DecDCT`); lowercase `_lib_` internals (`_spu_init`,
  `_card_info`, `_mk_spr_packet`, `_svm_*`); a handful of uppercase acronyms
  (`CD_sync`, `MDEC_vlc`, `GPU_cw`, `PAD_dr`); libc lowercase (`printf`, `memcpy`,
  `strncat`).
- **A Sony name is never derived from behaviour.** It comes from byte identity (T1)
  or source shape (T2) against Sony's own archives or source. See the pipeline doc.
  A library-band function that can't be tied to a Sony member keeps a
  behaviour name in **Konami** style, because by elimination it's Konami code
  (the SD driver, the date engine, `holiday_*`).
- **o.bin can't place Sony names in retail.** Its library-band addresses drift
  +0xBB48..+0xD6B0 non-uniformly, and the 2026-09-27 anchor audit refuted most
  proximity-placed Sony names (retail `SetProgramChange` is libgpu `DrawSyncCallback`).

### R3 — subject first, role last

151 of 762 snake functions (20%) start with a verb. The rest are
`<subsystem>_[<object>_]<role>`. Examples: `hizuke_show` (date display, show),
`dec_bg_cd_read` (MDEC background, CD read), `kega_hassei` (injury, onset),
`normal_date_girl_out_init`, `system_config_pad_mode_exit`.

Top first-token families (Konami snake functions): `file` 66 (`file_select_*`),
`normal` 40 (`normal_date_*`), `system` 37 (`system_config_*`), `get` 36,
`holiday` 22, `set` 20, `pre` 20 (`pre_syogatu_*`, `pre_date_*`, `pre_xmas_*`),
`game` 15 (`game_mode_*`), `load` 14, `title` 14, `k` 12 (kanji text), `event` 12,
`schedule` 12, `data` 11, `dec` 10 (MDEC), `icon` 9, `bust` 9 (`bust_up_*`),
`week` 9 (`week_day_*`).

Verbs that do lead, and what they mean:

| verb | n | meaning | examples |
|---|---:|---|---|
| `get_` | 36 | returns a value, no side effects | `get_season`, `get_pass_days`, `get_near_menu`, `get_ouen_girl`, `get_date_basyo_num` |
| `set_` | 20 | writes state / arms a resource | `set_brightness`, `set_kanji_string`, `set_c_girl`, `set_date_info`, `set_movie_offset` |
| `load_` | 14 | loads data or VRAM | `load_palette`, `load_string`, `load_vram`, `load_func_main`, `load_exe_bin` |
| `change_` | 10 | switches a mode / advances | `change_month`, `change_k_w`, `change_girl`, `change_dec_bg`, `change_yoshio` |
| `check_` | 8 | test, returns a flag | `check_load_func`, `check_date_info`, `check_para_limit`, `check_end_k`, `check_sum_check` |
| `goto_` | 7 | state-machine jump | `goto_step`, `goto_sub_step`, `goto_sub_sub_step`, `goto_tpage`, `goto_cs` |
| `show_` | 7 | draw a value (debug-ish) | `show_number`, `show_number_col`, `show_string_e`, `show_vcnt_info`, `show_packet_use_box` |

Other leading verbs (`make_` 5, `print_` 4, `search_` 4, `select_` 4, `swap_` 3,
`move_` 3, `put_` 3, `clear_` 3, `read_` 3, `return_` 2) are rare. For a new name,
**use `get_`/`set_`/`load_`/`goto_` as leading verbs, and otherwise put the subject
first.**

### R4 — lifecycle: `_init` / `_main` / `_exit`, bare stem = body

- Last-token counts: `init` 100, `main` 47, `exit` 28.
- 86 stems carry at least one of the three. The 20 full triplets `X_init` + `X` +
  `X_exit` are `system_config`, `file_select`, `join_club`, `week_day`,
  `holiday_club`, `holiday_tel`, `pre_date`, `data_save`, `data_load`,
  `callendar_disp`, plus 9 `system_config_<item>` menu items and `_pre_opening`.
- **The bare stem is the per-frame body**: `system_config`, `file_select`,
  `holiday_club`, `join_club`, `olh`. `X_main` is the entry/dispatcher when it
  exists (`game_mode_main`, `holiday_main`, `week_day_main`, `olh_main`).
- **Phases are numbered by gluing a digit on**: `game_mode_init0`, `game_mode_init1`,
  `pre_syogatu_init0`..`init3`, `vacation_day_init0`..`2`, `week_day_main0`,
  `uwasa_main0`.
- Other paired lifecycle words: `_start`/`_stop` (`start_card`, `stop_card`,
  `Hw_Start`, `Hw_Stop`), `_in`/`_out` for fades and slides (`white_in`,
  `title_white_out`, `normal_date_bg_in`, `normal_date_girl_out`, `title_fade_out`),
  and `_on`/`_off` (`dtd_on`, `SsUtReverbOn`, `all_object_disp_off`).

The olh object, the only overlay o.bin saw whole, is the canonical case:
**`olh_main` (entry) → `olh_init` → `olh` (per-frame) → `olh_exit`**, plus a helper
named for what it does (`move_menu_title`).

### R5 — display vocabulary

| suffix | n | meaning | examples |
|---|---:|---|---|
| `_show` | 25 | draw this frame | `hizuke_show`, `basyo_show`, `icon_show`, `mozaiku_show`, `menu_bar_show` |
| `_disp` | 9 | display a screen or panel | `game_info_disp`, `message_disp`, `callendar_disp`, `now_select_mode_disp`, `touzyou_fan_disp` |
| `_disp_switch` | 10 | turn a HUD widget on/off | `hizuke_disp_switch`, `icon_disp_switch`, `basyo_disp_switch`, `mascot_disp_switch`, `cursor_disp_switch` |
| `X_init` + `X_show` | 6 | widget setup + per-frame draw | `hizuke`, `icon`, `basyo`, `dec_bg`, `bust_up`, `message_window` |
| `_set` | 41 | build or arm a resource | `class_set`, `bg_set`, `menu_set`, `schedule_set`, `konami_mark_set` |
| `_clear` | 10 | reset a resource | `vram_clear`, `tpage_buf_clear`, `class_clear`, `k_sys_clear`, `schedule_buf_clear` |

A HUD widget in the date/schedule screens therefore has up to four names:
`<w>_disp_switch`, `<w>_init`, `<w>_show` and optionally `<w>_move`. New widgets
follow the same set.

### R6 — queries, decisions and paired events

- `*_check` (26) returns a condition: `birth_day_check`, `mem_card_check`,
  `hitsuji_check`, `radio_check`, `joho_check`.
- `*_judge` (3) decides whether a game event fires: `tokubetu_judge`, `geko_judge`,
  `date_sasoi_judge`.
- Paired event vocabulary: `<condition>_hassei` (onset) / `<condition>_kaifuku`
  (recovery): `kega_hassei`/`kega_kaifuku`, `noiroze_hassei`/`noiroze_kaifuku`,
  `sick_hassei`/`sick_kaifuku`. `*_up` / `*_down` for stat moves: `syoushin_up`,
  `yuukou_down`, `parameter_up_down`.
- **A `get_*` returns something.** The 2026-09-27 audit refuted several landed
  `get_*` names because the body was `void`. That test is mechanical, so apply it to
  every candidate.

### R7 — ordinals

- **Glued decimal, no underscore** (115): `main_pcm_trans2`, `select_girl2`,
  `bust_up_init3`, `file_select_init_card7`, `event_face_kuchi1`.
- **Hex step numbers** (8): `file_select_main40`..`file_select_main4e`. The
  `main4a`..`main4e` spellings show the numbers are the **hex step values**, not
  counters. This is the pattern for naming a step handler after its jump-table index
  (see §5).
- **Underscore ordinals** are rare (6): `menu_check_1`, `menu_check_2`,
  `_pre_opening_1`, `holiday_init_0`, `holiday_init_1`, `normal_date_speak_1line`.
- An ordinal also marks **variants of one routine**: `cdreaddata`/`cdreaddata2`,
  `mozaiku_show`/`mozaiku_show2`, `bg_read_sub2`, `swap_env3`, `make_color_bar16`.

### R8 — leading underscore

19 Konami functions; 6 have a public twin (`_schedule_init`/`schedule_init`,
`_schedule_set`/`schedule_set`, `_change_month`/`change_month`,
`_kega_check`/`kega_check`, `_load_vram`/`load_vram`, `_main`/`main`). Others:
`_pad_data_read_pad`, `_pad_data_read_mouse`, `_sys_default_tpage_set`,
`_game_work_init_set`, `_opening_movie`. **Use a leading underscore only for the
worker behind a public function of the same name**, or for a scene-internal variant
(`_pre_opening*`).

### R9 — romanized Japanese for game concepts

251 of 1,334 Konami names (19%) contain a Japanese token. Glossary, ordered by
frequency:

| token | n | gloss | examples |
|---|---:|---|---|
| `suuzi` | 19 | numeral (glyphs) | `suuzi0`..`suuzi16`, `suuzi` |
| `bukatu` | 15 | club activity | `bukatu_reset`, `bukatu_name0`, `bukatu_name_table` |
| `syogatu` | 9 | New Year | `pre_syogatu_init0`, `pre_syogatu`, `pre_syogatu_main` |
| `serifu` | 9 | spoken line | `now_serifu`, `ss_serifu`, `Get_Serifu` |
| `basyo` / `basho` | 8 / 1 | place | `basyo_init`, `basyo_show`, `get_date_basyo_num`, `basho_num` |
| `tokimeki` | 6 | the heart meter | `get_h_tokimeki`, `tokimeki_hosei_table`, `tokimeki_default_chara_init` |
| `uwasa` | 6 | rumour | `uwasa_set`, `uwasa_init`, `uwasa_main` |
| `kega` | 6 | injury | `kega_check`, `kega_hassei`, `kega_kaifuku` |
| `asobi` | 6 | play, an outing | `o_asobi_rom_table`, `o_asobi_animation_table` |
| `shugaku` | 6 | school trip | `shugaku_place`, `ss_serifu_shugaku` |
| `taku` | 5 | choice (N-way select) | `x_taku_menu_set`, `menu_girl_taku_set`, `date_x_taku_set` |
| `hizuke` | 5 | calendar date | `hizuke_init`, `hizuke_show`, `hizuke_hide` |
| `kuchi` | 5 | mouth (lip-flap) | `event_bustup_kuchi_ani`, `event_face_kuchi0` |
| `yuukou` | 4 | friendship | `get_h_yuukou`, `yuukou_down`, `yuukou_hosei_table` |
| `touzyou` | 4 | debut (a girl appears) | `touzyou_girl1`, `touzyou_fan_disp` |
| `byouki` | 3 | illness | `byouki_show`, `byouki_csr` |
| `hassei` / `kaifuku` | 3 / 3 | onset / recovery | `kega_hassei`, `sick_kaifuku` |
| `zyotai` | 2 | condition | `get_g_zyotai_s`, `get_g_zyotai_h` |
| `joho` / `jyouhou` | 2 / 1 | information | `joho_check`, `change_joho_look_f`, `JYOUHOU` |
| `yazirushi` / `yajirusi` | 2 / 2 | arrow | `move_yazirushi`, `set_yajirusi`, `yajirusi_timer` |
| `gamen` | 2 | screen | `last_gamen_mode`, `get_last_gamen_mode` |
| singletons | | | `hyouka` evaluation, `ouen` cheer, `sasoi` invitation, `kyuuka` vacation, `nenga` New-Year card, `gakko` school, `syoushin` promotion, `tokubetu` special, `geko` going home, `kaisu` count, `hosei` correction, `bairitu` multiplier, `mozaiku` mosaic, `hitsuji` sheep, `aisatu` greeting, `kigou` symbol, `demachiawase` meeting up, `migi`/`hidari` right/left, `daen` ellipse, `kao` face, `junban` turn order |

People: `fujisaki` (Shiori; also `fuji_para_tbl`, `ending_para_check_fuji`,
`fujisaki_2nin_birth_check`), `himo` (Yuina Himoo: `himo_robo_check`), `yoshio`/
`yosi`/`yoshi` (the friend Yoshio: `change_yoshio`, `yosi_trans`, `with_yoshi`),
`shiori` (`Shiori_Case_Of_Mine`). **Girls are named by surname** (the first
heroine's full surname, the others clipped). Stat names use one-kanji
abbreviations: `com_name_tai` (体), `_bun` (文), `_ri` (理), `_gei` (芸), `_un` (運),
`_zat` (雑), `_you` (容), `_kon` (根).

**Romanization is inconsistent, and the rule is to copy it, not normalise it.**
Konami mixed kunrei/wāpuro spellings (`syogatu`, `touzyou`, `zyotai`, `bukatu`,
`kaisu`, `yuukou`, long vowels as `ou`/`uu`) with Hepburn (`shugaku`, `fujisaki`,
`hassei`, `shiori`). The tool's substring heuristic finds 99 kunrei-like and 149
Hepburn-like names. The same concept is even spelled two ways (`basyo`/`basho`,
`yazirushi`/`yajirusi`, `joho`/`jyouhou`, `yosi`/`yoshi`). For a **new** name:

1. If o.bin already spells the concept, use o.bin's spelling. Where there are two,
   use the more frequent one (`basyo`, `yazirushi`, `joho`).
2. Otherwise use wāpuro/kunrei (`sy`, `zy`, `ty`, `tu`, `ou`/`uu` for long vowels),
   which is the majority for the game-domain vocabulary above.
3. English for engine/hardware concepts (`vram`, `palette`, `cursor`, `brightness`,
   `sprite`, `menu`), Japanese for the story/simulation domain (dates, stats,
   events, illnesses, seasons, places).

**Preserve Konami's typos in o.bin names**: `callendar_disp*` (4), `system_config_
mouse_botton_mode*` (3), `schdeule_girls_param`, `date_alubum_para_set`, `cashe_f`,
`load_fucn_check_sum` (11 names). Never introduce a new one.

### R10 — abbreviations

| abbr | n | meaning | examples |
|---|---:|---|---|
| `sd` | 82 | sound driver | `put_sd_error`, `set_sd_holiday`, `SD_PlaySeq`, `sd_bg_sector` |
| `bg` | 37 | background | `bg_init`, `dec_bg_show`, `back_bg_pri`, `get_weekly_bg_sector` |
| `k` | 21 | kanji text system | `k_printf`, `k_disp_start`, `k_speed_set`, `get_k_work`, `k_work` |
| `cd` | 16 | CD-ROM | `cd_command_set`, `Control_CD`, `get_CD_State`, `cd_read_end` |
| `dec` | 16 | MDEC decode | `dec_bg_reset`, `dec_env_set`, `set_dec_bri`, `dec_bg_brightness` |
| `mes` | 13 | message | `p_mes0`, `season_mess_clear`, `message_newspot` |
| `vram` | 12 | VRAM | `vram_clear`, `csr_load_vram`, `srn_vram_set` |
| `seq` | 12 | music sequence | `seq_switch`, `get_now_seq`, `cur_seq_num`, `season_seq` |
| `xa` | 10 | CD-XA audio | `xa_wait`, `seek_stop_xa`, `now_xa_num`, `XAsector` |
| `para` | 10 | parameter (stat) | `check_para_limit`, `para_sl`, `fuji_para_tbl` |
| `cal` | 10 | calendar | `cal_show`, `cal_font_load`, `cal_color`, `get_calx_pos` |
| `col` | 9 | colour | `show_number_col`, `sjis_col`, `byouki_col`, `cal_color` |
| `addr` / `adr` | 8 / 4 | address | `addr_init_main_rom`, `sd_adr`, `sde_adr` |
| `ev` | 7 | event | `ev_fadein`, `ev_me_on`, `card_ev_set` |
| `srn`, `mod` | 5, 5 | screen, mode | `srn_init`, `srn_w`, `mod_init`, `mod_w` |
| `buf`, `tbl` | 5, 2 | buffer, table | `tpage_buf_clear`, `string_buff`, `grp_tbl`, `fuji_para_tbl` |
| `csr`, `bri`, `fnt`, `sprt` | 4, 3, 1, 2 | cursor, brightness, font, sprite | `load_csr_ab`, `tbg_bri`, `get_fnt`, `set_tarao_sprt` |
| `g_`, `p_`, `h_` | 5, 18, 4 | girl, player, (heroine?) | `get_g_name`, `get_p_name`, `p_mes0`, `get_h_tokimeki` |
| `_f`, `_m`, `_s`/`_h` | 5, 4, 3 | flag; message variant; variants | `cashe_f`, `change_joho_look_f`, `xa_set_m`, `radio_check_m`, `get_g_zyotai_s` |

**`g_` means GIRL in this codebase**, which is another reason a
`g_PascalCase` global convention can't be used for symbol names.

---

## 3. Data rules

Konami data: 453 names (442 K-game + 11 K-sd). 92% of K-game data is lower snake.

| pattern | n | meaning | examples |
|---|---:|---|---|
| `now_*` | 21 | current state | `now_select_menu`, `now_icon`, `now_dec_bg_show`, `now_serifu`, `now_xa_num` |
| `last_*` | 7 | previous state | `last_sector`, `last_m_seq`, `last_s_se`, `last_gamen_mode`, `last_x_y` |
| `*_sector` | 17 | CD sector (LBA) tables | `dec_bg_sector`, `radio_sector`, `morning_sector`, `club_bg_sector`, `aisatu_sector` |
| `*_table` / `*_tbl` | 18 | lookup tables | `tokimeki_hosei_table`, `tokimeki_check_table`, `yuukou_hosei_table`, `grp_tbl`, `fuji_para_tbl` |
| `*_timer` / `*_cnt` / `*_counter` | 10 | countdowns and counters | `konami_command_timer`, `full_move_timer`, `season_timer`, `frame_cnt`, `trans_wait_counter` |
| `*_flag` / `*flag` | 6 | booleans | `konami_command_flag`, `trans_flag`, `sd_err_flag`, `print_flag`, `msflag` |
| `*_mode` / `*_kind` | 12 | enumerated state | `pad_mode`, `rgb_mode`, `message_kind`, `mascot_kind`, `ss_s_kind` |
| `*name*` | 92 | string tables | `date_name`, `tc_name`, `bukatu_name_table`, `card_file_name`, `concert_names` |
| `*_string` / `mes` | 27 | UI strings | `yes_string`, `no_string`, `migi_string`, `p_mes3`, `message_main` |
| `_x`/`_y`/`_w`/`_h` | 25 | geometry | `menu_x`, `menu_w`, `bg_scroll_x`, `fan_x_pos`, `icon_xy_a` |
| work areas | 13 | scratch/work blocks | `k_work`, `class_w`, `srn_w`, `mod_w`, `work_tarao` |
| SCREAMING_SNAKE | 31 | menu labels and tuning constants | `BUNKEI`, `SAVETYUU`, `STRESS_TABLE`, `SYOUSIN_BAIRITU`, `END_PLUS_POINT` |
| `o_<scene>_*` / `a_*` | 15 | animation sets | `o_main_rom_table`, `o_main_pattern_table`, `o_main_animation_table`, `o_main_max_animation`, `o_main_opd_info` |
| `gSD*` (K-sd) | 11 | sound-driver state | `gSDEvt`, `gSDInfo`, `gSDSeqTable`, `gSDCDLevel`, `gSDBodyAdrs` |
| Capitalized | 4 | engine singletons | `Sprites`, `Bg`, `Map`, `Cell` |

**Numbered elements plus an aggregate.** Konami declared each string or image as its
own object and then a table of them. There are 6 such families in o.bin:
`suuzi0`..`suuzi16` + `suuzi`, `date_name_0`..`_20` + `date_name`,
`tc_name_0`..`_15` + `tc_name`, `tc_sname_0`..`_15` + `tc_sname`,
`icon_image0`..`icon_imagec` (hex) + `icon_image`,
`bukatu_name0`..`11` + `bukatu_name_table`, and `p_mes0`..`p_mes12` + `player_message`.
When retail has a pointer table whose entries point at N adjacent objects, name them
`<base>N` / `<base>` (or `<base>_table`) the same way.

**An animation set is five objects with fixed suffixes**: `_rom_table`,
`_pattern_table`, `_animation_table`, `_max_animation`, `_opd_info`, prefixed
`o_<scene>_` (`o_main_`, `o_asobi_`), `a_` and `sd_`. Name the overlays' own sets
`o_<overlay>_*`.

**Work areas are named for a person, not a purpose**: `work_tarao`, `work_miura`,
`work_takak`, `work_yamag`, `work_fuku`, plus functions `set_tarao_bg`,
`set_tarao_sprt`, `set_tarao_rect`, `clear_work_tarao`, `_sprite_set_box_shade_tarao`.
These look like per-programmer scratch blocks (clipped surnames, 5 characters max).
**Keep the o.bin names, but never invent a person tag** for a new symbol. Name new
scratch by purpose (`*_work`).

---

## 4. Rules for names o.bin never saw

o.bin never saw overlay-local functions and data (≈6,195 functions and ≈9,257
per-overlay data addresses), file-scope statics, struct fields or locals. Those get
**behaviour-derived names in the style above** (tier T4 in the pipeline). The rules
below apply §1–§3 to those cases.

### 4.1 Overlay-local symbols

1. **Prefix = the overlay's lowercase name**: `bunkaken_`, `taco_`, `rpg_bat_`,
   `name_ent_`, `en_nichi_`, `date2_`. The `olh` object is the precedent. Where
   o.bin's own scene vocabulary is clearer, the scene word may replace the file name
   for scene-level functions (`shugaku_*` for SHUGAKU is both).
2. **Lifecycle, exactly as olh**: the function installed as the overlay entry
   (`D_8012B5F0`) is `<ov>_main`; its init, per-frame body and teardown are
   `<ov>_init`, `<ov>`, `<ov>_exit`. Only these carry the prefix unconditionally.
3. **Step handlers take their table index**: a function reached through a jump table
   on a step byte is `<ov>_<role><hex index>`, following `file_select_main4a`. Say
   the SUB table is on `D_800EF869` and the handler for sub 0x12 builds a menu: it's
   `<ov>_main12`, or with a behaviour role `<ov>_menu12`. Phase ordinals within one
   sub stay glued decimal (`_init0`, `_init1`) as in §2 R4. **Record the table
   position in the evidence** (pipeline doc §6), because the name alone won't prove it.
4. **Helpers are named by behaviour with no prefix**, if they're unique within main
   plus that overlay (`move_menu_title`). Add the prefix only on a collision.
5. **An overlay's copy of main or date-engine code takes the o.bin name.** Exact
   4-function size runs put `hidden_face_change0`..`4` in DATE, GYOZI, GEKO, EVENT
   and SHOUGATU, and `file_select_main44`..`49` in NAME_ENT. These are candidates:
   five consecutive 40-byte functions are a weak run on size alone, so read the
   code. In 1994 each overlay was linked separately, so duplicate names across
   overlays are period-correct. `config/symbol_names.json` keys overlay names by
   `(OV, address)`, and `pc/tools/generate_overlay_modules.py` (in toki1-pc) already prefixes
   overlay symbols per module (`TACO_func_8013201C`), so duplicates are safe in the
   PC port too once the generator reads names from the table.
6. **Overlay data**: menu tables `menu0`..`menuN` + `<ov>_menu` (the olh precedent);
   strings `<purpose>_string` / `<ov>_mes<N>`; animation sets `o_<ov>_*`; state
   `now_*` / `last_*` / `*_timer` / `*_flag` as in §3.

### 4.2 Main-binary symbols with no o.bin name

- **Statics** (never exported, so never in o.bin): name them by behaviour in the
  style of their TU's neighbours. The o.bin names bracketing the TU give the
  subsystem prefix. A static inside the `k_*` text system is `k_*`, one inside
  `dec_bg_*` is `dec_bg_*`.
- **Debug-build names retail doesn't have**: retail is a different source revision.
  64 of the 66 `file_select_*` names pair with nothing in the retail main-binary
  size alignment (`naming_census.py align --show`), and six of them
  (`file_select_main44`..`49`) match a NAME_ENT size run exactly, so some of that
  code moved into an overlay. Don't force an o.bin
  name onto a retail function because a slot is free.
- **Compiler-generated objects** (jump tables, anonymous string literals): write them
  back as `switch`/string literals in the C wherever the bytes allow. Where a named
  object is unavoidable, use `<function>_jtbl` / `<function>_str<N>`. These are the
  only sanctioned mechanical names, and they must never be used for a real global.

### 4.3 Struct types and fields

o.bin has no types, so every type and field name is T4.

- **Type names**: SCREAMING_SNAKE, following the Sony headers Konami compiled
  against (`GsSPRITE`, `DRAWENV`, `RECT`) and Konami's own uppercase constant tables:
  `GIRL_DATA`, `SAVE_WORK`, `SPRITE_REC`, `STEP_WORK`. This is a **project decision,
  not recovered evidence**. Say so in each typedef's header comment.
- **Object names come from o.bin whenever the object is anchored.** Example: debug
  `Sprites` has an extent of 0x900 = 64 records × 36 bytes. That's exactly retail's
  `D_80129CB0` sprite array, which `sprite_init(0, 64)` clears with a 36-byte stride
  and which is followed directly by the `bg_init` records at `D_8012A5B0`, as debug
  `Sprites` is followed by `Bg`.
- **Field names**: lower snake, 1–3 tokens, domain words in the §2 R9 vocabulary for
  simulation data (`tokimeki`, `yuukou`, `bukatu`, `kega`, `byouki`, `basyo`) and
  English for engine data (`x`, `y`, `w`, `h`, `tpage`, `clut`, `col`, `bri`,
  `timer`, `flag`, `kind`, `mode`, `step`).
- **An unknown field is `unk<HEX offset>`** (`unk1D9`), and padding is
  `pad<HEX offset>`. Never use `field_0x…`: that's a name that looks decided.
  `unk` is the honest equivalent of `func_` and is tracked as debt by the census.
- Arrays of records are plural or `*_tbl`/`*_table`, singular records are singular
  (`girl[i]`, `girl_tbl`).

### 4.4 Parameters and locals

- Short lower snake, C89: `i`, `j`, `n`, `p`, `girl`, `step`, `x`, `y`, `col`, `bri`,
  `sector`, `timer`.
- A parameter is named for what the caller passes (`girl`, `month`, `se_no`,
  `sector`), with the same vocabulary as the field it ends up in.
- Leave `arg0`/`sp10`-style names only where the purpose is genuinely unknown, and
  don't count them as named.
- ⚠️ Renaming a local or parameter is byte-neutral **only** when nothing else moves.
  Local *declaration order* sets IDO frame layout (`CLAUDE.md`), so rename in place.
  Never reorder declarations as part of a naming pass.

---

## 5. Checklist for a proposed name

1. Is it Sony code? Then only Sony's exact name (T1/T2), or no name yet.
2. Does o.bin name it, with non-positional corroboration? Then use o.bin's spelling
   exactly, typos included.
3. Otherwise: snake_case, subject first, role last (`_init`/`_main`/`_exit`/`_show`/
   `_disp`/`_disp_switch`/`_set`/`_clear`/`_check`/`_judge`/`get_`), ≤ ~24 characters.
4. Japanese for game-domain concepts, spelled as o.bin spells them; English for
   engine concepts.
5. Glued ordinals; hex for step indices; a leading underscore only for a worker
   behind a public twin.
6. Unique within its link unit (main, or main + that overlay).
7. `get_*` must return a value, `*_init` must initialise, `*_show`/`*_disp` must draw,
   and `*_check`/`*_judge` must return a condition. Verify each against the C: the
   2026-09-27 audit refuted `get_*` names on `void` functions and `*_disp` names on
   bodies with no draw calls.
