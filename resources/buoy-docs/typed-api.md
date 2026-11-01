---
title: "fontcanvas typed API"
summary: "Source-derived public signatures, properties, parameters, defaults, and return values."
category: "Reference"
order: 21
types: ["PrivateFontCanvas", "PrivateFontException", "PrivateFontMetrics"]
dependencies: []
symbols: ["PrivateFontCanvas", "PrivateFontException", "PrivateFontMetrics"]
related: ["api.md", "getting-started.md", "index.md"]
---

# fontcanvas typed API

Generated from the package source declarations. Platform guards are shown where present; inherited members are documented on the base type. See the [API contracts](api.md) for behavior and limitations.

## PrivateFontMetrics

`Sequential Structure PrivateFontMetrics`

### Fields

| Name | Type | Value / Default | Description |
| --- | --- | --- | --- |
| `X` | `Double` | — | — |
| `Y` | `Double` | — | — |
| `Width` | `Double` | — | — |
| `Height` | `Double` | — | — |
| `Ascent` | `Double` | — | — |
| `Descent` | `Double` | — | — |
| `LineSpacing` | `Double` | — | — |

## PrivateFontException

`Public Class PrivateFontException Inherits Exception`

### Constructor

`Public Sub Constructor(message As String, code As Integer = 0)`

#### Parameters

| Name | Type | Passing | Default |
| --- | --- | --- | --- |
| `message` | `String` | `ByVal` | Required |
| `code` | `Integer` | `ByVal` | `0` |

Returns: no value.

## FontCanvas functions

### PrivateFontCanvasSupported

`Public Function PrivateFontCanvasSupported() As Boolean`

No parameters.

#### Returns

| Type |
| --- |
| `Boolean` |

## PrivateFontCanvas

`Public Class PrivateFontCanvas`

Owns an independent GDI+ private font collection. Use and release on one thread.

### Properties

| Name | Type | Read-Only | Shared | Default | Description |
| --- | --- | --- | --- | --- | --- |
| `IsOpen` | `Boolean` | Yes | No | — | — |

### Constructor

`Public Sub Constructor(filePath As String)`

#### Parameters

| Name | Type | Passing | Default |
| --- | --- | --- | --- |
| `filePath` | `String` | `ByVal` | Required |

Returns: no value.

### FamilyCount

`Public Function FamilyCount() As Integer`

No parameters.

#### Returns

| Type |
| --- |
| `Integer` |

### FamilyName

`Public Function FamilyName(index As Integer = 0) As String`

#### Parameters

| Name | Type | Passing | Default |
| --- | --- | --- | --- |
| `index` | `Integer` | `ByVal` | `0` |

#### Returns

| Type |
| --- |
| `String` |

### StyleAvailable

`Public Function StyleAvailable(index As Integer = 0, bold As Boolean = False, italic As Boolean = False) As Boolean`

#### Parameters

| Name | Type | Passing | Default |
| --- | --- | --- | --- |
| `index` | `Integer` | `ByVal` | `0` |
| `bold` | `Boolean` | `ByVal` | `False` |
| `italic` | `Boolean` | `ByVal` | `False` |

#### Returns

| Type |
| --- |
| `Boolean` |

### Measure

`Public Function Measure(text As String, size As Double, index As Integer = 0, bold As Boolean = False, italic As Boolean = False, underline As Boolean = False) As PrivateFontMetrics`

#### Parameters

| Name | Type | Passing | Default |
| --- | --- | --- | --- |
| `text` | `String` | `ByVal` | Required |
| `size` | `Double` | `ByVal` | Required |
| `index` | `Integer` | `ByVal` | `0` |
| `bold` | `Boolean` | `ByVal` | `False` |
| `italic` | `Boolean` | `ByVal` | `False` |
| `underline` | `Boolean` | `ByVal` | `False` |

#### Returns

| Type |
| --- |
| `PrivateFontMetrics` |

### Rasterize

`Public Function Rasterize(text As String, size As Double, width As Integer, height As Integer, x As Double, baseline As Double, red As Double, green As Double, blue As Double, alpha As Double = 1.0, index As Integer = 0, bold As Boolean = False, italic As Boolean = False, underline As Boolean = False) As Picture`

Size, placement and metrics are raster pixels, with first baseline anchored at Y.

#### Parameters

| Name | Type | Passing | Default |
| --- | --- | --- | --- |
| `text` | `String` | `ByVal` | Required |
| `size` | `Double` | `ByVal` | Required |
| `width` | `Integer` | `ByVal` | Required |
| `height` | `Integer` | `ByVal` | Required |
| `x` | `Double` | `ByVal` | Required |
| `baseline` | `Double` | `ByVal` | Required |
| `red` | `Double` | `ByVal` | Required |
| `green` | `Double` | `ByVal` | Required |
| `blue` | `Double` | `ByVal` | Required |
| `alpha` | `Double` | `ByVal` | `1.0` |
| `index` | `Integer` | `ByVal` | `0` |
| `bold` | `Boolean` | `ByVal` | `False` |
| `italic` | `Boolean` | `ByVal` | `False` |
| `underline` | `Boolean` | `ByVal` | `False` |

#### Returns

| Type |
| --- |
| `Picture` |

### Close

`Public Sub Close()`

No parameters.

Returns: no value.

### Destructor

`Public Sub Destructor()`

No parameters.

Returns: no value.
