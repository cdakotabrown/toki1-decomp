#!/usr/bin/env python3
"""Apply Hook 38 (IDO52_FORCE_WEB_HOME) to a private uopt.c."""
import sys, pathlib
SD = pathlib.Path(__file__).resolve().parent
p = pathlib.Path(sys.argv[1]) if len(sys.argv) > 1 else SD.parent / "build" / "5.2" / "uopt.c"
s = p.read_text()
if "IDO52_FORCE_WEB_HOME" in s:
    print("already applied"); sys.exit(0)

# 1a. declarations, next to the Hook 37 declarations
a1 = "#define FORCE_WEB_NOSPLIT_MAX 24\n"
assert s.count(a1) == 1, "anchor 1a"
s = s.replace(a1, SD.joinpath("h38_decl.c").read_text() + "\n" + a1)

# 1b. functions, before ido52_apply_web_bind_bbs's comment block
anchor = "/*\n * ido52_apply_web_bind_bbs: After globalcolor()."
assert s.count(anchor) == 1, "anchor 1b"
s = s.replace(anchor, SD.joinpath("h38_func.c").read_text() + "\n" + anchor)

# 2. env parsing, inside ido52_init_hooks after the GC_COLOR_TRACE line
a2 = '    env = getenv("IDO52_GC_COLOR_TRACE");\n    if (env && *env) ido52_gccolor_trace = atoi(env) ? atoi(env) : 1;\n'
assert s.count(a2) == 1, "anchor 2"
s = s.replace(a2, a2 + '''
    /* IDO52_FORCE_WEB_HOME=BITPOS[:BB][=MASK][,...] (Hook 38) */
    env = getenv("IDO52_FORCE_WEB_HOME");
    if (env && *env) {
        char bufh[512];
        char *tokh;
        strncpy(bufh, env, sizeof(bufh) - 1);
        bufh[sizeof(bufh) - 1] = '\\0';
        tokh = strtok(bufh, ",");
        while (tokh && ido52_force_web_home_count < FORCE_WEB_HOME_MAX) {
            int _h = ido52_force_web_home_count;
            char *eq = strchr(tokh, '=');
            char *cn;
            ido52_force_web_home[_h].mask = 1;
            if (eq) { *eq = '\\0'; ido52_force_web_home[_h].mask = atoi(eq + 1); }
            cn = strchr(tokh, ':');
            ido52_force_web_home[_h].bb = -1;
            if (cn) { *cn = '\\0'; ido52_force_web_home[_h].bb = atoi(cn + 1); }
            ido52_force_web_home[_h].bitpos = atoi(tokh);
            ido52_force_web_home_count++;
            tokh = strtok(NULL, ",");
        }
        fprintf(stderr, "[WEB-HOME] %d force entries configured\\n",
                ido52_force_web_home_count);
    }
    /* IDO52_WEB_HOME_TRACE=1 (Hook 38) */
    env = getenv("IDO52_WEB_HOME_TRACE");
    if (env && *env) ido52_web_home_trace = atoi(env) ? atoi(env) : 1;
    /* IDO52_WEB_HOME_STAGE=1|2|3 (Hook 38); 1=pre-globalcolor 2=post 3=both */
    env = getenv("IDO52_WEB_HOME_STAGE");
    if (env && *env) ido52_web_home_stage = atoi(env) & 3;
    /* IDO52_FORCE_WEB_ADJSAVE=BITPOS:VALUE[,...] (Hook 38b) */
    env = getenv("IDO52_FORCE_WEB_ADJSAVE");
    if (env && *env) {
        char bufa[512];
        char *toka;
        strncpy(bufa, env, sizeof(bufa) - 1);
        bufa[sizeof(bufa) - 1] = '\\0';
        toka = strtok(bufa, ",");
        while (toka && ido52_force_web_adjsave_count < FORCE_WEB_ADJSAVE_MAX) {
            char *cn = strchr(toka, ':');
            if (cn) {
                int _a = ido52_force_web_adjsave_count;
                *cn = '\\0';
                ido52_force_web_adjsave[_a].bitpos = atoi(toka);
                ido52_force_web_adjsave[_a].value = (float)atof(cn + 1);
                ido52_force_web_adjsave_count++;
            }
            toka = strtok(NULL, ",");
        }
        fprintf(stderr, "[WEB-HOME] %d adjsave override(s) configured\\n",
                ido52_force_web_adjsave_count);
    }
''')

# 3. call site: after the PRECOLOR_WEB post-globalcolor hook
a3 = ("/* === IDO52_PRECOLOR_WEB post-globalcolor hook === */\n"
      "ido52_apply_precolor_web_post_gc(mem);\n")
assert s.count(a3) == 1, "anchor 3"
s = s.replace(a3, a3 + "/* === IDO52_FORCE_WEB_HOME hook (Hook 38), post-globalcolor === */\n"
                      "ido52_apply_force_web_home(mem, 2);\n")

# 3b. f_compute_save wrapper (Hook 38b) -- rename the definition first
a3c = "static void f_compute_save(uint8_t *mem, uint32_t sp, uint32_t a0) {\n"
assert s.count(a3c) == 1, "anchor 3b def"
s = s.replace(a3c, "static void f_compute_save_orig(uint8_t *mem, uint32_t sp, uint32_t a0) {\n")
a3b = "static void f_compute_save(uint8_t *mem, uint32_t sp, uint32_t a0);\n"
assert s.count(a3b) == 1, "anchor 3b decl"
s = s.replace(a3b, "static void f_compute_save_orig(uint8_t *mem, uint32_t sp, uint32_t a0);\n"
                   "static void ido52_force_adjsave(uint8_t *mem, uint32_t lr);\n"
                   "static void f_compute_save(uint8_t *mem, uint32_t sp, uint32_t a0) {\n"
                   "    f_compute_save_orig(mem, sp, a0);\n"
                   "    ido52_force_adjsave(mem, a0);\n"
                   "}\n")

# 4. pre-globalcolor call site
a4 = "f_globalcolor(mem, sp);\n"
assert s.count(a4) == 1, "anchor 4"
s = s.replace(a4, "/* === IDO52_FORCE_WEB_HOME hook (Hook 38), pre-globalcolor === */\n"
                  "ido52_apply_force_web_home(mem, 1);\n" + a4)
p.write_text(s)
print("Hook 38 applied to", p)
