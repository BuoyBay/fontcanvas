# Measure private Windows fonts and render them to Pictures

Opt-in Windows GDI+ private-font measurement and raster drawing. A
`PrivateFontCanvas` owns an independent `PrivateFontCollection` loaded from one
font file. It never installs/registers a GDI font or accesses Buoy's runtime-owned
collections. Its `Rasterize` result is copied through public `Picture.FromBuffer`;
use `Graphics.DrawPicture` to compose it in Canvas or an offscreen Picture.

`PrivateFontMetrics` contains `X`, `Y`, `Width`, `Height`, `Ascent`, `Descent`,
`LineSpacing`. Size, coordinates, bounds and scalar metrics are raster pixels,
with the first baseline at the caller's Y. X/Y are relative to that baseline.
Bounds are GDI+ typographic layout bounds (including trailing spaces), not an
exact glyph ink box or a guarantee of equality with Buoy GDI metrics. Empty text
has zero bounds while retaining font scalar metrics. The same font/format and
measurement path is used for measurement and drawing. The typographic format is
single-line, no-wrap; CR/LF/tab and U+2028/U+2029 are rejected. Baseline placement
uses the selected family's ascent/em metrics. Underline is a decoration.

## Install

From a Buoy consumer project with BPM installed:

```sh
bpm add fontcanvas --git https://github.com/BuoyBay/fontcanvas.git --rev refs/heads/main
```

BPM records the resolved commit in its lockfile. Keep that lockfile with the
consumer project for reproducible installs.

## API entrypoints

- `PrivateFontCanvas`
- `PrivateFontCollection`
- `Rasterize`
- `fontcanvas.FontCanvas As Fonts`
- `PrivateFontCanvasSupported()`

## Platform and scope

The private-font backend is Windows-specific. Rendering returns public Buoy Pictures; it does not make private fonts globally installed.

## Build and API reference

Source is in `modules/`, with native/runtime inputs where required. Use the
[package guide](docs/API.md) for backend-specific build prerequisites, ownership
rules and supported targets. Rebuilding a native backend requires that target's
compiler and SDK; sealing an archive alone does not rebuild the backend.

## Local Buoy setup

On your own machine, install Buoy and run:

```sh
buoy ai init --both
```


## Contributing

[Contributing](CONTRIBUTING.md) · [License](LICENSE)
