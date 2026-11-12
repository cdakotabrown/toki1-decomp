# `include/psyq/`: the two SDK headers this build still needs

This directory used to hold the 56 Psy-Q SDK headers. They are Sony's, they are
copyrighted, and only two of them were ever a build input, so the other 54 are
gone. What is left is project-written:

| Header | Why it exists |
|---|---|
| `libgte.h` | `MATRIX`, `VECTOR`, `SVECTOR` and the five GTE drivers (`RotMatrix`, `TransMatrix`, `ScaleMatrix`, `SetRotMatrix`, `SetTransMatrix`) used by `src/main/matched/GsSortBg.c`, `src/main/matched/GsSortSprite.c`, `src/main/pc-port/GsSortSprite.c` (in toki1-pc). |
| `stddef.h` | `size_t` / `NULL` / `offsetof`, standard C89, for sources compiled with `-I include/psyq`. |

Both declare only what the code uses. The layouts are the GTE register layout
and `make check-raw` / `make build-c` reproducing the retail SHA-1
(`2d8d4748...`) is what proves them right; if a layout were wrong the build would
not be byte-perfect.

If you need a Psy-Q header for research (the `.LIB` writer, symbol naming),
supply your own SDK copy: `tools/analysis/match_psyq_lib.py`, `naming_lint.py`
and `psyq_obj.py` take an optional path (`PSYQ_SDK_DIRS`) and skip cleanly
without one. Nothing in the build path reads an SDK archive.
