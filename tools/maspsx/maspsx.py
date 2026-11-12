import argparse
import shutil
import subprocess
import sys

from typing import List
from pathlib import Path

from maspsx import MaspsxProcessor


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--aspsx-version", type=str)
    parser.add_argument("--run-assembler", action="store_true")
    parser.add_argument("--gnu-as-path", default="mipsel-linux-gnu-as")
    parser.add_argument("--dont-force-G0", action="store_true")
    parser.add_argument("--expand-div", action="store_true")
    parser.add_argument("--defer-div", action="store_true")
    parser.add_argument("--mflo-nops-first", action="store_true")
    parser.add_argument("--div-mf-with-check", action="store_true")
    parser.add_argument("--defer-div-check-first-on-slot-read", action="store_true")
    parser.add_argument("--mflo-div-writer-slot", action="store_true")
    parser.add_argument("--mflo-noreorder-nops", action="store_true")
    parser.add_argument("--mflo-consumer-fill", action="store_true")
    parser.add_argument("--div-result-no-nop", action="store_true")
    parser.add_argument("--mflo-hoist-skip", type=str, default="")
    parser.add_argument("--load-base-nop", action="store_true")
    parser.add_argument("--delay-slot-load-nop", action="store_true")
    parser.add_argument("--label-load-nop", action="store_true")
    parser.add_argument("--loc-load-nop", action="store_true")
    parser.add_argument("--konami-lib-sched", action="store_true")
    parser.add_argument("--macro-lui-fill", action="store_true")
    parser.add_argument("--macro-lui-fill-far", action="store_true")
    parser.add_argument("--macro-lui-fill-cross", action="store_true")
    parser.add_argument("--macro-lui-fill-split", action="store_true")
    parser.add_argument("--load-dest-temp", action="store_true")
    parser.add_argument("--jal-load-nop", action="store_true")
    parser.add_argument("--store-macro-split", action="store_true")
    parser.add_argument("--store-macro-split-window", type=int, default=3)
    parser.add_argument("--reuse-at-same-hi", action="store_true")
    parser.add_argument("--macro-inc", action="store_true")
    parser.add_argument("--dont-expand-li", action="store_true")
    parser.add_argument("--force-stdin", action="store_true")
    parser.add_argument("--use-comm-section", action="store_true")
    parser.add_argument("--use-comm-for-lcomm", action="store_true")
    # decomp.me debugging
    parser.add_argument("--print-output", action="store_true")
    parser.add_argument("--print-input", action="store_true")
    # deprecated
    parser.add_argument("--no-macro-inc", action="store_true")
    parser.add_argument("--expand-li", action="store_true")

    args, as_args = parser.parse_known_args()

    if args.no_macro_inc:
        sys.stderr.write(
            "MASPSX: --no-macro-inc is no longer required and will be removed in a future update\n"
        )

    if args.expand_li:
        sys.stderr.write(
            "MASPSX: --expand-li is enabled automatically if --aspsx-version is below 2.56\n"
        )

    read_from_file = sys.stdin.isatty()

    if not read_from_file:
        in_lines = sys.stdin.readlines()
        if len(in_lines) == 0:
            if args.force_stdin:
                sys.stderr.write("MASPSX: --force-stdin but no input from stdin!\n")
                sys.exit(1)
            else:
                sys.stderr.write(
                    "MASPSX: Warning, no input from stdin, will try to read from a file\n"
                )
                read_from_file = True

    if read_from_file:
        try:
            input_file = as_args.pop()
        except IndexError:
            sys.stderr.write("MASPSX: Error, no input file found!\n")
            sys.exit(1)

        with open(input_file, "r", encoding="utf") as f:
            in_lines = f.readlines()

    if args.print_input:
        sys.stderr.write("".join(in_lines))

    preamble = [
        '.include "macro.inc"' if args.macro_inc else "",
    ]

    sdata_limit = 0
    filtered_as_args: List[str] = []
    for arg in as_args:
        # Can we stop gcc from passing us this flag?
        if arg == "-KPIC":
            continue

        # GNU as does not support -mcpu flag
        if arg.startswith("-mcpu="):
            arg = arg.replace("-mcpu=", "-mtune=")

        elif arg.startswith("-G") and len(arg) > 2:
            sdata_limit = int(arg[2:])

        filtered_as_args.append(arg)

    div_uses_tge = False  # use tge instruction instead of break in divide
    nop_at_expansion = False  # insert nop between v0/at?
    nop_mflo_mfhi = True  # ensure 2 ops between mfhi/mflo and div/mult
    sltu_at = True  # sltu uses at?
    expand_li = True  # turn li into lui/ori
    gp_allow_offset = False  # use gp for sym+offset?
    gp_allow_la = False  # use gp for la
    addiu_at = False  # use addiu when expanding lw to use $at

    if args.aspsx_version:
        aspsx_version = tuple(int(x) for x in args.aspsx_version.split("."))
        if (1, 10) < aspsx_version < (2, 10):
            div_uses_tge = True
        if aspsx_version < (2, 30):
            nop_at_expansion = True
            nop_mflo_mfhi = False
            addiu_at = True
        if aspsx_version >= (2, 50):
            expand_li = False
        if aspsx_version >= (2, 70):
            sltu_at = False
            gp_allow_offset = True
        if aspsx_version >= (2, 80):
            gp_allow_la = True

    if args.dont_expand_li and expand_li:
        expand_li = False

    maspsx_processor = MaspsxProcessor(
        in_lines,
        sdata_limit=sdata_limit,
        expand_div=args.expand_div,
        defer_div=args.defer_div,
        expand_li=expand_li,
        nop_at_expansion=nop_at_expansion,
        nop_mflo_mfhi=nop_mflo_mfhi,
        mflo_nops_first=args.mflo_nops_first,
        div_mf_with_check=args.div_mf_with_check,
        div_check_first_on_slot_read=args.defer_div_check_first_on_slot_read,
        mflo_div_writer_slot=args.mflo_div_writer_slot,
        mflo_noreorder_nops=args.mflo_noreorder_nops,
        mflo_consumer_fill=args.mflo_consumer_fill,
        div_result_no_nop=args.div_result_no_nop,
        mflo_hoist_skip=tuple(int(x) for x in args.mflo_hoist_skip.split(",") if x),
        load_base_nop=args.load_base_nop,
        delay_slot_load_nop=args.delay_slot_load_nop,
        label_load_nop=args.label_load_nop,
        loc_load_nop=args.loc_load_nop,
        macro_lui_fill=args.macro_lui_fill,
        macro_lui_fill_far=args.macro_lui_fill_far,
        macro_lui_fill_cross=args.macro_lui_fill_cross,
        macro_lui_fill_split=args.macro_lui_fill_split,
        load_dest_temp=args.load_dest_temp,
        jal_load_nop=args.jal_load_nop,
        store_macro_split=args.store_macro_split,
        store_macro_split_window=args.store_macro_split_window,
        reuse_at_same_hi=args.reuse_at_same_hi,
        sltu_at=sltu_at,
        addiu_at=addiu_at,
        div_uses_tge=div_uses_tge,
        gp_allow_offset=gp_allow_offset,
        gp_allow_la=gp_allow_la,
        use_comm_section=args.use_comm_section,
        use_comm_for_lcomm=args.use_comm_for_lcomm,
        konami_lib_sched=args.konami_lib_sched,
    )
    try:
        out_lines = maspsx_processor.process_lines()
    except Exception as err:
        sys.stderr.write(f"MASPSX: An exception occurred: {err}\n")
        sys.exit(1)

    out_text = "\n".join(preamble + out_lines)

    # avoid "Warning: end of file not at end of a line; newline inserted"
    out_text += "\n"

    if args.print_output:
        sys.stderr.write(out_text)

    if args.run_assembler:
        gnu_as_path = Path(args.gnu_as_path)
        if not gnu_as_path.is_file() and not shutil.which(args.gnu_as_path):
            sys.stderr.write(f"MASPSX: {args.gnu_as_path} not found")
            sys.exit(1)

        cmd = [
            args.gnu_as_path,
            *filtered_as_args,
            "-",  # read from stdin
        ]
        if not args.dont_force_G0:
            cmd.insert(-1, "-G0")

        with subprocess.Popen(
            cmd, stdout=subprocess.PIPE, stdin=subprocess.PIPE, stderr=subprocess.PIPE
        ) as process:
            out_bytes = out_text.encode("utf")
            stdout, stderr = process.communicate(input=out_bytes)
            if len(stdout):
                sys.stdout.write(stdout.decode("utf"))
            if len(stderr):
                sys.stderr.write(stderr.decode("utf"))
    else:
        sys.stdout.write(out_text)


if __name__ == "__main__":
    main()
