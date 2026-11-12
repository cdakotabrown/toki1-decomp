# PSY-Q compatible assembly macros
# Ensures move instruction uses addu encoding (0x21) instead of or (0x25)

.macro move a, b
    addu \a, \b, $zero
.endm

.macro negu a, b
    subu \a, $zero, \b
.endm
