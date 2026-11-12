#!/usr/bin/env python3
"""
naming_census.py -- reproducible statistics behind
docs/reference/NAMING-CONVENTIONS-1994.md.

READ-ONLY.  It reads the o.bin symbol cache and the source tree and prints
counts; it writes nothing unless --json OUT is given (and then only OUT).

Two halves:

  style   The ~2,132 external names in disks/o.bin (1,559 functions, 573
          data, split at the debug text end 0x800C1480) plus the olh overlay's
          own 5 procedures and 9 data symbols.  Names are banded by their
          position in the DEBUG link (o.bin order is link order within a band;
          the addresses themselves do not transfer to retail):

            K-game   warm_reset_init .. twinbee        Konami game core
            K-sd     SD_Call_SD .. cdread_callback      Konami sound driver
            K-date   normal_date_bg_suddenin .. pcm_read_myu
                                                       Konami date/event engine
            Sony     DecDCTReset .. ChangeClearRCnt     Psy-Q libraries,
                                                       libc and BIOS stubs

          Data are split into Konami and Sony by an explicit name rule
          (SONY_DATA_* below), because the debug data layout interleaves them.

  tree    Current func_/D_ inventory of main and the 26 overlays, for sizing
          the rename pipeline.  Nothing here trusts a landed name: a name is
          counted as "named" only when it is not func_XXXXXXXX, and the
          symbol_names.json audit verdict is reported alongside.

Usage:
    python3 tools/analysis/naming_census.py                 # both halves
    python3 tools/analysis/naming_census.py style           # style only
    python3 tools/analysis/naming_census.py tree            # tree only
    python3 tools/analysis/naming_census.py style --examples 8
    python3 tools/analysis/naming_census.py --json OUT.json # machine-readable
    python3 tools/analysis/naming_census.py tokens          # full token table
    python3 tools/analysis/naming_census.py align [--band game|sd-date]
        [--show] [--controls N]
        # DP alignment of o.bin debug gaps against retail function sizes,
        # with shuffled-gap controls.  EVIDENCE for reviewers only -- an
        # exact-size pair is a candidate, never a name to land by itself.
"""

import argparse
import collections
import json
import os
import re
import statistics
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent.parent
SPLIT = ROOT / "build/slps00065/symbols/obin_split.json"
ALLSYM = ROOT / "build/slps00065/symbols/obin_symbols.json"

# --------------------------------------------------------------------------
# o.bin loading
# --------------------------------------------------------------------------


def load_obin():
    """Return (functions, data, olh_local) as lists of dicts in debug order."""
    if not SPLIT.exists() or not ALLSYM.exists():
        sys.exit(
            "missing o.bin cache; run:\n"
            "  python3 tools/analysis/extract_obin_symbols.py --all --json "
            f"{ALLSYM}\n"
            "  python3 tools/analysis/symbol_names.py inventory"
        )
    split = json.load(open(SPLIT))
    allsym = json.load(open(ALLSYM))["symbols"]
    olh = [s for s in allsym
           if not (s["type"] == "Global" and s["storage"] == "Undefined")]
    return split["functions"], split["data"], olh


BAND_MARKERS = [  # (first name of band, band label), in debug order
    ("warm_reset_init", "K-game"),
    ("SD_Call_SD", "K-sd"),
    ("normal_date_bg_suddenin", "K-date"),
    ("DecDCTReset", "Sony"),
]


def band_functions(funcs):
    idx = {f["name"]: i for i, f in enumerate(funcs)}
    starts = [(idx[n], lab) for n, lab in BAND_MARKERS]
    out = []
    for i, f in enumerate(funcs):
        lab = [l for s, l in starts if s <= i][-1]
        out.append(dict(f, band=lab))
    return out


# Sony data: libgs/libgte/libspu/libsnd/libcd/libpress/libapi/libetc names.
SONY_DATA_PREFIX = ("_svm_", "_spu_", "_snd_", "_ss_", "Gs", "PSD", "CD_",
                    "Ring", "StCd", "_LC")
SONY_DATA_EXACT = {
    "DCL_DVLC", "DCC_DVLC", "CF_DVLC", "CF2_DVLC", "GPU_printf", "rsin_tbl",
    "rcossin_tbl", "Vcount", "HWD0", "VWD0", "CLIP2", "DSTACK", "FinalSector",
    "MarkCallback", "VBLANK_MINUS", "kMaxPrograms", "spuVmMaxVoice",
    "PadIdentifier", "dc_y", "dc_cr", "dc_cb", "dc_type", "dec_used",
    "dec_pt", "bs_curt", "mdec_iq", "mdec_coef", "WorldOT", "OTTags",
    "GpuPacketArea", "out_packet2", "activeBuff", "_96_init",
}


def band_data(data):
    out = []
    for d in data:
        n = d["name"]
        sony = n in SONY_DATA_EXACT or n.startswith(SONY_DATA_PREFIX)
        if n.startswith("gSD"):
            lab = "K-sd"
        else:
            lab = "Sony" if sony else "K-game"
        out.append(dict(d, band=lab))
    return out


# --------------------------------------------------------------------------
# lexical classification
# --------------------------------------------------------------------------

CASING = [
    ("snake_lower", re.compile(r"^_?[a-z][a-z0-9]*(_[a-z0-9]+)*$")),
    ("SCREAMING_SNAKE", re.compile(r"^_?[A-Z][A-Z0-9]*(_[A-Z0-9]+)*$")),
    ("Prefix_CamelCase", re.compile(r"^_?[A-Z][A-Za-z0-9]*_[A-Z][A-Za-z0-9]*$")),
    ("Title_Snake", re.compile(r"^[A-Z][a-z0-9]+(_[A-Z][a-z0-9]*)+$")),
    ("CamelCase", re.compile(r"^_?[A-Z][a-z0-9]+([A-Z0-9][A-Za-z0-9]*)+$")),
    ("lowerCamel", re.compile(r"^_?[a-z]+[A-Z][A-Za-z0-9]*$")),
    ("Capitalized", re.compile(r"^[A-Z][a-z0-9]+$")),
]


def casing(name):
    for lab, rx in CASING:
        if rx.match(name):
            return lab
    return "mixed"


def tokens(name):
    """snake tokens; digit runs split off a word tail (init0 -> init,0)."""
    toks = []
    for t in name.strip("_").split("_"):
        if not t:
            continue
        toks.append(t.lower())
    return toks


def stem(tok):
    """strip a trailing ordinal: init0->init, main4a->main, card7->card."""
    m = re.match(r"^([a-z]+?)([0-9][0-9a-f]*)$", tok)
    return m.group(1) if m else tok


# Lifecycle / role suffixes (last token, stemmed)
SUFFIXES = [
    "init", "main", "exit", "disp", "show", "switch", "set", "check",
    "read", "load", "clear", "call", "select", "wait", "move", "on", "off",
    "in", "out", "start", "stop", "reset", "judge", "sub", "func", "table",
    "tbl", "flag", "timer", "sector", "name", "buf", "col", "mode", "kind",
    "string", "pos", "adr", "data", "info", "step", "hassei", "kaifuku",
    "up", "down", "change", "print", "trans", "task", "ok", "go",
]

# Leading verbs (first token)
VERBS = [
    "get", "set", "load", "check", "make", "show", "goto", "change",
    "clear", "init", "put", "print", "move", "read", "search", "select",
    "sort", "swap", "restore", "return", "disp", "join", "trans", "save",
    "draw", "seek", "wait", "day",
]

# Romanized Japanese vocabulary seen in o.bin, with gloss.  Hand-curated;
# counted by exact snake-token (after stemming) match.
JAPANESE = {
    "tokimeki": "heart-throb (the 'love' meter)", "kega": "injury",
    "hassei": "occurrence / outbreak", "kaifuku": "recovery",
    "noiroze": "neurosis (stress illness)", "byouki": "illness",
    "sick": "(English) illness", "uwasa": "rumour",
    "hyouka": "evaluation", "joho": "information", "jyouhou": "information",
    "yuukou": "friendship", "ouen": "cheer / support",
    "touzyou": "appearance (girl debut)", "sasoi": "invitation",
    "kyuuka": "vacation", "syogatu": "New Year", "nenga": "New-Year card",
    "shugaku": "school trip", "basyo": "place", "basho": "place",
    "hizuke": "date (calendar)", "suuzi": "numeral", "yazirushi": "arrow",
    "yajirusi": "arrow", "bukatu": "club activity", "gakko": "school",
    "syoushin": "promotion / improvement", "hosei": "correction factor",
    "bairitu": "multiplier", "kaisu": "count / times", "mozaiku": "mosaic",
    "hitsuji": "sheep (sleep)", "asobi": "play / outing", "gamen": "screen",
    "serifu": "spoken line", "kao": "face", "kuchi": "mouth",
    "junban": "order / turn", "taku": "choice (x-taku = N-way select)",
    "zyotai": "condition", "demachiawase": "meeting up",
    "aisatu": "greeting", "kigou": "symbol", "migi": "right",
    "hidari": "left", "daen": "ellipse", "tate": "vertical / standing",
    "zyota": "condition (abbr.)", "nin": "persons (2nin = two people)",
    "fujisaki": "Fujisaki (Shiori)", "fuji": "Fujisaki (abbr.)",
    "shiori": "Shiori", "himo": "Himoo (Yuina)", "yoshio": "Yoshio (friend)",
    "yosi": "Yoshio (abbr.)", "yoshi": "Yoshio (abbr.)", "myu": "? (unknown)",
    "tai": "physique stat", "bun": "humanities stat", "ri": "science stat",
    "gei": "art stat", "un": "sports stat", "zat": "trivia stat",
    "you": "looks stat", "kon": "guts stat", "bunkei": "humanities",
    "rikei": "science", "geijyutu": "art", "undou": "sports",
    "yousi": "appearance", "neru": "sleep", "denwa": "telephone",
    "savetyuu": "saving...", "yokujitu": "next day", "kun": "-kun",
    "san": "-san", "shu": "week / main", "oname": "name (o-namae)",
    "bye": "(English) goodbye", "don": "? (don_init: kettle-drum / 'don')",
    "yubi": "finger (pointer)", "tarao": "developer/work tag",
    "miura": "developer/work tag", "takak": "developer/work tag",
    "yamag": "developer/work tag", "fuku": "developer/work tag",
    "mascot": "(English) mascot", "radio": "(English) radio",
    "tel": "telephone", "olh": "olh overlay",
    "kanji": "kanji text", "sjis": "Shift-JIS",
    "twinbee": "TwinBee (Konami title)", "konami": "Konami",
    "tokubetu": "special", "geko": "going home (after school)",
    "syougatu": "New Year", "schdeule": "(typo) schedule",
    "callendar": "(typo) calendar", "alubum": "(typo) album",
    "botton": "(typo) button", "cashe": "(typo) cache",
    "fucn": "(typo) func", "tyuu": "middle",
}

# Kunrei/wapuro spellings vs Hepburn spellings (as token substrings)
KUNREI = re.compile(r"(?:^|_)[a-z]*(?:sy[aou]|zy[aou]|ty[aou]|tu(?![a-z]*(?:re|rn))|si(?=[^n]|$)|zi)")
HEPBURN = re.compile(r"(?:sh[aiou]|j[aiou]|ch[aiou]|tsu|fu)")

# Common Konami abbreviations (token -> meaning)
ABBREV = {
    "k": "kanji text system", "csr": "cursor", "bri": "brightness",
    "col": "colour", "para": "parameter (stat)", "bg": "background",
    "sd": "sound driver", "xa": "CD-XA audio", "pn": "? (pn_switch)",
    "srn": "screen", "mod": "mode / modulation", "tbl": "table",
    "buf": "buffer", "fnt": "font", "dat": "data", "mes": "message",
    "cal": "calendar", "ev": "event", "dec": "MDEC decode", "tbg": "? bg",
    "pcm": "PCM audio", "seq": "sequence (music)", "se": "sound effect",
    "vab": "VAB sound bank", "vram": "VRAM", "spu": "SPU", "cd": "CD",
    "adr": "address", "addr": "address", "num": "number", "pos": "position",
    "calx": "calendar x", "caly": "calendar y", "g": "girl", "p": "player",
    "n": "number", "f": "flag", "sprt": "sprite", "opd": "? object data",
    "tc": "? (tc_name)", "nf": "? (get_nf)", "sf": "? (biorhythm_sf)",
    "grp": "group", "lf": "load file", "sys": "system", "ksys": "kanji system",
}


# --------------------------------------------------------------------------
# style census
# --------------------------------------------------------------------------


def is_konami(band):
    return band.startswith("K-")


def style_census(examples=5):
    funcs, data, olh = load_obin()
    F = band_functions(funcs)
    D = band_data(data)
    R = collections.OrderedDict()

    def ex(lst, n=examples):
        return lst[:n]

    # --- bands
    R["function_bands"] = dict(collections.Counter(f["band"] for f in F))
    R["data_bands"] = dict(collections.Counter(d["band"] for d in D))
    R["olh_local"] = [s["name"] for s in olh]

    # --- casing by band
    cas = collections.defaultdict(collections.Counter)
    cas_ex = collections.defaultdict(list)
    for s in F + [dict(d, band="data:" + d["band"]) for d in D]:
        c = casing(s["name"])
        cas[s["band"]][c] += 1
        key = (s["band"], c)
        if len(cas_ex[key]) < examples:
            cas_ex[key].append(s["name"])
    R["casing"] = {b: dict(c) for b, c in cas.items()}
    R["casing_examples"] = {f"{b}|{c}": v for (b, c), v in cas_ex.items()}

    kf = [f for f in F if is_konami(f["band"])]
    ksnake = [f for f in kf if casing(f["name"]) == "snake_lower"]
    kd = [d for d in D if is_konami(d["band"])]

    # --- lengths
    def lenstats(lst):
        L = [len(x["name"]) for x in lst]
        T = [len(tokens(x["name"])) for x in lst]
        return {"n": len(L), "chars_median": statistics.median(L),
                "chars_max": max(L), "tokens_median": statistics.median(T),
                "tokens_max": max(T)}
    R["length"] = {"konami_func": lenstats(kf), "konami_data": lenstats(kd),
                   "sony_func": lenstats([f for f in F if f["band"] == "Sony"]),
                   "sony_data": lenstats([d for d in D if d["band"] == "Sony"])}

    # --- token frequency (Konami functions + data)
    tokc = collections.Counter()
    for s in kf + kd:
        for t in set(stem(t) for t in tokens(s["name"])):
            tokc[t] += 1
    R["top_tokens"] = tokc.most_common(80)

    # --- prefix families (first token / first two tokens)
    first = collections.Counter()
    first2 = collections.Counter()
    fam_ex = collections.defaultdict(list)
    for f in ksnake:
        t = tokens(f["name"])
        first[stem(t[0])] += 1
        fam_ex[stem(t[0])].append(f["name"])
        if len(t) >= 2:
            first2[t[0] + "_" + stem(t[1])] += 1
    R["prefix_first"] = first.most_common(60)
    R["prefix_first2"] = first2.most_common(40)
    R["prefix_examples"] = {k: ex(v) for k, v in fam_ex.items()
                            if first[k] >= 4}

    # --- suffix (role) families
    last = collections.Counter()
    last_ex = collections.defaultdict(list)
    for f in ksnake:
        t = tokens(f["name"])
        if len(t) < 2:
            continue
        s = stem(t[-1])
        last[s] += 1
        last_ex[s].append(f["name"])
    R["suffix_last"] = last.most_common(50)
    R["suffix_examples"] = {k: ex(v) for k, v in last_ex.items()
                            if last[k] >= 3}

    # --- lifecycle groups: a stem X with >=2 of X_init/X_main/X_exit/X/...
    names = set(f["name"] for f in kf)
    life = {}
    for f in ksnake:
        n = f["name"]
        m = re.match(r"^(.*)_(init|main|exit)[0-9a-f]*$", n)
        if m:
            life.setdefault(m.group(1), set()).add(m.group(2))
    groups = {k: sorted(v) for k, v in life.items()}
    full = {k: v for k, v in groups.items()
            if {"init", "main", "exit"} <= set(v)}
    bare = {k: v for k, v in groups.items() if k in names}
    R["lifecycle"] = {"stems_with_init_main_or_exit": len(groups),
                      "stems_with_all_three": len(full),
                      "all_three_examples": sorted(full)[:examples * 2],
                      "stems_also_bare": len(bare),
                      "bare_examples": sorted(bare)[:examples * 2]}

    # --- ordinal suffixes: decimal and hex
    ordd = [f["name"] for f in kf if re.search(r"[a-z][0-9]+$", f["name"])]
    ordh = [f["name"] for f in kf if re.search(r"[0-9][a-f]$", f["name"])]
    ordu = [f["name"] for f in kf if re.search(r"_[0-9]+$", f["name"])]
    R["ordinals"] = {"glued_decimal": len(ordd), "glued_examples": ex(ordd),
                     "hex_digit": len(ordh), "hex_examples": ex(ordh),
                     "underscore_decimal": len(ordu),
                     "underscore_examples": ex(ordu)}

    # --- leading underscore helper variants
    und = [f["name"] for f in kf if f["name"].startswith("_")]
    pairs = [n for n in und if n[1:] in names]
    R["leading_underscore"] = {"konami_funcs": len(und),
                               "with_public_twin": len(pairs),
                               "twin_examples": ex(pairs),
                               "examples": ex(und)}

    # --- verb-first names
    vf = collections.Counter()
    vf_ex = collections.defaultdict(list)
    for f in ksnake:
        t0 = tokens(f["name"])[0]
        if t0 in VERBS:
            vf[t0] += 1
            vf_ex[t0].append(f["name"])
    R["verb_first"] = {"total": sum(vf.values()), "of": len(ksnake),
                       "by_verb": vf.most_common(),
                       "examples": {k: ex(v) for k, v in vf_ex.items()}}

    # --- Japanese vocabulary
    jc = collections.Counter()
    jx = collections.defaultdict(list)
    jn = 0
    for s in kf + kd:
        hit = False
        for t in tokens(s["name"]):
            st = stem(t)
            for key in (t, st):
                if key in JAPANESE:
                    jc[key] += 1
                    jx[key].append(s["name"])
                    hit = True
                    break
        jn += hit
    R["japanese"] = {"names_with_japanese_token": jn, "of": len(kf) + len(kd),
                     "tokens": [(k, v, JAPANESE[k], ex(jx[k], 3))
                                for k, v in jc.most_common()]}
    kun = sum(1 for s in kf + kd if KUNREI.search(s["name"].lower()))
    hep = sum(1 for s in kf + kd if HEPBURN.search(s["name"].lower()))
    R["romanization"] = {"kunrei_like_names": kun, "hepburn_like_names": hep,
                         "note": "substring heuristic; see guide for pairs"}

    # --- abbreviations
    ac = collections.Counter()
    ax = collections.defaultdict(list)
    for s in kf + kd:
        for t in set(stem(t) for t in tokens(s["name"])):
            if t in ABBREV:
                ac[t] += 1
                ax[t].append(s["name"])
    R["abbrev"] = [(k, v, ABBREV[k], ex(ax[k], 4)) for k, v in ac.most_common()]

    # --- data naming patterns (Konami data)
    dn = [d["name"] for d in kd]
    dset = set(dn)
    pat = collections.OrderedDict()

    def dpat(label, pred):
        hits = [n for n in dn if pred(n)]
        pat[label] = (len(hits), ex(hits))
    dpat("now_* (current state)", lambda n: n.startswith("now_"))
    dpat("last_* (previous state)", lambda n: n.startswith("last_"))
    dpat("*_sector (CD LBA tables)", lambda n: "_sector" in n)
    dpat("*_table / *_tbl", lambda n: n.endswith(("_table", "_tbl")) or "_table" in n)
    dpat("*_flag / *flag", lambda n: n.endswith("flag") or "_flag" in n)
    dpat("*_timer / *_cnt / counter", lambda n: n.endswith(("_timer", "_cnt", "counter", "_count")) or n == "s_count")
    dpat("*_name* (strings)", lambda n: "name" in n)
    dpat("*_string / *_mes / message", lambda n: "string" in n or "mes" in n)
    dpat("*_x / *_y / *_w / *_h (geometry)", lambda n: re.search(r"_(x|y|w|h)[0-9]?$", n) is not None or re.search(r"_(x|y)_", n) is not None)
    dpat("*_work / work_* / *_w (work areas)", lambda n: "work" in n or n.endswith("_w"))
    dpat("SCREAMING_SNAKE (menu labels/constant tables)", lambda n: casing(n) == "SCREAMING_SNAKE")
    dpat("g* Hungarian (SD driver)", lambda n: re.match(r"^g[A-Z]", n) is not None)
    dpat("o_<scene>_* animation sets", lambda n: n.startswith("o_") or n.startswith("a_"))
    dpat("*_mode / *_kind", lambda n: n.endswith(("_mode", "_kind")) or "_kind" in n)
    R["data_patterns"] = pat

    # numbered element + aggregate table
    numbered = collections.defaultdict(list)
    for n in dn:
        m = re.match(r"^(.*?)_?([0-9]+|[0-9a-f])$", n)
        if m and re.search(r"[0-9]$", n) or re.match(r"^icon_image[0-9a-c]$", n):
            base = re.sub(r"_?([0-9]+|[0-9a-c])$", "", n)
            numbered[base].append(n)
    agg = {}
    for base, members in numbered.items():
        if len(members) < 3:
            continue
        cand = [b for b in (base, base + "_table", base.rstrip("_"))
                if b in dset]
        agg[base] = {"members": len(members),
                     "aggregate": cand[0] if cand else None}
    R["numbered_arrays"] = agg

    # --- Sony: exact-name discipline
    sony_f = [f["name"] for f in F if f["band"] == "Sony"]
    sony_pref = collections.Counter()
    for n in sony_f:
        m = re.match(r"^(_?[A-Z][a-z]+|_?[a-z]+_|_[a-z]+|[A-Z]+(?=[A-Z][a-z]))", n)
        sony_pref[m.group(1) if m else n] += 1
    R["sony_prefixes"] = sony_pref.most_common(30)

    # --- typos preserved
    typos = [s["name"] for s in kf + kd
             if any(t in s["name"] for t in
                    ("schdeule", "callendar", "alubum", "botton", "cashe",
                     "fucn"))]
    R["typos"] = typos

    # --- developer / work tags
    tags = [s["name"] for s in kf + kd
            if re.search(r"(tarao|miura|takak|yamag|fuku)\b", s["name"])
            or re.search(r"_(tarao|miura|takak|yamag|fuku)$", s["name"])]
    R["developer_tags"] = tags

    # --- Title_Snake island (K-date band)
    ts = [f["name"] for f in F if casing(f["name"]) == "Title_Snake"]
    R["title_snake"] = ts

    # --- olh overlay object (the one overlay o.bin saw)
    R["olh_note"] = ("olh_main (entry/dispatcher), olh_init, olh (per-frame), "
                     "olh_exit, move_menu_title; data menu0..menu7 + olh_menu")
    return R


# --------------------------------------------------------------------------
# tree census
# --------------------------------------------------------------------------

FUNC_RX = re.compile(r"\bfunc_([0-9A-Fa-f]{8})\b")
D_RX = re.compile(r"\bD_([0-9A-Fa-f]{8})\b")
MAIN_TEXT_END = 0x800BB3C0
OVL_BASE = 0x80132000


def scan(paths, rx):
    c = collections.Counter()
    for p in paths:
        try:
            txt = open(p, errors="replace").read()
        except OSError:
            continue
        # drop comments crudely? no: comments name globals too; keep code
        # only by removing /* */ blocks
        txt = re.sub(r"/\*.*?\*/", "", txt, flags=re.S)
        for m in rx.finditer(txt):
            c[m.group(1).upper()] += 1
    return c


def cfiles(d):
    return [str(p) for p in Path(d).rglob("*.c")] + \
        [str(p) for p in Path(d).rglob("*.h")]


def dband(a):
    a = int(a, 16)
    if a < MAIN_TEXT_END:
        return "below-0x800BB3C0(rodata/const)"
    if a < OVL_BASE:
        return "main-data/bss"
    if a < 0x80180000:
        return "overlay-area(0x80132000+)"
    return "fixed-buffer(>=0x80180000)"


def tree_census(top=60):
    R = collections.OrderedDict()
    # ---- main functions
    audit = json.load(open(ROOT / "config/main_function_audit_by_addr.json"))
    addrs = audit["addresses"]
    names = {k: v for k, v in json.load(open(ROOT / "config/symbol_names.json"))["symbols"].items()
             if ":" not in k}    # main records only (overlay keys are "OV:0xADDR")
    verdict = {k.upper(): (v.get("audit") or {}).get("verdict")
               for k, v in names.items()}
    rows = collections.Counter()
    for a, v in addrs.items():
        if v["kind"] not in ("matched_c", "asm_only", "head_fragment"):
            continue
        src = v.get("source") or ""
        base = Path(src).stem if src else ""
        named = bool(base) and not base.lower().startswith("func_")
        vd = verdict.get(a.upper().replace("0X", "0x"))
        vd = verdict.get("0X" + a[2:].upper()) if vd is None else vd
        rows[(v["region"], v["kind"],
              "named" if named else "func_",
              (vd or "-") if named else "-")] += 1
    R["main_functions"] = {"|".join(k): n for k, n in sorted(rows.items())}
    tot = sum(rows.values())
    func_ = sum(n for k, n in rows.items() if k[2] == "func_")
    R["main_function_totals"] = {"entries": tot, "func_": func_,
                                 "named": tot - func_}
    R["symbol_names_json"] = dict(collections.Counter(
        (v["kind"], "landed" if v.get("landed") else "proposed",
         (v.get("audit") or {}).get("verdict") or "-")
        for v in names.values()).items()) and {
        "|".join(k): n for k, n in collections.Counter(
            (v["kind"], "landed" if v.get("landed") else "proposed",
             (v.get("audit") or {}).get("verdict") or "-")
            for v in names.values()).items()}

    # ---- main references
    main_src = (cfiles(ROOT / "src/main/matched") +
                cfiles(ROOT / "src/main/semi-matched"))
    port_src = cfiles(ROOT / "src/main/pc-port")
    pc_src = cfiles(ROOT / "pc")
    inc_src = cfiles(ROOT / "include")
    md = scan(main_src, D_RX)
    mf = scan(main_src, FUNC_RX)
    R["main_refs"] = {
        "files": len(main_src),
        "distinct_D": len(md), "D_refs": sum(md.values()),
        "distinct_func_refs": len(mf),
        "D_by_band": dict(collections.Counter(dband(a) for a in md)),
    }
    pd_ = scan(port_src, D_RX)
    pf = scan(port_src, FUNC_RX)
    qd = scan(pc_src, D_RX)
    qf = scan(pc_src, FUNC_RX)
    idd = scan(inc_src, D_RX)
    idf = scan(inc_src, FUNC_RX)
    R["pc_port_and_include"] = {
        "src/main/pc-port": {"files": len(port_src), "distinct_D": len(pd_),
                             "distinct_func": len(pf)},
        "pc/": {"files": len(pc_src), "distinct_D": len(qd),
                "distinct_func": len(qf)},
        "include/": {"files": len(inc_src), "distinct_D": len(idd),
                     "distinct_func": len(idf)}}

    # ---- overlays
    ovl = collections.OrderedDict()
    allD = collections.Counter(md)
    ovl_users = collections.defaultdict(set)
    for a in md:
        ovl_users[a].add("main")
    for ov in sorted(os.listdir(ROOT / "src/overlay")):
        d = ROOT / "src/overlay" / ov
        if not d.is_dir():
            continue
        sym = ROOT / "config/overlay" / f"{ov}.sym"
        nf = nn = 0
        if sym.exists():
            for line in open(sym):
                line = line.strip()
                if not line or line.startswith("#"):
                    continue
                parts = line.split(":")
                if len(parts) < 3:
                    continue
                if parts[2].lower().startswith("func_"):
                    nf += 1
                else:
                    nn += 1
        files = cfiles(d)
        od = scan(files, D_RX)
        of = scan(files, FUNC_RX)
        local = [a for a in od if int(a, 16) >= OVL_BASE and
                 int(a, 16) < 0x80180000]
        shared = [a for a in od if int(a, 16) < OVL_BASE]
        mainf = [a for a in of if int(a, 16) < OVL_BASE]
        for a in od:
            # overlay-local addresses are per-overlay objects
            key = f"{ov}:{a}" if OVL_BASE <= int(a, 16) < 0x80180000 else a
            allD[key] += od[a]
            ovl_users[key].add(ov)
        ovl[ov] = {"sym_func_": nf, "sym_named": nn, "c_files": len(files),
                   "D_local": len(local), "D_main": len(shared),
                   "D_other": len(od) - len(local) - len(shared),
                   "main_func_called": len(mainf)}
    R["overlays"] = ovl
    R["overlay_totals"] = {k: sum(v[k] for v in ovl.values())
                           for k in next(iter(ovl.values()))}
    def addr(k):
        return int(k.split(":")[-1], 16)
    maindata = [a for a in allD if ":" not in a and
                MAIN_TEXT_END <= addr(a) < OVL_BASE]
    R["distinct_D_union"] = {
        "main_objects_all_bands": sum(1 for a in allD if ":" not in a),
        "main_data_bss": len(maindata),
        "main_text_rodata": sum(1 for a in allD
                                if ":" not in a and addr(a) < MAIN_TEXT_END),
        "overlay_local_per_overlay_sum": R["overlay_totals"]["D_local"],
        "overlay_local_note": "overlays all load at 0x80132000, so the "
                              "same address is a different object per "
                              "overlay; only the per-overlay sum counts",
        "fixed_buffers": sum(1 for a in allD
                             if ":" not in a and addr(a) >= 0x80180000),
        "main_data_used_by_>=2_units": sum(1 for a in maindata
                                           if len(ovl_users[a]) >= 2),
    }
    # hottest globals + per-page histogram of main data
    R["hot_D"] = [(("%s:D_%s" % tuple(a.split(":"))) if ":" in a
                   else f"D_{a}", n, len(ovl_users[a])) for a, n in
                  allD.most_common(top)]
    pages = collections.Counter()
    prefs = collections.Counter()
    for a in maindata:
        pg = int(a, 16) & ~0xFFF
        pages[f"0x{pg:08X}"] += 1
        prefs[f"0x{pg:08X}"] += allD[a]
    R["main_data_pages"] = [(p, pages[p], prefs[p])
                            for p in sorted(pages, key=lambda x: -prefs[x])[:25]]
    return R


# --------------------------------------------------------------------------
# align: o.bin debug gaps vs retail function sizes (evidence, not a lander)
# --------------------------------------------------------------------------


def retail_sizes(lo, hi):
    audit = json.load(open(ROOT / "config/main_function_audit_by_addr.json"))
    ks = sorted(int(k, 16) for k, v in audit["addresses"].items()
                if v["kind"] in ("matched_c", "asm_only", "head_fragment",
                                 "covered"))
    ks = [k for k in ks if lo <= k < hi] + [hi]
    return [(ks[i], ks[i + 1] - ks[i]) for i in range(len(ks) - 1)]


def dp_align(ob, ret):
    """Global alignment of debug gaps to retail sizes.  Match scores: exact
    +5, within 16 bytes +2, else -2; a debug entry may absorb two retail
    entries (a split head/body) at -0.5; skips cost 1."""
    n, m = len(ob), len(ret)
    NEG = -1e18
    S = [[0.0] * (m + 1) for _ in range(n + 1)]
    B = [[0] * (m + 1) for _ in range(n + 1)]

    def sc(g, z):
        if g is None:
            return -1
        if g == z:
            return 5
        return 2 if abs(g - z) <= 16 else -2
    for i in range(n + 1):
        Si, Sp = S[i], S[i - 1] if i else None
        for j in range(m + 1):
            if not i and not j:
                continue
            best, bb = NEG, 0
            if i and Sp[j] - 1 > best:
                best, bb = Sp[j] - 1, 1
            if j and Si[j - 1] - 1 > best:
                best, bb = Si[j - 1] - 1, 2
            if i and j:
                v = Sp[j - 1] + sc(ob[i - 1], ret[j - 1][1])
                if v > best:
                    best, bb = v, 3
            if i and j > 1:
                v = Sp[j - 2] + sc(ob[i - 1], ret[j - 2][1] +
                                   ret[j - 1][1]) - 0.5
                if v > best:
                    best, bb = v, 4
            Si[j], B[i][j] = best, bb
    i, j, pairs = n, m, []
    while i or j:
        b = B[i][j]
        if b == 1:
            i -= 1
        elif b == 2:
            j -= 1
        elif b == 3:
            pairs.append((i - 1, ret[j - 1][0], ret[j - 1][1]))
            i, j = i - 1, j - 1
        else:
            pairs.append((i - 1, ret[j - 2][0],
                          ret[j - 2][1] + ret[j - 1][1]))
            i, j = i - 1, j - 2
    return pairs[::-1]


def run_stats(ob, pairs):
    exact = [(k, a, z) for k, a, z in pairs if ob[k] == z]
    runs, cur, prev = [], [], None
    for k, a, z in pairs:
        if ob[k] == z and cur and prev == k - 1:
            cur.append(k)
        elif ob[k] == z:
            if cur:
                runs.append(cur)
            cur = [k]
        else:
            if cur:
                runs.append(cur)
            cur = []
        prev = k
    if cur:
        runs.append(cur)
    long_ = [r for r in runs if len(r) >= 3]
    return len(exact), sum(len(r) for r in long_), len(long_)


def align(lo, hi, i0, i1, controls=3, show=False):
    import random
    funcs, _, _ = load_obin()
    names = [f["name"] for f in funcs[i0:i1]]
    gaps = [f["gap"] for f in funcs[i0:i1]]
    ret = retail_sizes(lo, hi)
    pairs = dp_align(gaps, ret)
    ex, inrun, nrun = run_stats(gaps, pairs)
    print(f"o.bin[{i0}:{i1}] ({len(gaps)} names) vs retail "
          f"0x{lo:08X}-0x{hi:08X} ({len(ret)} entries)")
    print(f"  real:    paired {len(pairs)}  exact-size {ex}  "
          f"in runs>=3 {inrun} ({nrun} runs)")
    for seed in range(1, controls + 1):
        g = gaps[:]
        random.Random(seed).shuffle(g)
        cp = dp_align(g, ret)
        cex, cin, cn = run_stats(g, cp)
        print(f"  control(shuffled gaps, seed {seed}): exact-size {cex}  "
              f"in runs>=3 {cin} ({cn} runs)")
    if show:
        for k, a, z in pairs:
            mark = "==" if gaps[k] == z else ""
            print(f"  {names[k]:36s} gap {gaps[k]!s:>5} -> 0x{a:08X} "
                  f"size {z:5d} {mark}")


# --------------------------------------------------------------------------
# printing
# --------------------------------------------------------------------------


def pr_style(R):
    print("=== o.bin STYLE CENSUS ===")
    print("function bands:", R["function_bands"])
    print("data bands:    ", R["data_bands"])
    print("olh local:     ", " ".join(R["olh_local"]))
    print("\n-- casing by band")
    for b, c in R["casing"].items():
        print(f"  {b:12s}", dict(sorted(c.items(), key=lambda x: -x[1])))
    print("\n-- casing examples")
    for k, v in sorted(R["casing_examples"].items()):
        print(f"  {k:32s}", ", ".join(v))
    print("\n-- length", json.dumps(R["length"], indent=None))
    print("\n-- top tokens (Konami, distinct names containing token)")
    print("  " + ", ".join(f"{t}:{n}" for t, n in R["top_tokens"]))
    print("\n-- first-token families (Konami snake functions)")
    print("  " + ", ".join(f"{t}:{n}" for t, n in R["prefix_first"]))
    print("\n-- first-two-token families")
    print("  " + ", ".join(f"{t}:{n}" for t, n in R["prefix_first2"]))
    print("\n-- last-token (role) families")
    print("  " + ", ".join(f"{t}:{n}" for t, n in R["suffix_last"]))
    for k in ("init", "main", "exit", "disp", "show", "switch", "set",
              "check", "read", "load", "clear", "call", "select", "judge",
              "hassei", "kaifuku", "sub", "in", "out"):
        if k in R["suffix_examples"]:
            print(f"    *_{k}: " + ", ".join(R["suffix_examples"][k]))
    print("\n-- lifecycle", json.dumps(R["lifecycle"]))
    print("-- ordinals", json.dumps(R["ordinals"]))
    print("-- leading underscore", json.dumps(R["leading_underscore"]))
    print("\n-- verb-first", R["verb_first"]["total"], "of",
          R["verb_first"]["of"], R["verb_first"]["by_verb"])
    for k, v in R["verb_first"]["examples"].items():
        print(f"    {k}_*: " + ", ".join(v))
    print("\n-- Japanese vocabulary:", R["japanese"]["names_with_japanese_token"],
          "of", R["japanese"]["of"], "Konami names")
    for t, n, g, e in R["japanese"]["tokens"]:
        print(f"    {t:14s} {n:3d}  {g:34s} {', '.join(e)}")
    print("-- romanization", R["romanization"])
    print("\n-- abbreviations")
    for t, n, g, e in R["abbrev"]:
        print(f"    {t:6s} {n:3d}  {g:24s} {', '.join(e)}")
    print("\n-- Konami data patterns")
    for k, (n, e) in R["data_patterns"].items():
        print(f"    {k:46s} {n:3d}  {', '.join(e)}")
    print("-- numbered arrays (base: members -> aggregate)")
    for b, v in sorted(R["numbered_arrays"].items()):
        print(f"    {b:22s} {v['members']:3d} -> {v['aggregate']}")
    print("\n-- Sony function prefixes:", R["sony_prefixes"])
    print("-- typos preserved:", R["typos"])
    print("-- developer/work tags:", R["developer_tags"])
    print("-- Title_Snake island:", R["title_snake"])


def pr_tree(R):
    print("\n=== TREE CENSUS ===")
    print("main function entries (region|kind|state|audit):")
    for k, n in R["main_functions"].items():
        print(f"  {k:48s} {n}")
    print("totals:", R["main_function_totals"])
    print("symbol_names.json:", R["symbol_names_json"])
    print("main refs:", R["main_refs"])
    print("pc-port/include:", R["pc_port_and_include"])
    print("\noverlay  sym_func_ sym_named c_files D_local D_main D_other main_func_called")
    for ov, v in R["overlays"].items():
        print(f"  {ov:9s} {v['sym_func_']:6d} {v['sym_named']:6d} "
              f"{v['c_files']:6d} {v['D_local']:7d} {v['D_main']:6d} "
              f"{v['D_other']:6d} {v['main_func_called']:6d}")
    t = R["overlay_totals"]
    print(f"  {'TOTAL':9s} {t['sym_func_']:6d} {t['sym_named']:6d} "
          f"{t['c_files']:6d} {t['D_local']:7d} {t['D_main']:6d} "
          f"{t['D_other']:6d} {t['main_func_called']:6d}")
    print("\ndistinct D_ union:", R["distinct_D_union"])
    print("\nhottest D_ (name, refs, units using it):")
    for n, c, u in R["hot_D"]:
        print(f"  {n} {c:6d} {u:3d}")
    print("\nmain data pages by references (page, distinct D_, refs):")
    for p, n, c in R["main_data_pages"]:
        print(f"  {p} {n:5d} {c:7d}")


def pr_tokens():
    funcs, data, _ = load_obin()
    F = band_functions(funcs)
    D = band_data(data)
    c = collections.Counter()
    for s in F + D:
        if is_konami(s["band"]):
            for t in tokens(s["name"]):
                c[stem(t)] += 1
    for t, n in c.most_common():
        print(f"{n:5d} {t}")


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("what", nargs="?", default="all",
                    choices=["all", "style", "tree", "tokens", "align"])
    ap.add_argument("--band", default="game",
                    choices=["game", "sd-date"],
                    help="align: game = o.bin[0:697] vs retail "
                         "0x80041000-0x80086000; sd-date = o.bin[697:881] vs "
                         "0x80086000-0x800BB3C0")
    ap.add_argument("--controls", type=int, default=3)
    ap.add_argument("--show", action="store_true",
                    help="align: print every aligned pair")
    ap.add_argument("--examples", type=int, default=5)
    ap.add_argument("--top", type=int, default=60)
    ap.add_argument("--json", metavar="OUT")
    a = ap.parse_args()
    out = {}
    if a.what == "tokens":
        pr_tokens()
        return
    if a.what == "align":
        if a.band == "game":
            align(0x80041000, 0x80086000, 0, 697, a.controls, a.show)
        else:
            align(0x80086000, MAIN_TEXT_END, 697, 881, a.controls, a.show)
        return
    if a.what in ("all", "style"):
        out["style"] = style_census(a.examples)
        if not a.json:
            pr_style(out["style"])
    if a.what in ("all", "tree"):
        out["tree"] = tree_census(a.top)
        if not a.json:
            pr_tree(out["tree"])
    if a.json:
        with open(a.json, "w") as fh:
            json.dump(out, fh, indent=1, default=list)
        print("wrote", a.json)


if __name__ == "__main__":
    main()
