# ESPHomeFastEPD

Thin ESPHome external component wrapper around [bitbank2/FastEPD](https://github.com/bitbank2/FastEPD), focused on **M5Stack PaperS3 (ESP32-S3)**.

## What this repo is

This repository is an **ESPHome external component** that adds a `display:` platform named `fastepd_display`.

The wrapper is intentionally small:
- It wires ESPHome's drawing/lambda pipeline into FastEPD.
- It does **not** vendor the FastEPD source tree.
- It fetches FastEPD from upstream at build time via ESPHome codegen (`cg.add_library(...)`).

## Current support (first milestone)

- Hardware target: **M5Stack PaperS3 / ESP32-S3**
- Display mode: monochrome full refresh
- ESPHome display lambda support (`it.fill(...)`, `it.print(...)`, etc.)
- Goal validated by config: render a simple **"Hello world"** display lambda

## Framework compatibility (important)

FastEPD upstream includes both Arduino and ESP-IDF build assets. However, this first wrapper milestone currently enforces:

- ✅ `esp32` + **Arduino framework**
- ❌ pure `esp-idf` framework in ESPHome (not enabled in this initial wrapper)

Reason: the current integration path uses ESPHome's Arduino library dependency flow (`cg.add_library(...)`) as the smallest, safest path for a working external component milestone. A future milestone can add a dedicated ESP-IDF dependency path (using ESPHome's IDF component integration) once validated end-to-end for ESPHome + PaperS3.

## Usage

```yaml
external_components:
  - source:
      type: git
      url: https://github.com/ThomasHFWright/ESPHomeFastEPD
      ref: main
    components: [fastepd_display]

font:
  - file: "gfonts://Roboto"
    id: my_font
    size: 42

display:
  - platform: fastepd_display
    id: hello_display
    update_interval: never
    rotation: 270
    lambda: |-
      it.fill(Color(255, 255, 255));
      it.print(it.get_width()/2, it.get_height()/2, id(my_font), Color(0, 0, 0), TextAlign::CENTER, "Hello world");
```

See: [`example/hello_world_m5papers3.yaml`](example/hello_world_m5papers3.yaml).

## Upstream dependency strategy (low maintenance)

To minimize maintenance burden while avoiding unpredictable breakage:

- FastEPD is pulled directly from upstream GitHub, **not vendored**.
- The dependency is **pinned** to a known upstream release tag (`1.4.6`) instead of tracking `main`.
- The pinned ref is kept in one obvious tracker file: `components/fastepd_display/upstream_fastepd_ref.txt`.
- A scheduled GitHub Action (`.github/workflows/check-upstream-fastepd.yml`) checks upstream releases weekly and opens an issue when a new release is available.

This provides reproducible builds while still keeping updates visible and easy.

## Limitations

- First milestone is full-refresh only.
- M5 PaperS3 focused; no generic panel selection yet.
- No LVGL integration in this repo yet.
- No touch input integration yet.
- No Home Assistant-specific auth/token/websocket flows (out of scope by design).

## License

MIT for this wrapper repo. FastEPD itself remains licensed by its upstream project.
