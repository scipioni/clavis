## Why

Clavis today uses Kailh Choc mechanical switches in a digital diode matrix: actuation is fixed by the switch's mechanical contact point, keys wear out, and contacts bounce (currently masked with a 5 ms debounce). Kailh's PG1353C magnetic switch (part `CPG1353C01D01-03`, datasheet in `pcb/docs/`) has no electrical contacts at all — a Ø2.50 mm NdFeB magnet in the slider, rated 100,000,000 cycles — and its published flux curve is a good match for an analog Hall sensor read through the PCB.

Sensing that magnet with a linear Hall sensor and an ADC turns every key into a continuous position measurement instead of a binary contact. That unlocks a **user-configurable, per-key-calibrated actuation point** (and, later, rapid trigger and analog axes) — the actual motivation for this change. It also eliminates contact bounce entirely, allowing debounce to drop toward zero.

## What Changes

- **BREAKING — switch family.** Replace Kailh Choc (3.0 mm travel, Choc stem, hotswap socket) with Kailh PG1353C (2.8 mm travel, MX cross stem, PCB-mount legs). This invalidates the existing keycaps, plate cutouts, and case geometry. Delivered as a **new PCB revision alongside the existing one**, not a replacement — `pcb/config.yaml` revisions are already versioned (`config-0_2/1_0/1_1`), and the Choc build stays buildable.
- **BREAKING — key sensing.** Remove the 36 diodes and 36 hotswap sockets and the digital row/column matrix. Add 36 DRV5053OA linear Hall sensors on the PCB face opposite the switches, read through two 16:1 analog multiplexers per half into the nRF52840 SAADC, with the whole sensor rail on a GPIO-controlled load switch.
- **New ergogen footprint** for the PG1353C (14×14 mm plate cutout, Ø5.00 centre boss, 2× Ø1.80 legs at 10.00 mm pitch, no electrical pads) plus a Hall-sensor footprint placed at the magnet's position on the opposite layer.
- **New ZMK shield.** Promote Clavis from borrowing upstream `corne_left`/`corne_right` to a local `boards/shields/clavis/`. The kscan node changes completely and cannot be expressed while consuming the upstream Corne shield.
- **New out-of-tree ZMK module** implementing an analog Hall kscan driver: adaptive-rate ADC sampling, per-key two-point calibration, linearization of the sensor's non-linear field-vs-travel response into millimetres, and configurable actuation/release thresholds persisted in ZMK settings.
- **New calibration workflow** for a display-less keyboard, including a keymap-invocable calibration behaviour and continuous rest-reference tracking to compensate magnet and sensor temperature drift.
- **Case and plate regeneration** for the taller (~14.5 mm vs ~11.5 mm) MX-stem switch: `top_plate`/`bottom_plate` regenerate from ergogen, but the Blender models and CNC `.tap` files in `3d/` need new pocket depths and switch openings.
- **Battery sizing.** The 140 mAh cell in the BOM was sized for a matrix that costs almost nothing to idle. Analog sensing needs a larger cell.
- **Non-goal for this change:** rapid trigger, dynamic keystroke, and analog-axis/pointing output. The driver is designed so they can be added later without further hardware changes, but this change delivers only a configurable, calibrated actuation point.

## Capabilities

### New Capabilities

- `keyboard/analog-key-sensing`: How a key's physical position is measured — sensor and magnet geometry, the flux budget the design must satisfy, the sampling architecture, and the required accuracy and scan-rate behaviour.
- `keyboard/key-calibration`: Per-key calibration of the raw sensor signal into physical travel — what is stored, how it is acquired and refreshed, how drift is compensated, and how the keyboard behaves before calibration exists.
- `keyboard/actuation-config`: The user-facing actuation contract — actuation and release points expressed in millimetres, their valid ranges, hysteresis guarantees, how they are configured and persisted, and defaults.
- `hardware/magnetic-switch-pcb`: The PCB-level contract for a magnetic-switch Clavis revision — switch footprint, sensor placement relative to the magnet, board stack-up, power gating, and the constraints the analog signal path imposes on layout.

### Modified Capabilities

None. `openspec/specs/` is currently empty — this change introduces the project's first specs, so there are no existing requirements to modify.

## Impact

**Hardware / PCB**
- `pcb/config.yaml` → new sibling revision for the magnetic variant; existing Choc revisions untouched.
- `pcb/footprints/` → new `magnetic_pg1353c.js` and Hall-sensor footprint; `choc`/`diode` footprints no longer used by the new revision.
- Likely move from 2-layer to 4-layer with a ground pour: 18 analog nets per half plus multiplexers will not route comfortably on 2 layers, and the analog path wants guarding away from the BLE antenna region.
- BOM: −36 hotswap sockets, −36 diodes; +36 DRV5053OA, +4 multiplexers, +2 load switches, +bypass caps, +36 PG1353C switches, +MX keycaps, +larger LiPo.

**Firmware**
- `firmware/zmk/build.yaml` → new shield target; existing Corne targets remain for the Choc build.
- New `boards/shields/clavis/` (shield definition, overlays, `Kconfig.shield`). `config/corne.zmk.yml` is the unused stub this replaces.
- New out-of-tree Zephyr module (Hall kscan driver, calibration settings handler, calibration/actuation behaviours), added via `config/west.yml`.
- `config/corne.conf` → debounce can drop toward 0 ms; sleep and idle timeouts need revisiting against the new power profile.
- **Unaffected:** the Miryoku layout, home-row mods, combos, layers, and the ZMK split transport. The driver implements ZMK's stock kscan API and reports plain press/release events, so nothing downstream observes that the switches are analog. `firmware/zmk-timeless/` is likewise unaffected.

**Mechanical**
- `3d/` Blender sources, STLs, and CNC `.tap` files need new switch openings and pocket depths.
- Keycaps change from MBK Choc low-profile to MX-stem; Clavis stops being a low-profile board in this revision.

**Power**
- Battery life moves from months to an estimated ~8–19 days per charge on a 700 mAh cell, depending on how much ADC oversampling the resolution requirement demands. This is a deliberate, accepted trade for configurable actuation.

**Key open risks** (detailed and tracked in `design.md`)
- The DRV5053's power-on settling time (`t_on`) governs the achievable scan rate and therefore the entire power budget.
- The exact magnet position within the switch footprint must be read off the drawing, not assumed to be on the key centreline.
- The published field-vs-travel relationship is a two-point specification; the real curve must be measured on a single-key test coupon before committing to a PCB revision.
