---
title: "fontcanvas API contracts"
summary: "Usage and behavioral contracts for fontcanvas."
category: "Reference"
symbols: ["PrivateFontMetrics", "PrivateFontException", "PrivateFontCanvas"]
related: ["index.md", "getting-started.md", "typed-api.md"]
---

# FontCanvas 0.1.0

Opt-in Windows GDI+ private-font measurement and raster drawing for Buoy. A
`PrivateFontCanvas` owns an independent `PrivateFontCollection` loaded from one
font file. It never installs/registers a GDI font or accesses Buoy's runtime-owned
collections. Its `Rasterize` result is copied through public `Picture.FromBuffer`;
use `Graphics.DrawPicture` to compose it in Canvas or an offscreen Picture.

Import `fontcanvas.FontCanvas As Fonts`; types remain unqualified.

| API | Behavior |
| --- | --- |
| `PrivateFontCanvasSupported()` | Windows platform predicate, not runtime dependency verification. |
| `New PrivateFontCanvas(path)` | Load a UTF-8 file path into a private collection or throw. |
| `FamilyCount()`, `FamilyName(index=0)` | Enumerate only families from the loaded file. |
| `StyleAvailable(index=0,bold=False,italic=False)` | Query GDI+'s native face/style capability. |
| `Measure(text,size,index=0,bold=False,italic=False,underline=False)` | Return GDI+ typographic bounds and native scalar font metrics. |
| `Rasterize(text,size,width,height,x,baseline,red,green,blue,alpha=1,index=0,bold=False,italic=False,underline=False)` | Copy a straight-alpha RGBA raster into a public Picture. |
| `IsOpen`, `Close()` | Explicit owner state; Close is idempotent and the destructor closes. |

`PrivateFontMetrics` contains `X`, `Y`, `Width`, `Height`, `Ascent`, `Descent`,
`LineSpacing`. Size, coordinates, bounds and scalar metrics are raster pixels,
with the first baseline at the caller's Y. X/Y are relative to that baseline.
Bounds are GDI+ typographic layout bounds (including trailing spaces), not an
exact glyph ink box or a guarantee of equality with Buoy GDI metrics. Empty text
has zero bounds while retaining font scalar metrics. The same font/format and
measurement path is used for measurement and drawing. The typographic format is
single-line, no-wrap; CR/LF/tab and U+2028/U+2029 are rejected. Baseline placement
uses the selected family's ascent/em metrics. Underline is a decoration.

Family selection is by collection index, so a missing requested family cannot
silently select a system family. Unavailable bold/italic combinations fail
explicitly with `PrivateFontException` code 2. GDI+ may expose synthetic styles;
availability does not prove a separate physical face exists. Per-glyph fallback,
complex-script shaping, bidi and font coverage remain native GDI+ behavior;
no coverage or resolved fallback-run API is claimed. This is not DirectWrite.

Use and release a collection on one application thread. No native font objects
escape a call. Returned Pictures own copied pixels and survive `Close()`.
Do not delete/replace the font file before closing its collection. Text/path
arguments use NUL-terminated UTF-8; Buoy's existing String ABI can discard NUL
before the package sees it, so no end-to-end embedded-NUL rejection is claimed.

Text is capped at 1 MiB of UTF-8 and UTF-16-unit-count × size at 262144. Size is
finite in `(0,16384]`, raster dimensions in `1..16384`, RGBA buffer at most64 MiB,
finite coordinates and normalized RGBA in `[0,1]`. Multiple temporary buffers
can use more than the final RGBA size. Pixels cross the public API as straight
RGBA; GDI+ `PixelFormat32bppARGB` lock data is straight-alpha BGRA and is reordered.
Partial-alpha Picture round trips have ordinary 8-bit rounding. Transparent
pixels have zero RGB. ClearType is not used on transparent surfaces.

Invalid arguments throw `InvalidArgumentException`; font/load/rendering problems
throw `PrivateFontException`. Other platforms throw `UnsupportedOperationException`
on construction. The platform predicate is safe everywhere. Existing built-in
`Graphics.DrawString` font selection is unchanged; only explicit public-image
composition uses this package.

## Native build requirements

The C++ bridge is Windows-specific and requires a matching Windows compiler,
SDK and import-library toolchain. Compile it for the same architecture as the
consumer. Other platforms expose the capability predicate but reject collection
construction.

Native buffer capacities are explicit: `New Bytes(capacity)` reserves storage
without increasing Count. Raster results cross the public API as copied Pictures.
The associated compiler-runtime notices remain in `resources/licenses/`.

## Native references

- GDI+ private font collections
- Native font metrics
- Bitmap LockBits and stride
- ARGB versus premultiplied PARGB

[Typed API](typed-api.md) · [Getting started](getting-started.md)
