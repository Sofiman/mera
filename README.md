# MIRA (Mathematical Expression Rendering Assistant)

> ⚠️a This library is not ready yet. Use it at your own risk.

MIRA is a light weight C library that can layout and rasterize mathematical
expressions. This library can render to any user-defined target such as image
files, frame buffers (usable in embedded applications) and more! Please note
that library does not perform any tokenisation or calculation: the expected
input is a vector of tokens.

# Installation

*TBD*

# Usage

*TBD*

# Roadmap

This is the list of expected features:
- [ ] Render basic expressions such as `1+1=2`
- [ ] Layout medium complexity expressions such as `max(1/5, 2)` including
fractions and functions like sqrt.
- [ ] Rendering of medium complexity expressions such as `1 + sin(max(srqt(90), 180/3)*π)`
- [ ] Rendering using user-provided TTF fonts
- [ ] Layout high complexity expressions such as matrices, vectors of
scalars and sets.
- [ ] Rendering of high complexity expressions such operation on matrices and/or
on vectors.

*This roadmap will be further extended as needed.*
