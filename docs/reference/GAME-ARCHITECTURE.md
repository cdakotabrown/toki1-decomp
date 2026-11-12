# Tokimeki Memorial: Game Architecture

**Last updated: 2026-05-11**

A plain-English explanation of how this PS1 game is structured, why there are
overlays, and how all the pieces fit together.

---

## The Big Picture

Think of Tokimeki Memorial like a building:

```
┌─────────────────────────────────────────────┐
│                GAME CONTENT                  │  ← EXN Overlays (26 files)
│  Dating scenes, events, holidays, exams,     │     5,352 functions
│  minigames, endings, phone calls, etc.       │     Loaded ONE AT A TIME
│  Only one overlay is active at any moment.   │     to address 0x80132000
├─────────────────────────────────────────────┤
│                GAME ENGINE                   │  ← Main Binary (SLPS_000.65)
│  Scene dispatch, rendering, sprites, text,   │     ~1,300 functions
│  sound system, input, save/load, 3D math     │     Always in memory
│  Always resident in memory.                  │     at 0x80041000
├─────────────────────────────────────────────┤
│              HARDWARE / OS                   │  ← PsyQ SDK + PS1 Hardware
│  GPU, SPU, CD-ROM, Memory Card, Controller   │     (PC port: SDL2 HAL)
└─────────────────────────────────────────────┘
```

**The engine** (bottom) is always loaded. It handles rendering, sound, input,
and the scene dispatch loop. This is the `SLPS_000.65` executable — 688KB,
~1,300 functions. We've decompiled 1,069 of these to byte-perfect C plus
142 byte-perfect ASM = 1,211 byte-perfect total.

**The content** (top) swaps in and out. The PS1 only has 2MB RAM, so the game
can't fit all its content at once. Instead, it loads "overlay" code modules
from the CD-ROM as needed. When you go on a date, `DATE.EXN` loads. When you
get a phone call, `TEL.EXN` loads. When you open options, `OPTION.EXN` loads.
Only ONE overlay is active at a time.

---

## How Scenes Work

The game runs an infinite loop that checks a **scene ID** (`D_800EF868`, a
single byte, 0-255) and calls the appropriate handler:

```
Main Loop (func_80042304):
  forever:
    read input
    switch (scene_id):
      case 0:  boot/opening movie
      case 16: initial data load
      case 18: title data read
      case 48: new game start
      case 50: TITLE SCREEN ← you see this
      case 52: title transition
      case 65: file select
      case 66: save/load
      ...
      case 3, 17-196: OVERLAY DISPATCH ← this is the gameplay
      ...
    render frame
    wait for VSync
```

Most cases (0, 16, 18, 48, 50, etc.) call functions directly from the engine.
But cases 3 and 17-196 call through a **function pointer** (`D_8012B5F0`) that
points to whatever overlay is currently loaded.

---

## The Overlay System

### How overlays load

1. Engine decides which scene to go to (e.g., "start a date")
2. `ending_main` reads the appropriate EXN file from CD-ROM
3. The EXN binary is copied to RAM at address `0x80132000`
4. The overlay's entry function is stored in `D_8012B5F0`
5. The scene dispatch loop calls `D_8012B5F0()` every frame

### What's inside an overlay

Each EXN file is exactly 192KB (196,608 bytes). It contains:
- **MIPS machine code** — compiled C functions, same compiler as the engine
- **Data** — strings, lookup tables, sprite coordinates
- **A 4-byte checksum** at the very end

The overlay code calls BACK into the engine constantly. For example, a dating
scene might call:
- `func_8004F690()` to render text (engine function)
- `func_80044568()` to blit sprites (engine function)
- `func_80042A28()` to change scenes (engine function)
- Its own internal functions for date-specific logic

### Dead disc file: `EXEDIR/O.BIN` (recorded 2026-10-04)

`CDROM/EXEDIR/` holds 27 files of exactly 196,608 bytes (96 sectors): the 26
`.EXN` overlays and `O.BIN`. **`O.BIN` is not an overlay and is never loaded.**
It is byte-identical to `disks/o.bin`, the debug-symbol source used for main
binary naming (`tools/analysis/extract_obin_symbols.py`).

Evidence (reproduce from the Track 1 `.bin`, the ISO 9660 directory and
`disks/SLPS_000.65`, which is byte-identical to the disc's `PSX.EXE`):

- **It is a MIPSEL ECOFF object, not a raw image.** Byte 0 is the ECOFF file
  header (magic `0x0162`, timestamp 1995-07-25, symbolic header at file offset
  0x5C0, magic `0x7009`), followed by an a.out header (magic `0x0107`,
  entry = text_start = `0x80132000`) and section headers: `.text`
  0x80132000/0x3A0 at file offset 0xF0, `.rdata` 0x801323A0/0x90, `.data`
  0x80132430/0xA0. That is 928 bytes of code linked at the overlay base.
  The rest is the debug symbol table (about 2,100 names of main-binary
  functions such as `game_main` and `step_up`). Overlays are read raw, as 96
  sectors to `0x80132000`. Reading `O.BIN` that way would put the ECOFF header
  where the first instruction goes and shift the 928-byte `.text` 0xF0 bytes
  away from the address it was linked for.
- **Its sector number is never referenced.** `O.BIN` sits at LBA 38474
  (`0x964A`), between `NAME_ENT.EXN` (38378) and `OLH.EXN` (38570). Overlays
  are read by `cd_read_start2(96, D_800BD23C = 0x80132000, lba)`, and that LBA
  is always an `ori a2,$zero,<lba>` immediate: one per scene group in
  `mascot_init` (0x80078CD8), and `TT.EXN` once more in `ending_main`. Across
  `SLPS_000.65` and all 26 EXNs, `0x964A` never occurs as an `ori`/`li`
  immediate, an aligned halfword or word, lba±150, a byte offset (lba×2048) or
  a BCD MSF triple. There is also no `O.BIN` / `EXEDIR` / `.EXN` filename
  string in the executable, so no `CdSearchFile` path to it exists either.

⚠️ **The second test is necessary, not sufficient, and it also flags two real
overlays.** Only 24 of the 26 EXN LBAs appear as loader immediates. `EVENT.EXN`
(37898, `0x940A`, 805 functions) and `GYOZI.EXN` (38090, `0x94CA`) are just as
absent from `SLPS_000.65` and every EXN by every encoding above. How they are
loaded has not been established: a computed LBA is the likeliest
explanation, but whether they are reachable at all is an open question. What
makes `O.BIN` dead is the ECOFF format. The missing reference only backs it
up. Do not use LBA absence on its own to declare an overlay dead.

`O.BIN` is not in the overlay universe (`config/overlay/*.sym`), and no
coverage or effective figure counts it.

### Why overlays matter for the decomp

The engine (the main executable) is the skeleton of the game. The overlays
(the 26 `.EXN` files) are the flesh: all the actual gameplay content.

| What | Where |
|------|-------|
| How text renders | Engine |
| How sprites draw | Engine |
| How sound plays | Engine |
| What happens on a date | DATE.EXN overlay |
| What happens at a festival | BUNKASAI.EXN |
| What phone calls say | TEL.EXN overlay |
| How the TACO minigame works | TACO.EXN overlay |
| What the endings show | ENDING.EXN overlay |

Every overlay function is byte-perfect C or assembly by origin; see
`README.md` for the current breakdown.

---

## Memory Layout

```
PS1 RAM (2MB):
0x80000000 ┬─────────────────────
           │  System/BIOS area
0x80041000 ├─────────────────────
           │  GAME ENGINE CODE    ← SLPS_000.65 .text section
           │  (functions)          ~1,300 functions
0x80086000 ├─────────────────────
           │  PsyQ SDK + Data     Library code + initialized globals
0x800E9000 ├─────────────────────
           │  Runtime data         BSS, heap, stack
0x80132000 ├─────────────────────
           │  OVERLAY CODE         ← EXN loaded here (192KB)
           │  (one overlay at      Only ONE at a time
           │   a time)
0x80162000 ├─────────────────────
           │  Overlay data +       Scene-specific buffers
           │  texture buffers
0x80200000 └─────────────────────  End of RAM
```

---

## PS1 Rendering Pipeline

The PS1 has a 1024×512 pixel VRAM (Video RAM) that's separate from main RAM.
All graphics go through VRAM:

```
Game Logic → Build Primitives → Ordering Table → GPU → VRAM → Display

1. Game code creates sprite/polygon primitives (SPRT, TILE, POLY_F3, etc.)
2. Primitives are inserted into an Ordering Table (depth-sorted linked list)
3. DrawOTag() walks the OT and sends primitives to the GPU
4. GPU rasterizes to VRAM (1024×512 16-bit framebuffer)
5. Display controller shows a 320×240 region of VRAM on screen
```

For the PC port, we emulate this with a software VRAM buffer + SDL2.

---

## Asset Pipeline

Game assets are stored on the CD-ROM in Konami's custom formats:

| Format | Contents | How loaded |
|--------|----------|------------|
| `.EXN` | Overlay code + data | CdRead → memcpy to 0x80132000 |
| `.NBN` | Sprite/texture data | CdRead → DMA to VRAM |
| `.TIM` | Font/texture images | CdRead → LoadImage to VRAM |
| `.VH/.VB` | Sound bank (instruments) | CdRead → SPU transfer |
| `.SEQ` | Music sequences (MIDI-like) | CdRead → sequencer |
| `.XA` | Voice acting (ADPCM audio) | CD streaming → SPU |
| `.STR` | FMV video (MDEC codec) | CD streaming → MDEC → VRAM |

---

## Compiler

Everything was compiled with **IDO 5.2** (SGI's IRIX C compiler), the same
compiler used for N64 games. It's a C89 compiler with quirky optimizations:
- Uses `t6`-`t9` as temporaries (unusual register allocation)
- "Cross-prologue scheduling" — hoists loads above the stack frame setup
- Never emits `j` (jump) instructions — uses `b` (branch) instead
- Aggressive CSE, LICM, and strength reduction

A small number of PsyQ SDK functions were compiled with **GCC 2.6.0** (the
Psy-Q toolchain's bundled compiler).

The overlays use the **same IDO 5.2 compiler** as the engine — same
techniques, same PostProc tools, same matching workflow.

---

## What "Decompilation" Means Here

For each function, we:
1. Read the MIPS machine code (binary → assembly)
2. Understand the logic (assembly → pseudocode)
3. Write equivalent C89 code
4. Compile with IDO 5.2
5. Compare the output byte-for-byte against the original
6. Tweak until it matches perfectly ("byte-matched")

**Byte-matched** = our C code compiles to the EXACT same binary as the
original. Every function in the repository is byte-matched or kept as
assembly; `README.md`, "What matching means here", says how.
