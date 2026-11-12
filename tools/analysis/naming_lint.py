#!/usr/bin/env python3
"""naming_lint.py -- check proposed symbol names against the 1994 Konami
house style (docs/reference/NAMING-CONVENTIONS-1994.md) and against every
name already in the tree.

READ-ONLY.  Prints one line per finding:

    ERROR  the name must not land as proposed
    WARN   out of style or risky; land only with a reason in the evidence
    INFO   what the reviewer has to check (tier of evidence it needs)

Exit status 1 if any ERROR (so a proposer batch can be gated on it).

Checks (rule numbers are the style guide's):
  syntax     identifier charset; length (R-len: > 24 WARN, > 36 ERROR);
             placeholder prefixes (func_/D_/FUN_/DAT_/sub_/field_0x) ERROR
  R1 casing  Konami functions/data are lower snake_case; the islands
             SD_CamelCase (sound driver), Title_Snake (date engine) and
             acronyms (CD/XA/CDQ...) are INFO; other CamelCase/lowerCamel WARN;
             SCREAMING_SNAKE is data/type-only
  R2 Sony    a Sony name (o.bin Sony band + every Psy-Q archive symbol on disk: 2.5/3.3/3.5/3.6/4.0/4.5 + the psx_psyq_signatures label sets)
             must be verbatim; a Sony-looking name that no archive exports
             is ERROR; a Sony name outside the library band is WARN; any
             library-band address gets the T1/T2-only INFO
  R3 order   leading verb other than get_/set_/load_/goto_/change_/check_/
             show_ (and the rarer o.bin ones) WARN; a role word first
             (init_x, show_x) WARN with the subject-first spelling
  R4/R5      non-1994 role words (_update, _draw, _render, _initialize,
             _handler...) WARN with the house word
  R7         underscore ordinal (_1) INFO -- glued (init1) is the norm
  R8         leading underscore needs a public twin (WARN otherwise)
  R9         romanization: a concept o.bin spells differently (basho ->
             basyo) or in Japanese (rumor -> uwasa) INFO
  R10        g_ prefix: g_ means GIRL here; g_PascalCase is ERROR
  R6/§5.7    with --addr, `get_*` on a function whose C returns void ERROR
  collision  same name at another address in main, or in main + the same
             overlay: ERROR; in another overlay: INFO (per-overlay links);
             in pc-port / pc sources: WARN; an o.bin name: INFO naming the
             tier it needs, and what align_candidates.py says about it

Usage:
    python3 tools/analysis/naming_lint.py kega_hassei --addr 0x8006F1A4
    python3 tools/analysis/naming_lint.py GsSortSprite --addr 0x8009B000
    python3 tools/analysis/naming_lint.py now_girl --kind data --addr 0x800EF6BF
    python3 tools/analysis/naming_lint.py taco_main --ov TACO --addr 0x8013201C
    python3 tools/analysis/naming_lint.py --batch proposals.txt
        # lines: ADDR NAME [func|data|field|type] [OV]   ('#' comments)
        # or a JSON list of {"addr", "name", "kind", "ov"}
"""
import argparse
import collections
import glob
import json
import os
import re
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

GAME_LO, LIB_LO, LIB_HI, OVL = 0x80041000, 0x80086000, 0x800BB3C0, 0x80132000
ADDR_NAME = re.compile(r"^(?:func|FUN|D|DAT|sub|jtbl|L)_[0-9A-Fa-f]{6,8}$")
IDENT = re.compile(r"^[A-Za-z_][A-Za-z0-9_]*$")
SNAKE = re.compile(r"^_?[a-z][a-z0-9]*(_[a-z0-9]+)*$")
SCREAM = re.compile(r"^_?[A-Z][A-Z0-9]*(_[A-Z0-9]+)*$")
SD_CAMEL = re.compile(r"^SD_[A-Z][A-Za-z0-9]*$")
TITLE_SNAKE = re.compile(r"^[A-Z][a-z0-9]+(_[A-Z][a-z0-9]*)+$")
ACRONYMS = ("CD", "XA", "CDQ", "SE", "BGM", "VRAM", "GPU", "SPU", "OT", "RGB")
SONY_PREFIX = re.compile(
    r"^(_?(Gs|Ss|Spu|Snd|Cd|St|DecDCT|Gpu|Pad|Mem|Card|Mouse|Rot|Trans|"
    r"Set|Get|Load|Store|Move|Draw|Clear|Put|Open|Close|Read|Write|Init|"
    r"Reset|Enable|Disable|Start|Stop|Enter|Exit|Change|Test|Deliver|Undeliver|"
    r"Apply|Mul|Compare|Push|Pop|Vsync|VSync|Kanji|Font|Krom|FntOpen|FntPrint|"
    r"Add|Cat|Term|Is|Next|Func|Flush|Wait|Rcnt|Scale)[A-Z0-9_]|"
    r"_(svm|spu|snd|ss|card|bu|mk|96|cd|gs)_)")
# leading verbs o.bin Konami code uses (R3); anything else verb-like WARNs
VERBS = {"init", "initialize", "update", "draw", "do", "handle", "process",
         "calc", "compute", "run", "exec", "execute", "render", "create",
         "build", "add", "remove", "apply", "fill", "copy", "write", "open",
         "close", "reset", "start", "stop", "play", "wait", "load", "set",
         "get", "goto", "change", "check", "show", "make", "print", "search",
         "select", "swap", "move", "put", "clear", "read", "return", "is",
         "has", "can", "find", "parse", "decode", "encode", "fetch", "send",
         "trigger", "fire", "spawn", "tick", "step", "advance", "toggle",
         "enable", "disable", "setup", "prepare"}
ROLE_FIRST = {"init": "_init", "main": "_main", "exit": "_exit",
              "show": "_show", "disp": "_disp", "judge": "_judge"}
ROLE_WORDS = {"update": "the bare stem (per-frame body) or _main",
              "tick": "the bare stem (per-frame body)",
              "draw": "_show / _disp", "render": "_show / _disp",
              "display": "_disp", "initialize": "_init", "setup": "_init",
              "init_all": "_init", "finalize": "_exit", "cleanup": "_exit",
              "term": "_exit", "terminate": "_exit", "handler": "a role word "
              "(_main, _check, _set)", "func": "a role word", "proc": "a role"
              " word", "routine": "a role word", "manager": "a role word",
              "is": "_check / _judge", "flag": None}
ROMAJI = {"basho": "basyo", "yajirusi": "yazirushi", "jyouhou": "joho",
          "jouhou": "joho", "shogatsu": "syogatu", "shougatsu": "syogatu",
          "bukatsu": "bukatu", "serifu": None, "tokubetsu": "tokubetu",
          "shoushin": "syoushin", "yuukou": None, "yukou": "yuukou",
          "touzyou": None, "toujou": "touzyou", "tojo": "touzyou",
          "jotai": "zyotai", "joutai": "zyotai", "suji": "suuzi",
          "suuji": "suuzi", "gakkou": "gakko", "aisatsu": "aisatu",
          "kaisuu": "kaisu", "byoki": "byouki"}
ENGLISH_FOR_JP = {"rumor": "uwasa", "rumour": "uwasa", "injury": "kega",
                  "illness": "byouki", "numeral": "suuzi", "digit": "suuzi",
                  "friendship": "yuukou", "debut": "touzyou",
                  "greeting": "aisatu", "invitation": "sasoi",
                  "vacation": "kyuuka", "evaluation": "hyouka",
                  "cheer": "ouen", "dialogue": "serifu", "place": "basyo",
                  "location": "basyo", "newyear": "syogatu",
                  "club_activity": "bukatu", "schooltrip": "shugaku"}


# ----------------------------------------------------------------- indexes
class Index(object):
    def __init__(self):
        self.main = collections.defaultdict(set)   # name -> {addr}
        self.landed_at = {}     # landed name -> its symbol_names.json address
        self.ovl_landed = {}    # (overlay, landed name) -> its prologue address
        self.ovl = collections.defaultdict(lambda: collections.defaultdict(set))
        self.pc = set()
        self.obin = {}          # name -> (band, debug addr)
        self.sony = set()
        self.obin_first = collections.Counter()
        self.align = {}         # name -> [(retail, tier)]
        self.align_at = {}      # retail -> (candidate, tier)
        self.src_of = {}        # addr -> C path
        self._load()

    def _add_main(self, name, addr):
        if name and not ADDR_NAME.match(name):
            self.main[name].add(addr)

    def _load(self):
        # main: audit (current file names + aliases), symbol table, retail syms
        # audit aliases that only a PC-port override carries (its file name +
        # `Address:` header, no PS1 source of that name): PC-port-only
        pc_only = set()
        ps1 = set(os.path.splitext(os.path.basename(p))[0] for p in
                  glob.glob(os.path.join(ROOT, "src/main/matched/*.c")) +
                  glob.glob(os.path.join(ROOT, "src/main/semi-matched/*.c")))
        registry = json.load(open(os.path.join(ROOT, "config/level3_functions.json")))
        registered = {(f["name"], int(f["address"], 16))
                      for f in registry["functions"]}
        for p in glob.glob(os.path.join(ROOT, "src/main/pc-port/*.c")):
            n = os.path.splitext(os.path.basename(p))[0]
            if n in ps1:
                continue
            try:
                head = open(p, errors="replace").read(2000)
            except OSError:
                continue
            for m in re.finditer(r"Address:\s*0x([0-9a-fA-F]+)", head):
                pc_only.add((n, int(m.group(1), 16)))
        try:
            a = json.load(open(os.path.join(
                ROOT, "config/main_function_audit_by_addr.json")))["addresses"]
            for k, v in a.items():
                ad = int(k, 16)
                src = v.get("source")
                if src:
                    self._add_main(os.path.splitext(os.path.basename(src))[0],
                                   ad)
                    if src.endswith(".c"):
                        self.src_of[ad] = src
                for al in v.get("aliases") or ():
                    if (al, ad) in pc_only:
                        # The alias belongs to a PC-port override, not retail.
                        continue
                    source_name = os.path.splitext(os.path.basename(src or ""))[0]
                    if al != source_name and (al, ad) not in registered:
                        # Audit aliases include names reverted after a bad
                        # proximity guess. A new function at a different
                        # address may now legitimately own the same name.
                        continue
                    self._add_main(al, ad)
        except (OSError, ValueError):
            pass
        try:
            sy = json.load(open(os.path.join(ROOT, "config/symbol_names.json")))
            for k, v in sy["symbols"].items():
                if ":" in k:
                    # overlay-scoped record ("OV:0xADDR"): not a main name.  A landed
                    # one remembers its PROLOGUE address (the .sym window may start
                    # earlier over an absorbed head) so a re-lint is not a collision.
                    if v.get("landed"):
                        scope, _, a = k.partition(":")
                        self.ovl_landed[(scope, v["name"])] = int(a, 16)
                    continue
                if v.get("landed"):
                    self._add_main(v["name"], int(k, 16))
                    self.landed_at[v["name"]] = int(k, 16)
        except (OSError, ValueError, KeyError):
            pass
        try:
            rs = json.load(open(os.path.join(
                ROOT, "config/main_retail_symbols.json")))
            for n, v in rs.items():
                ad = v if isinstance(v, str) else (v.get("address") if
                                                   isinstance(v, dict) else None)
                try:
                    self._add_main(n, int(ad, 16) if isinstance(ad, str)
                                   else int(ad))
                except (TypeError, ValueError):
                    self._add_main(n, None)
        except (OSError, ValueError):
            pass
        for p in glob.glob(os.path.join(ROOT, "src/main/matched/*.c")) + \
                glob.glob(os.path.join(ROOT, "src/main/semi-matched/*.c")):
            n = os.path.splitext(os.path.basename(p))[0]
            if n not in self.main:
                self._add_main(n, None)
        # overlays
        for p in glob.glob(os.path.join(ROOT, "config/overlay/*.sym")):
            ov = os.path.basename(p)[:-4]
            for line in open(p, errors="replace"):
                parts = line.strip().split(":")
                if len(parts) >= 3 and not line.startswith("#"):
                    try:
                        ad = int(parts[0], 16)
                    except ValueError:
                        continue
                    n = parts[2].strip()
                    if not ADDR_NAME.match(n):
                        self.ovl[ov][n].add(ad)
        # pc port: basenames + file-scope function definitions
        fdef = re.compile(r"^[A-Za-z_][\w \t\*]*?\b([A-Za-z_]\w*)\s*\([^;]*$")
        for pat in ("src/main/pc-port/*.c", "pc/src/*.c", "pc/hal/*.c",
                    "pc/src/*/*.c"):
            for p in glob.glob(os.path.join(ROOT, pat)):
                self.pc.add(os.path.splitext(os.path.basename(p))[0])
                try:
                    for line in open(p, errors="replace"):
                        m = fdef.match(line)
                        if m and m.group(1) not in ("if", "while", "for",
                                                    "switch", "return"):
                            self.pc.add(m.group(1))
                except OSError:
                    pass
        # o.bin -- banded by POSITION in the debug link (the same slices
        # naming_census.py `align` uses: 0:697 K-game, 697:881 K-sd/K-date,
        # 881: Sony), so a rename sweep over the tree cannot break the lookup
        try:
            import naming_census as NC
            funcs, data, _ = NC.load_obin()
            for i, s in enumerate(funcs):
                b = "K-game" if i < 697 else ("K-sd/K-date" if i < 881
                                               else "Sony")
                self.obin[s["name"]] = (b, s["address"])
                if b == "Sony":
                    self.sony.add(s["name"])
                elif SNAKE.match(s["name"]):
                    self.obin_first[s["name"].lstrip("_").split("_")[0]] += 1
            pre = getattr(NC, "SONY_DATA_PREFIX", ())
            exact = getattr(NC, "SONY_DATA_EXACT", set())
            for s in data:
                n = s["name"]
                sony = n in exact or n.startswith(tuple(pre))
                self.obin[n] = ("Sony" if sony else "K-data", s["address"])
                if sony:
                    self.sony.add(n)
        except (SystemExit, Exception):
            pass
        # Psy-Q archives
        try:
            import psyq_obj
            # every SDK archive the user supplies (2026-09-27 Stage 1: retail's
            # libraries are 3.0-3.3 generation code, whose names -- public,
            # static and bss -- are not all in 2.5). Sony's archives are NOT in
            # the tree since the 2026-11 cleanup: PSYQ_SDK_DIRS points at your
            # own copy, and the public sozud/psy-q-decomp checkout is used when
            # present. Library symbol tables plus each member's own exports,
            # function and local (static) symbols.
            home = os.path.expanduser("~/psy-q-decomp/psy-q")
            libdirs = [d for d in os.environ.get("PSYQ_SDK_DIRS", "").split(os.pathsep)
                       if d] + \
                [os.path.join(home, v, "PSX/LIB") for v in ("3.3", "3.5", "3.6", "4.0")]
            for lp in psyq_obj.iter_libs(libdirs):
                try:
                    for m in psyq_obj.parse_lib(lp):
                        self.sony.update(m.symbols)
                        for e in (list(m.obj.exports) + list(m.obj.functions)
                                  + list(m.obj.locals)):
                            self.sony.add(e["name"])
                except Exception:
                    continue
        except Exception:
            pass
        # the psx_psyq_signatures label sets (SDK 2.6/3.0/3.4/... object
        # layouts, incl. statics); auto-generated loc_/text_ labels skipped
        try:
            import glob as _glob
            sigdir = os.path.expanduser("~/psy-q-decomp/psx_psyq_signatures")
            for fn in _glob.glob(os.path.join(sigdir, "*", "*.json")):
                try:
                    for o in json.load(open(fn)):
                        for lab in o.get("labels") or []:
                            n = lab["name"] if isinstance(lab, dict) else lab
                            if not re.match(r"^(loc|text|data|bss|rdata|sdata|sbss)_[0-9A-F]+$", n):
                                self.sony.add(n)
                        for n in o.get("xbss") or []:
                            self.sony.add(n["name"] if isinstance(n, dict) else n)
                except Exception:
                    continue
        except Exception:
            pass
        # alignment candidates, if generated
        p = os.path.join(ROOT, "build/analysis/align_candidates.json")
        try:
            J = json.load(open(p))
            for b in J["bands"].values():
                for r in b["rows"]:
                    c = r.get("candidate")
                    if c and not r["tier"].endswith("_tail"):
                        ad = int(r["retail"], 16)
                        self.align.setdefault(c, []).append((ad, r["tier"]))
                        self.align_at[ad] = (c, r["tier"])
        except (OSError, ValueError, KeyError):
            pass


# ------------------------------------------------------------------ checks
def tokens(name):
    return [t for t in name.lstrip("_").split("_") if t]


def band_of(addr, ov):
    if ov:
        return "overlay"
    if addr is None:
        return None
    if GAME_LO <= addr < LIB_LO:
        return "game"
    if LIB_LO <= addr < LIB_HI:
        return "lib"
    if LIB_HI <= addr < OVL:
        return "data"
    return "other"


def returns_void(path, addr):
    """Best effort: the return type of the function defined in `path`."""
    try:
        t = open(path, errors="replace").read()
    except OSError:
        return None
    t = re.sub(r"/\*.*?\*/", "", t, flags=re.S)
    defs = re.findall(r"^([A-Za-z_][\w \t\*]*?)\b([A-Za-z_]\w*)\s*\(([^;{]*)\)"
                      r"\s*(?:\n[^{;]*)?\{", t, flags=re.M)
    want = os.path.splitext(os.path.basename(path))[0]

    def is_void(ret, params):
        if ret.split()[-1:] != ["void"] or "*" in ret:
            return False
        # a void function that writes its result through a pointer
        # parameter (`get_h_tokimeki_table(s8 *dest)` fills dest[0..10])
        # still GETS something: not a contradiction (2026-09-27, batch 2)
        for pn in re.findall(r"\*\s*([A-Za-z_]\w*)\s*(?:,|$|\[)", params):
            if re.search(r"(?:\b%s\s*\[[^\]]*\]|\*\s*\(?\s*%s\b[^;=]*?)"
                         r"\s*=(?!=)" % (pn, pn), t):
                return False
        return True
    for ret, nm, params in defs:
        if nm == want or nm.lower().endswith("%08x" % addr):
            return is_void(ret, params)
    if len(defs) == 1:
        return is_void(defs[0][0], defs[0][2])
    return None


def lint(name, addr=None, kind="func", ov=None, idx=None, batch_names=()):
    out = []

    def f(level, rule, msg):
        out.append((level, rule, msg))
    band = band_of(addr, ov)
    toks = tokens(name)
    # syntax
    if not IDENT.match(name):
        f("ERROR", "syntax", "not a C identifier")
        return out
    # `sub_` is a placeholder only with an address (IDA's sub_8004ABCD): the
    # o.bin names sub_step_up / sub_sub_step_up are real (batch 1, 2026-09-27)
    if ADDR_NAME.match(name) or name.startswith(("func_", "D_", "FUN_",
                                                 "DAT_")) or \
            re.match(r"^sub_[0-9A-Fa-f]{6,8}$", name):
        f("ERROR", "syntax", "placeholder/address name -- not a name")
        return out
    if re.search(r"field_0x|^unk_?[0-9A-Fa-f]*$", name) and kind != "field":
        f("ERROR", "syntax", "unk/field_0x is for struct fields only (s4.3)")
    if "field_0x" in name:
        f("ERROR", "s4.3", "field_0x looks decided; use unk<HEX>")
    L = len(name)
    if L > 36:
        f("ERROR", "length", "%d chars (o.bin max 36)" % L)
    elif L > 24:
        f("WARN", "length", "%d chars; house median is 14 (functions) / 10 "
          "(data), > ~24 is out of style" % L)
    if len(toks) > 6:
        f("WARN", "length", "%d tokens (o.bin max 6)" % len(toks))
    # Sony
    is_sony = idx and name in idx.sony
    looks_sony = bool(SONY_PREFIX.match(name)) and not SNAKE.match(name)
    if is_sony:
        if band in ("game", "overlay"):
            f("WARN", "R2", "Sony name at a %s address: Sony code lives in the "
              "library band (0x80086000-0x800BB3C0)" % band)
        else:
            f("INFO", "R2", "Sony name: lands only on T1 (whole-member archive "
              "identity, match_psyq_lib.py) or T2 (source shape) evidence, "
              "never o.bin position")
    elif looks_sony and kind in ("func", "data"):
        f("ERROR", "R2", "Sony-style name that no Psy-Q archive on disk and no "
          "o.bin Sony symbol exports -- a Sony name is never invented")
    if band == "lib" and not is_sony and kind == "func":
        f("INFO", "R2", "library-band address with a non-Sony name: allowed "
          "only once no Sony member matches (by elimination Konami code: "
          "SD driver, date engine, holiday_*); Konami style applies")
    # casing
    obin = idx.obin.get(name) if idx else None
    if not is_sony and not obin:
        if kind == "type":
            if not SCREAM.match(name):
                f("WARN", "s4.3", "type names are SCREAMING_SNAKE "
                  "(GIRL_DATA, SAVE_WORK)")
        elif SNAKE.match(name):
            pass
        elif SD_CAMEL.match(name):
            f("INFO", "R1", "SD_CamelCase island: only for the Konami sound "
              "driver (SD_*, gSD* data)")
        elif TITLE_SNAKE.match(name):
            f("INFO", "R1", "Title_Snake island: only for code demonstrably "
              "in the date-engine TU (Graph_Init, Fade_Out_Main)")
        elif SCREAM.match(name):
            if kind == "func":
                f("WARN", "R1", "SCREAMING_SNAKE is for data constants and "
                  "types, not functions")
        elif all(SNAKE.match(t.lower()) and (t.islower() or t.isdigit()
                                             or t in ACRONYMS)
                 for t in toks):
            f("INFO", "R1", "acronym capitals inside snake_case "
              "(get_CD_State): keep to real acronyms")
        else:
            f("WARN", "R1", "not lower snake_case; CamelCase/lowerCamel is "
              "not the Konami style (86% snake)")
    # g_
    if name.startswith("g_"):
        if re.match(r"^g_[A-Z]", name):
            f("ERROR", "R10", "g_PascalCase: g_ means GIRL in this codebase "
              "(get_g_name); no g_ global prefix in o.bin")
        else:
            f("INFO", "R10", "g_ means GIRL (get_g_name, get_g_zyotai_s); "
              "make sure that is meant")
    # subject first
    if toks and SNAKE.match(name) and kind == "func":
        t0 = toks[0]
        ok_verbs = {"get", "set", "load", "goto", "change", "check", "show"}
        if t0 in ROLE_FIRST and len(toks) > 1:
            f("WARN", "R3", "role word first: try %s%s" % (
                "_".join(toks[1:]), ROLE_FIRST[t0]))
        elif t0 in VERBS and t0 not in ok_verbs:
            n = idx.obin_first.get(t0, 0) if idx else 0
            if n >= 2:
                f("INFO", "R3", "leading verb '%s' is rare (o.bin: %d names); "
                  "house style is subject first" % (t0, n))
            else:
                f("WARN", "R3", "leading verb '%s' (o.bin: %d): subject first, "
                  "role last (hizuke_show, not show_hizuke); lead only with "
                  "get_/set_/load_/goto_" % (t0, n))
    # role vocabulary
    for t in toks:
        if t in ROLE_WORDS and ROLE_WORDS[t]:
            if t == "is" and toks[0] != "is":
                continue
            f("WARN", "R4/R5", "'%s' is not 1994 vocabulary: use %s" % (
                t, ROLE_WORDS[t]))
    # ordinals
    if re.search(r"_\d+$", name):
        f("INFO", "R7", "underscore ordinal is rare (6 in o.bin); glued is "
          "the norm (init0, main4a for hex steps)")
    # leading underscore
    if name.startswith("_") and kind == "func" and not is_sony:
        twin = name.lstrip("_")
        known = (idx and (twin in idx.main or twin in idx.obin or
                          any(twin in d for d in idx.ovl.values()))) or \
            twin in batch_names
        if not known:
            f("WARN", "R8", "leading underscore needs a public twin '%s' "
              "(the worker behind it)" % twin)
    # romanization / Japanese vocabulary
    for t in toks:
        tl = t.lower()
        if tl in ROMAJI and ROMAJI[tl]:
            f("INFO", "R9", "o.bin spells this concept '%s' -- copy it" %
              ROMAJI[tl])
        if tl in ENGLISH_FOR_JP:
            f("INFO", "R9", "game-domain concept: o.bin says '%s'" %
              ENGLISH_FOR_JP[tl])
    # get_ must return a value
    if (kind == "func" and toks and toks[0] == "get" and addr is not None
            and idx and addr in idx.src_of):
        rv = returns_void(idx.src_of[addr], addr)
        if rv:
            f("ERROR", "s5.7", "get_* on a function whose C returns void (%s)"
              % os.path.relpath(idx.src_of[addr], ROOT))
    # collisions
    if idx:
        m = idx.main.get(name, set())
        other = sorted(a for a in m if a is not None and a != addr)
        if other and not ov and idx.landed_at.get(name) == addr:
            # re-linting a LANDED name: the audit also lists it at the
            # function's prologue key (absorbed head), at a same-TU layout
            # copy (`main` inside _main.c) or at a PC-port override's
            # `Address:` header.  Uniqueness of landed names is enforced by
            # `symbol_names.py check`; here it is only a note.
            f("INFO", "collision", "landed here; the audit also lists it at "
              "0x%08X (prologue key, same-TU copy or PC-port header)"
              % other[0])
        elif other and not ov:
            f("ERROR", "collision", "already names main 0x%08X" % other[0])
        elif other and ov:
            f("ERROR", "collision", "main already has it (0x%08X); an overlay "
              "links against main" % other[0])
        elif None in m and addr is None:
            f("WARN", "collision", "a main source file already has this name")
        if ov:
            o = idx.ovl.get(ov, {}).get(name, set())
            if idx.ovl_landed.get((ov, name)) == addr:
                f("INFO", "collision", "landed here as an overlay name (%s.sym window may start "
                  "earlier over an absorbed head)" % ov)
            elif any(a != addr for a in o):
                f("ERROR", "collision", "already names %s 0x%08X" % (
                    ov, sorted(a for a in o if a != addr)[0]))
        elsewhere = sorted(o2 for o2, d in idx.ovl.items()
                           if name in d and o2 != ov)
        if elsewhere:
            f("INFO", "collision", "also named in overlay(s) %s -- per-overlay "
              "links make that period-correct (s4.1.5) if it is the same code"
              % ",".join(elsewhere))
        if name in idx.pc and not (m and addr in m):
            f("WARN", "collision", "pc-port / pc sources define '%s': the PC "
              "link would collide unless the override IS this function" % name)
        if obin and not is_sony:
            b, dad = obin
            msg = "o.bin %s name (debug %s): a T3 claim -- needs size/order " \
                  "evidence AND a code-reading review" % (b, dad)
            al = idx.align.get(name)
            if al and addr is not None:
                here = [t for a, t in al if a == addr]
                if here:
                    msg += "; align_candidates agrees here (%s)" % here[0]
                else:
                    msg += "; align_candidates places it at %s" % ", ".join(
                        "0x%08X (%s)" % x for x in al)
                    f("WARN", "collision", msg)
                    msg = None
            if msg:
                f("INFO", "collision", msg)
        elif addr is not None and addr in idx.align_at and kind == "func":
            c, t = idx.align_at[addr]
            if t.startswith(("A_", "B_", "C_run")) and c != name:
                f("WARN", "align", "align_candidates proposes o.bin '%s' here "
                  "(%s): check it before inventing a T4 name" % (c, t))
    return out


def parse_batch(path):
    txt = open(path).read()
    if txt.lstrip().startswith("["):
        rows = []
        for r in json.loads(txt):
            a = r.get("addr")
            rows.append((int(a, 16) if isinstance(a, str) else a, r["name"],
                         r.get("kind", "func"), r.get("ov")))
        return rows
    rows = []
    for line in txt.splitlines():
        line = line.split("#")[0].strip()
        if not line:
            continue
        p = line.split()
        a = int(p[0], 16) if p[0].lower().startswith("0x") else None
        rest = p[1:] if a is not None else p
        rows.append((a, rest[0], rest[1] if len(rest) > 1 else "func",
                     rest[2] if len(rest) > 2 else None))
    return rows


def main():
    ap = argparse.ArgumentParser(
        description=__doc__,
        formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("name", nargs="?")
    ap.add_argument("--addr")
    ap.add_argument("--kind", default="func",
                    choices=["func", "data", "field", "type"])
    ap.add_argument("--ov", help="overlay the symbol belongs to")
    ap.add_argument("--batch", help="file of proposals (see usage)")
    ap.add_argument("--quiet", action="store_true", help="hide INFO")
    a = ap.parse_args()
    if not a.name and not a.batch:
        ap.error("give a NAME or --batch FILE")
    idx = Index()
    rows = parse_batch(a.batch) if a.batch else [
        (int(a.addr, 16) if a.addr else None, a.name, a.kind, a.ov)]
    names = collections.Counter(r[1] for r in rows)
    batch_names = set(names)
    nerr = nwarn = 0
    for addr, name, kind, ov in rows:
        res = lint(name, addr, kind, ov, idx, batch_names)
        if names[name] > 1 and not ov:
            res.append(("ERROR", "collision", "proposed %d times in this batch"
                        % names[name]))
        where = ("%s:" % ov if ov else "") + ("0x%08X" % addr if addr is not
                                              None else "-")
        shown = [r for r in res if not (a.quiet and r[0] == "INFO")]
        nerr += sum(1 for r in res if r[0] == "ERROR")
        nwarn += sum(1 for r in res if r[0] == "WARN")
        if not shown:
            print("%-20s %-28s ok" % (where, name))
        for lv, rule, msg in shown:
            print("%-20s %-28s %-5s %-9s %s" % (where, name, lv, rule, msg))
    if len(rows) > 1:
        print("\n%d names: %d ERROR, %d WARN" % (len(rows), nerr, nwarn))
    sys.exit(1 if nerr else 0)


if __name__ == "__main__":
    main()
