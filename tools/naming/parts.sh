# Sourced by the landers (land_overlay.sh, land_overlay_data.sh, land_data.sh)
# after they cd to the repository root.  Says which other parts of the project
# this checkout holds, through tools/repo_paths.py:
#
#   HAVE_PC=1       the PC port (pc/, src/main/pc-port) is in THIS checkout
#   HAVE_ENGLISH=1  the English patch (tools/patch, config/patch, src/patch) is
#
# A lander keeps a part in step (renames, module regeneration, its gates)
# only when that part is in this checkout: the landing is one commit here and
# never writes into a sibling repository.  Every step it leaves out is named
# with `part_skip`, so the log says what still has to run elsewhere.
_parts_root=$(pwd -P)
_parts_out=$(python3 tools/repo_paths.py) || return 9 2>/dev/null || exit 9
PC_DIR=$(sed -n 's/^PC=//p' <<<"$_parts_out")
ENGLISH_DIR=$(sed -n 's/^ENGLISH=//p' <<<"$_parts_out")
HAVE_PC=0; [ "$PC_DIR" = "$_parts_root" ] && HAVE_PC=1
HAVE_ENGLISH=0; [ "$ENGLISH_DIR" = "$_parts_root" ] && HAVE_ENGLISH=1

# part_where PART DIR: a short phrase for the skip notice
_part_where() {
  if [ -z "$2" ]; then echo "$1 is not present"
  else echo "$1 is not in this checkout (it is at $2; run the step there)"; fi
}
# part_skip pc|english STEP...: say out loud that STEP was not run
part_skip() {
  local part=$1; shift
  case $part in
    pc) echo "SKIP: $(_part_where 'the PC port' "$PC_DIR"): $*" ;;
    english) echo "SKIP: $(_part_where 'the English patch' "$ENGLISH_DIR"): $*" ;;
  esac
}
# parts_banner: one line per absent part, for the top of a lander's log
parts_banner() {
  [ $HAVE_PC = 1 ] || part_skip pc "every PC-port step of this landing (renames in pc/, module regeneration, PC gates)"
  [ $HAVE_ENGLISH = 1 ] || part_skip english "every English-patch step of this landing (sync_patch_symbols.py, fast_check.py)"
}
