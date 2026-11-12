"""Identity transform — passes assembly text through unchanged.
Used by ido_as_intercept when only the as0→as1 pipeline change is needed."""

def transform(lines):
    return lines
