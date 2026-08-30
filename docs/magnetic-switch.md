# Magnetic Switch for Clavis — Feasibility

Assessment of replacing Clavis's Kailh Choc mechanical switches with **Kailh PG1353C** magnetic switches sensed by **TI DRV5053OA** analog Hall sensors, giving a user-configurable, per-key-calibrated actuation point.

**Sources:** `pcb/docs/CPG1353C01D01-03.pdf` (Kaihua PG1353C, rev A0 2025-01) · `pcb/docs/drv5053.pdf` (TI SLIS153D rev D, 2023-02)
**Full detail:** `openspec/changes/add-analog-hall-switches/`

---

## Verdict

**Feasible. Proceed — but build the single-key test coupon before spending anything on a PCB.**

The electrical design is sound and every hard number now comes from a datasheet rather than an estimate. Sensor selection, board stack-up, sampling architecture, and the calibration model are all settled. The remaining risk is concentrated in one bench session.

Two caveats worth accepting deliberately:

- **Battery life drops from months to ~1–3 weeks.** This is inherent to analog Hall sensing, not a flaw in the design. It is the price of a configurable actuation point.
- **The mechanical scope is larger than the electronics.** New keycaps, new plate, new case. Clavis stops being a low-profile board in this revision.

Ship it as a **new PCB revision alongside** the Choc board, not a replacement.

---

## Key parameters

| | |
| --- | --- |
| Switch | Kailh PG1353C, 2.8 mm travel, 30 gf initial, MX cross stem, PCB-mount |
| Magnet | Ø2.50 mm NdFeB, N pole down, **offset from the key centreline** |
| Flux through 1.6 mm PCB | 10 mT at rest → 60 mT bottomed (6.0× swing) |
| Sensor | DRV5053**OA**, −11 mV/mT, B_SAT 73 mT, `t_on` 35 µs, I_CC 2.7 mA |
| Signal | 0.91 V → 0.36 V, i.e. 550 mV over 2.8 mm (196 mV/mm) |
| Board | 1.6 mm, 4-layer with ground pour |
| Sampling | 2× 74HC4067 multiplexers + switched sensor rail → nRF52840 SAADC |
| Pins | 2 analog + 4 select + 1 rail enable = 7 (Clavis uses 9 today) |
| Life | 100,000,000 cycles (vs ~50 M typical mechanical) |

---

## Benefits

**Configurable actuation point.** The reason to do this at all. Actuation and release depths set in millimetres, per key or globally, adjustable from the keymap and persisted across reboots.

**No contact bounce, ever.** There are no electrical contacts. Debounce drops from the current 5 ms to zero, so key latency may end up *lower* than the mechanical board despite a slower scan rate.

**Sealed and effectively unwearing.** 100 M cycles rated, nothing to oxidise or fatigue, and the switch's spring is explicitly non-magnetic so it does not distort the field.

**Simpler board per key.** 36 diodes and 36 hotswap sockets are deleted. Sensor isolation replaces diode isolation.

**Reversibility comes free.** The switch manufacturer specifies flux *through* the PCB, so the sensor mounts on the opposite face. Switches on top, sensors underneath, both halves identical — no front/back population choice, no polarity flip.

**Frees two GPIO** and leaves 14 spare multiplexer channels.

**Headroom for more.** The same hardware supports rapid trigger, dynamic keystroke, and analog axes later, with no further board revision. `CONFIG_ZMK_POINTING` is already enabled.

**Nothing downstream changes.** The driver implements ZMK's stock kscan API and emits plain press/release, so Miryoku, home-row mods, combos, layers, and the split transport are untouched. Analog values never cross the split link.

---

## Critical issues

### 1. Sensor selection is unforgiving — two obvious parts are both wrong

The switch's magnet is strong, and *at rest* it already presents 10 mT. That single fact eliminates most of the obvious candidates:

```
  DRV5032 (digital Hall switch)   B_OP ≈ 2–7 mT   →  reads PRESSED permanently
  DRV5053VA (analog, −90 mV/mT)   B_SAT  = 9 mT   →  SATURATED at rest
  DRV5053OA (analog, −11 mV/mT)   B_SAT  = 73 mT  →  ✓ the only viable variant
```

Both failure modes are silent-looking on a datasheet skim. **DRV5053OA is the only DRV5053 variant that spans this switch's travel.**

### 2. Battery life regression

Sensors draw 2.7 mA each; 18 per half is ~48.6 mA if left powered. Duty-cycling the whole rail makes scan rate the power dial:

| scan rate | 1× oversampling | 4× oversampling |
| --- | --- | --- |
| 1000 Hz | 8.0 mA | 21 mA |
| 500 Hz | 4.0 mA | 10.4 mA |
| 100 Hz | 0.8 mA | 2.1 mA |
| 10 Hz (idle) | 0.08 mA | 0.21 mA |

With an adaptive rate, a realistic day gives **~19 days at 1× or ~8 days at 4×** on a 700 mAh cell. The 140 mAh cell in the current BOM is not viable.

Deep sleep inverts: waking on a keypress requires powered sensors, so "sleep" means the slowest poll rate, not a dead rail.

### 3. Resolution and power are in direct tension

Sensor noise is 0.49 mT p-p against a field gradient that varies ~7:1 across the travel:

```
  at 0.2 mm travel  →  ~0.083 mm of noise   ✗ misses the 0.05 mm target
  at 1.3 mm         →  ~0.038 mm            ✓
  at 2.5 mm         →  ~0.012 mm            ✓
```

4× ADC oversampling fixes the shallow end but roughly halves battery life. Mitigation is adaptive oversampling — 1× to detect movement, higher only for keys near their threshold.

### 4. Per-key calibration is mandatory, not a refinement

The DRV5053OA's sensitivity is specified **−5 to −17.5 mV/mT** (3.5:1 spread) with quiescent voltage 0.9–1.15 V. No fixed threshold survives that. Add the magnet's ±15 % flux tolerance and NdFeB's −0.11 %/°C drift and continuous rest-reference tracking becomes a requirement too — untracked drift is the most likely field failure mode.

The saving grace: because the field follows a near-inverse-cube law, **two calibration points per key recover both the sensor's parameters and the full travel curve**. No per-key lookup tables, no slow sweep by the user.

### 5. Mechanical scope exceeds electrical scope

```
  Choc (today)                 PG1353C
  ───────────                  ───────
  3.0 mm travel                2.8 mm            ≈ same, fine
  ~11.5 mm tall                ~14.5 mm          case grows ~3 mm
  Choc stem / MBK caps         MX cross stem     ✗ new keycaps
  Choc plate cutout            14×14 MX          ✗ regenerate plate
  socket + diode               boss + 2 legs     ✗ new ergogen footprint
```

Plates regenerate from ergogen almost free. The Blender cases and hand-maintained CNC `.tap` files in `3d/` do not.

### 6. Firmware has no upstream equivalent

ZMK ships no analog kscan driver. This needs an out-of-tree Zephyr module: multiplexed ADC sampling, rail gating, adaptive scan rate, linearization, per-key calibration with settings persistence, and a calibration UX for a keyboard with **no display and no LEDs** (planned solution: the keyboard types its own status to the host).

Clavis must also be promoted to a local ZMK shield — it currently borrows upstream `corne_left`/`corne_right`, which cannot express the new kscan node. That work is worth doing regardless.

### 7. One question remains genuinely open

**Is an unpowered DRV5053's output high-impedance?** TI documents that the device is inactive below 2.5 V but not what OUT does. If it is high-Z, the multiplexers disappear entirely and the existing column/row topology works as-is with power gating — a materially simpler board. The baseline design does not depend on the answer, but the answer is worth having.

---

## Before committing to a PCB

One switch, one sensor, one nice!nano — an evening's work that de-risks everything downstream:

1. Confirm the real voltage-vs-travel curve against the inverse-cube model
2. Confirm output polarity (predicted falling; if rising, ADC gain changes to 1/3)
3. Determine the oversampling factor needed at 0.2 mm travel
4. Answer the unpowered-output question above
5. **Verify the magnet's exact XY offset against a physical switch** — it is not on the key centreline, and getting it wrong wastes a full board revision
