#!/usr/bin/env python3
"""Apply Hook 40 (IDO52_VETO_WEB) to a private uopt.c."""
import sys, pathlib
SD = pathlib.Path(__file__).resolve().parent
p = pathlib.Path(sys.argv[1]) if len(sys.argv) > 1 else SD.parent / "build" / "5.2" / "uopt.c"
s = p.read_text()
if "IDO52_VETO_WEB" in s:
    print("already applied"); sys.exit(0)

a1 = "#define FORCE_WEB_NOSPLIT_MAX 24\n"
assert s.count(a1) == 1, "anchor 1"
s = s.replace(a1, SD.joinpath("h40_decl.c").read_text() + "\n" + a1)

anchor = "/*\n * ido52_apply_web_bind_bbs: After globalcolor()."
assert s.count(anchor) == 1, "anchor 2"
s = s.replace(anchor, SD.joinpath("h40_func.c").read_text() + "\n" + anchor)

a3 = '    env = getenv("IDO52_GC_COLOR_TRACE");\n    if (env && *env) ido52_gccolor_trace = atoi(env) ? atoi(env) : 1;\n'
assert s.count(a3) == 1, "anchor 3"
s = s.replace(a3, a3 + '''
    /* IDO52_VETO_WEB=BITPOS[,BITPOS...] (Hook 40) */
    env = getenv("IDO52_VETO_WEB");
    if (env && *env) {
        char bufv[256];
        char *tokv;
        strncpy(bufv, env, sizeof(bufv) - 1);
        bufv[sizeof(bufv) - 1] = '\\0';
        tokv = strtok(bufv, ",");
        while (tokv && ido52_veto_web_count < VETO_WEB_MAX) {
            ido52_veto_web[ido52_veto_web_count++] = atoi(tokv);
            tokv = strtok(NULL, ",");
        }
        fprintf(stderr, "[VETO-WEB] %d entries configured\\n",
                ido52_veto_web_count);
    }
    /* IDO52_VETO_WEB_DROPLR=1 (Hook 40) -- diagnostic only, aborts uopt */
    env = getenv("IDO52_VETO_WEB_DROPLR");
    if (env && *env) ido52_veto_web_droplr = atoi(env);
    /* IDO52_VETO_WEB_STAGE=1|2|3 (Hook 40) */
    env = getenv("IDO52_VETO_WEB_STAGE");
    if (env && *env) ido52_veto_web_stage = atoi(env) & 3;
    /* IDO52_VETO_FORMLIV=OCC[:SITE][,...] (Hook 40b) */
    env = getenv("IDO52_VETO_FORMLIV");
    if (env && *env) {
        char buff2[256];
        char *tokf2;
        strncpy(buff2, env, sizeof(buff2) - 1);
        buff2[sizeof(buff2) - 1] = '\\0';
        tokf2 = strtok(buff2, ",");
        while (tokf2 && ido52_veto_formliv_count < VETO_FORMLIV_MAX) {
            char *cn2 = strchr(tokf2, ':');
            int _v = ido52_veto_formliv_count;
            ido52_veto_formliv[_v].site = -1;
            if (cn2) { *cn2 = '\\0'; ido52_veto_formliv[_v].site = atoi(cn2 + 1); }
            ido52_veto_formliv[_v].occ = atoi(tokf2);
            ido52_veto_formliv_count++;
            tokf2 = strtok(NULL, ",");
        }
        fprintf(stderr, "[FORMLIV] %d veto entries configured\\n",
                ido52_veto_formliv_count);
    }
    /* IDO52_FORMLIV_TRACE=1 (Hook 40b) */
    env = getenv("IDO52_FORMLIV_TRACE");
    if (env && *env) ido52_formliv_trace = atoi(env);
''')

a4 = "/* === IDO52_PRECOLOR_WEB hook === */\nido52_apply_precolor_web(mem, sp);\n"
assert s.count(a4) == 1, "anchor 4"
s = s.replace(a4, a4 + "/* === IDO52_VETO_WEB hook (Hook 40), post-makelivranges === */\n"
                      "ido52_apply_veto_web(mem, sp, 1);\n")

# 4b. f_formlivbb wrapper (Hook 40b): rename definition, then declaration
a4b = "static void f_formlivbb(uint8_t *mem, uint32_t sp, uint32_t a0, uint32_t a1, uint32_t a2) {\n"
assert s.count(a4b) == 1, "anchor 4b def"
s = s.replace(a4b, "static void f_formlivbb_orig(uint8_t *mem, uint32_t sp, uint32_t a0, uint32_t a1, uint32_t a2) {\n")
a4c = "static void f_formlivbb(uint8_t *mem, uint32_t sp, uint32_t a0, uint32_t a1, uint32_t a2);\n"
assert s.count(a4c) == 1, "anchor 4b decl"
s = s.replace(a4c, a4c + "static void ido52_formliv_reset(void);\n")
# per-proc occurrence reset: f_makelivranges entry
a4d = "f_makelivranges(mem, sp);\n"
assert s.count(a4d) == 1, "anchor 4b reset"
s = s.replace(a4d, "ido52_formliv_reset();\n" + a4d)

a5 = "f_globalcolor(mem, sp);\n"
assert s.count(a5) == 1, "anchor 5"
s = s.replace(a5, "/* === IDO52_VETO_WEB hook (Hook 40), pre-globalcolor === */\n"
                  "ido52_apply_veto_web(mem, sp, 2);\n" + a5)
p.write_text(s)
print("Hook 40 applied to", p)
