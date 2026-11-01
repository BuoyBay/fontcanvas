---
title: FontCanvas
summary: Windows GDI+ private font collections, typographic metrics and public Picture rasterization.
category: Graphics
related: ["getting-started.md", "api.md", "typed-api.md"]
---

FontCanvas loads one font file into an independently owned GDI+ private
collection. It exposes only that file's families and rejects missing style
combinations explicitly. Raster results are copied through `Picture.FromBuffer`
and composed using `Graphics.DrawPicture`, without Buoy runtime internals or
global font registration.

Import `fontcanvas.FontCanvas As Fonts`; construct `PrivateFontCanvas`, query
families/styles, then call `Measure` or `Rasterize`. Sizes and placement are
pixels, with a caller-supplied first baseline. Close the collection on its owning
application thread; copied Pictures remain valid afterward.

The complete platform, ownership, input limits, native references and verified
test instructions ship in [reference manual](reference.md). The checked executable
example in this workspace is `tests/fontcanvas_test.bui`. Windows x64 has native
execution evidence; non-Windows stubs have six-target compiler checks and a
Linux unsupported-path execution test. Windows ARM64 needs its own bridge build
and native verification.
