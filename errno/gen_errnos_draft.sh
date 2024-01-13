#!/bin/sh

# arthurbacci's suspicious tools
# PART OF THE BUILD OF ERRNO
#
# Distributed under the public domain under the terms of either:
# 	Unlicense
# 	CC0
# 	BSD0
#
# ~ Arthur Bacci 2024

cc -dM -E include_errno.c | sed -n 's/#define E\(.*\)[\\t ].*/ERRNO_MACRO\(E\1, \"E\1\"\)/gp' > errnos_draft.def

