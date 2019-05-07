#!/usr/bin/env python3
import sys

HEADER = """/*
 * Author: Sam McCreery 2019
 * License: Creative Commons Attribution
 *          CC BY, https://creativecommons.org/licenses/by/4.0/
 */
"""

for name in sys.argv[1:]:
    with open(name, "r") as f:
        contents = f.read()

    if contents[:2] != "/*":
        with open(name, "w") as f:
            f.write(HEADER + contents)
