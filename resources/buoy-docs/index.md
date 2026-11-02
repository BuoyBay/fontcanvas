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

The [reference manual](reference.md) documents platform support, ownership, input limits and native APIs. The Windows bridge must be built for each deployment architecture; unsupported-target stubs do not provide native font collection operations.
