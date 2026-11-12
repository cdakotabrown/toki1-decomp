#!/usr/bin/env python3
"""Custom post-processing for final_year_event_check.

This function has 3 extra scalar variables (count, p, i) that cause IDO to
allocate a 96-byte frame instead of the target's 104-byte frame. The arrays
sp4C[] and sp2C[] land at sp+52 and sp+84 in the compiled code, but the target
has them at sp+44 and sp+76.

Additionally, the target uses t-registers shifted by +1 relative to what IDO
compiles, requiring a series of liveness-aware register renames.

Target: 73 instructions (292 bytes)

Transformations (in order):
1. patch_stack_complete --old 96 --new 104  (frame: 96 -> 104)
2. remap_stack_slots --map 52:44,84:76      (array offsets: 52->44, 84->76)
3. swap_registers_liveness x7 passes       (t-register cyclic shift)
"""
import subprocess
import sys
import os
import tempfile


def run_tool(tool_path, args, input_path, output_path):
    """Run a tool with the given args, input.o -> output.o."""
    cmd = [sys.executable, tool_path] + args + [input_path, output_path]
    result = subprocess.run(cmd, capture_output=True, text=True)
    if result.returncode != 0:
        print(f"ERROR: {tool_path} failed:")
        print(result.stdout)
        print(result.stderr)
        sys.exit(1)
    return result


def main():
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} input.o output.o")
        sys.exit(1)

    input_path = sys.argv[1]
    output_path = sys.argv[2]

    tools_dir = os.path.dirname(os.path.abspath(__file__))
    patch_stack = os.path.join(tools_dir, "patch_stack_complete.py")
    remap_slots = os.path.join(tools_dir, "remap_stack_slots.py")
    swap_live = os.path.join(tools_dir, "swap_registers_liveness.py")

    # Find project root (two levels up from tools/ido/)
    proj_root = os.path.dirname(os.path.dirname(tools_dir))
    target_exe = os.path.join(proj_root, "disks", "SLPS_000.65")
    func_addr = "0x8006EAE4"

    # Use a temporary directory for intermediate files
    with tempfile.TemporaryDirectory() as tmpdir:
        step = [input_path]

        def next_step(name):
            path = os.path.join(tmpdir, name)
            step.append(path)
            return step[-2], path

        # Step 1: patch_stack_complete --old 96 --new 104
        inp, out = next_step("s1_patchstack.o")
        run_tool(patch_stack, ["--old", "96", "--new", "104"], inp, out)

        # Step 2: remap_stack_slots --map 52:44,84:76
        inp, out = next_step("s2_remapslots.o")
        run_tool(remap_slots, ["--map", "52:44,84:76"], inp, out)

        # Steps 3-9: swap_registers_liveness (7 passes, converge automatically)
        # Each pass applies whatever safe renames it finds and exits.
        # We iterate until no more renames are found or we've done 10 passes.
        current = step[-1]
        for i in range(10):
            next_path = os.path.join(tmpdir, f"s{3+i}_swaplive.o")
            cmd = [
                sys.executable, swap_live,
                current, next_path,
                "--target-exe", target_exe,
                "--address", func_addr,
            ]
            result = subprocess.run(cmd, capture_output=True, text=True)
            if result.returncode != 0:
                print(f"ERROR: swap_registers_liveness pass {i+1} failed:")
                print(result.stdout)
                print(result.stderr)
                sys.exit(1)
            if "No safe renames found" in result.stdout or \
               "No safe renames identified" in result.stdout:
                # Converged - use previous output
                break
            current = next_path

        # Copy final result to output
        import shutil
        shutil.copy2(current, output_path)

    print(f"Done: final_year_event_check post-processing complete -> {output_path}")


if __name__ == "__main__":
    main()
