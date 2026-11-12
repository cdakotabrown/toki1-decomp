#!/usr/bin/env python3
"""Apply Hook 39 (IDO52_FORCE_WEB_SPILL) to a private uopt.c."""
import sys, pathlib
SD = pathlib.Path(__file__).resolve().parent
p = pathlib.Path(sys.argv[1]) if len(sys.argv) > 1 else SD.parent / "build" / "5.2" / "uopt.c"
s = p.read_text()
if "IDO52_FORCE_WEB_SPILL" in s:
    print("already applied"); sys.exit(0)

a1 = "#define FORCE_WEB_NOSPLIT_MAX 24\n"
assert s.count(a1) == 1, "anchor 1"
s = s.replace(a1, SD.joinpath("h39_decl.c").read_text() + "\n" + a1)

anchor = "/*\n * ido52_apply_web_bind_bbs: After globalcolor()."
assert s.count(anchor) == 1, "anchor 2"
s = s.replace(anchor, SD.joinpath("h39_func.c").read_text() + "\n" + anchor)

a3 = '    env = getenv("IDO52_GC_COLOR_TRACE");\n    if (env && *env) ido52_gccolor_trace = atoi(env) ? atoi(env) : 1;\n'
assert s.count(a3) == 1, "anchor 3"
s = s.replace(a3, a3 + '''
    /* IDO52_FORCE_WEB_SPILL=BITPOS[,BITPOS...] (Hook 39) */
    env = getenv("IDO52_FORCE_WEB_SPILL");
    if (env && *env) {
        char bufs[256];
        char *toks;
        strncpy(bufs, env, sizeof(bufs) - 1);
        bufs[sizeof(bufs) - 1] = '\\0';
        toks = strtok(bufs, ",");
        while (toks && ido52_force_web_spill_count < FORCE_WEB_SPILL_MAX) {
            ido52_force_web_spill[ido52_force_web_spill_count++] = atoi(toks);
            toks = strtok(NULL, ",");
        }
        fprintf(stderr, "[WEB-SPILL] %d entries configured\\n",
                ido52_force_web_spill_count);
    }
    /* IDO52_FORCE_WEB_NOSPILL=BITPOS[,BITPOS...] (Hook 39) */
    env = getenv("IDO52_FORCE_WEB_NOSPILL");
    if (env && *env) {
        char bufns[256];
        char *tokns;
        strncpy(bufns, env, sizeof(bufns) - 1);
        bufns[sizeof(bufns) - 1] = '\\0';
        tokns = strtok(bufns, ",");
        while (tokns && ido52_force_web_nospill_count < FORCE_WEB_NOSPILL_MAX2) {
            ido52_force_web_nospill[ido52_force_web_nospill_count++] = atoi(tokns);
            tokns = strtok(NULL, ",");
        }
        fprintf(stderr, "[WEB-SPILL] %d nospill entries configured\\n",
                ido52_force_web_nospill_count);
    }
''')

a4 = "f_globalcolor(mem, sp);\n"
assert s.count(a4) == 1, "anchor 4"
s = s.replace(a4, "/* === IDO52_FORCE_WEB_SPILL hook (Hook 39) === */\n"
                  "ido52_apply_force_web_spill(mem);\n" + a4)
p.write_text(s)
print("Hook 39 applied to", p)
