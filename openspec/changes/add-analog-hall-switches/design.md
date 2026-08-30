## Context

See `proposal.md` — Why. This document records the technical decisions behind an analog Hall-sensed Clavis revision.

**Current state.** Clavis is a 36-key split (18 per half: 5×3 matrix + 3 thumb). Each half runs an nRF52840 SuperMini (nice!nano v2 compatible) driving a digital 5-column × 4-row diode matrix. `firmware/zmk` carries only a keymap and `.conf`; the kscan definition comes from upstream ZMK's `corne_left`/`corne_right` shields. `config/corne.zmk.yml` exists but is not wired up as a shield. The PCB is generated from `pcb/config.yaml` by ergogen 4.1.0, auto-routed with freerouting, and revisions are versioned as sibling config files.

**Fixed inputs from the switch datasheet** (`pcb/docs/CPG1353C01D01-03.pdf`, Kaihua PG1353C, rev A0 2025-01):

| Property | Value |
| --- | --- |
| Travel | 2.8 ± 0.25 mm |
| Initial force | 30 ± 10 gf, rising to ~50 gf at 2.5 mm, 120 gf hard stop |
| Body | 15×15 mm housing over a 14×14 mm base → standard MX plate cutout |
| Height | ~14.5 mm (Choc ≈ 11.5, MX ≈ 18.5) |
| Stem | MX cross |
| Mounting | Ø5.00 mm centre boss + 2× Ø1.80 mm legs at 10.00 mm pitch — **PCB-mount** |
| Magnet | Ø2.50 mm NdFeB, nickel plated, **N pole down**, offset from the key centreline |
| Spring | stainless, explicitly non-magnetic (无磁) |
| Contacts | none |
| Life | 100,000,000 cycles; −40…+70 °C |

Flux, as published — note the manufacturer specifies it **through the PCB**, which fixes the sensor on the opposite face:

| | at magnet face | through 1.2 mm PCB | through 1.6 mm PCB |
| --- | --- | --- | --- |
| At rest | 160 ± 10 Gs (16 mT) | 120 ± 15 Gs (12 mT) | 100 ± 15 Gs (10 mT) |
| Bottomed out | 2300 ± 100 Gs (230 mT) | 850 ± 80 Gs (85 mT) | 600 ± 80 Gs (60 mT) |
| Swing ratio | 14.4× | 7.08× | 6.00× |

**Hard platform constraints.**
- The nRF52840 SAADC has 8 channels on AIN0–7 (P0.02/03/04/05/28/29/30/31). On nice!nano pin numbering these are pins 14/15/18/19/20/21 — i.e. the pins Clavis already uses as matrix columns. 18 keys per half therefore **cannot** be read without multiplexing.
- One AIN is consumed by the board's battery-sense divider.
- ZMK has no upstream analog kscan driver: only `kscan-gpio-{matrix,direct,charlieplex,mock}`.

## Goals / Non-Goals

**Goals**

- Measure per-key travel as a continuous value and expose an actuation point configured in **millimetres**, not raw ADC counts.
- Per-key calibration that survives reboot, tolerates part-to-part magnet and sensor variation, and self-corrects for temperature drift.
- Keep everything downstream of kscan untouched — Miryoku, home-row mods, combos, layers, and the split transport must not know the switches are analog.
- Keep the wireless build viable: a defensible battery life under an adaptive scan rate, with the power budget as an explicit design parameter rather than an afterthought.
- Leave headroom for rapid trigger and analog axes without another hardware revision.

**Non-Goals**

- Implementing rapid trigger, dynamic keystroke, or analog/pointing output in this change. The data path is designed to carry them; the behaviour is not built here.
- Matching the idle current of the existing digital matrix. Analog sensing is fundamentally more expensive and the trade is accepted.
- Preserving Choc keycap, plate, or case compatibility. This is a separate PCB revision.
- A wired/QMK variant of the analog board.

## Decisions

### D1 — Sensor: DRV5053**OA**

Values below are from the DRV5053 datasheet, `pcb/docs/drv5053.pdf` (TI SLIS153D Rev. D, February 2023).

**The variant is selected by saturation field, not by sensitivity.** The DRV5053 family trades sensitivity against input saturation field `B_SAT`, and the switch demands 10–60 mT (1.6 mm board) or 12–85 mT (1.2 mm board) of usable range. That eliminates every variant but one:

| Variant | Sensitivity (typ) | B_SAT (typ) | Verdict |
| --- | --- | --- | --- |
| **OA** | **−11 mV/mT** | **73 mT** | **Selected — the only variant that spans the travel** |
| PA | −23 mV/mT | 35 mT | Saturates mid-travel |
| RA | −45 mV/mT | 18 mT | Saturates near rest |
| VA | −90 mV/mT | 9 mT | **Saturated at rest** — below the switch's 10 mT resting flux |
| CA | +23 mV/mT | 35 mT | Saturates mid-travel |
| EA | +45 mV/mT | 18 mT | Saturates near rest |

DRV5053**VA** — the part named during early exploration of this change — has `B_SAT = 9 mT`, *below* the switch's at-rest flux. It would read saturated before the key is touched and would never leave saturation. The −11 mV/mT figure that motivated the original choice belongs to the **OA**.

With OA at typical values (`V_Q` = 1.02 V, S = −11 mV/mT) through a 1.6 mm board:

```
  10 mT rest    → 0.91 V
  60 mT bottom  → 0.36 V        B_SAT = 73 mT, so no saturation at typical S
  ─────────────────────
  550 mV over 2.8 mm  =  196 mV/mm
```

*Alternative rejected — a digital Hall switch (DRV5032 family, B_OP ≈ 2–7 mT).* Rejected on numbers, not preference: the switch's at-rest flux is 10–12 mT, already 2–5× above B_OP, so it reads permanently pressed. Making it work would need a ~20–25 mT B_OP part at µA-class supply current, a poorly-populated corner of the market — and it would give a fixed actuation point, defeating the purpose of the change.

**Part-to-part spread is large, and is why calibration is mandatory rather than a refinement.** The OA's sensitivity is specified as **−5 to −17.5 mV/mT** (typ −11) and `V_Q` as **0.9 to 1.15 V** (typ 1.02), across parts and temperature. No fixed threshold survives that spread; see D6.

Worst-case sensitivity also sets the board thickness decision in D10:

| | S = −5 (worst low) | S = −11 (typ) | S = −17.5 (worst high) |
| --- | --- | --- | --- |
| Signal over 2.8 mm | 250 mV (89 mV/mm) | 550 mV (196 mV/mm) | saturates before bottom-out |
| Saturation onset | none | none | 46.9 mT |

**ADC configuration.** Across all tolerance corners the signal window spans roughly 0.2 V to 1.15 V, which fits the nRF52840 SAADC at gain 1/2 with the 0.6 V internal reference (1.2 V full scale, 0.293 mV/LSB). The datasheet's load limits on OUT — `R > 10 kΩ`, `C < 10 nF` — are satisfied by a multiplexer input. The output stage sources only 300 µA, so no resistive divider may be placed on it.

**Output polarity.** The magnet presents its N pole toward the board and the sensor sits on the opposite face, so the package's marked side faces away from the magnet. The expected result is a positive field and therefore a *falling* output as the key is pressed, keeping the signal in the 0.2–1.02 V window assumed above. If the sign proves inverted on the coupon, the output rises toward the 1.8 V rail instead and the only consequence is an ADC gain change to 1/3. The variant choice is unaffected either way. Confirmed in task 1.5.

### D2 — Sensor on the opposite PCB face, no clearance hole

The datasheet quotes flux *through* 1.2 mm and 1.6 mm of PCB, which is the manufacturer stating the intended configuration: switch on one face, sensor on the other, sensing through the board.

This has a valuable side effect for Clavis's `reversible: true` PCB. Switches always mount on the finger-facing side; sensors mount uniformly on the other. Both halves are identical, there is no front/back population decision, and the magnet's N-pole-down orientation relative to the sensor face is preserved when the board is flipped. **Reversibility, which looked like a problem, is free.**

The `PCB开孔图` shows a Ø2.50 feature labelled 磁铁 (magnet) among the drill features. It is read here as a **position marker for the magnet's projection, not a clearance hole** — consistent with flux being specified through solid board, and with not wanting to weaken the PCB. Flagged in Open Questions; confirm against a physical sample before layout.

### D3 — Multiplexed sampling with a single switched sensor rail

```
                        ┌── P-FET load switch ──── VCC to all 18 sensors
           1 GPIO ──────┘

  9 sensors ──┐                                       ┌── AIN_a ──┐
              ├──► 74HC4067 (16:1) ───────────────────┤            │  nRF52840
  9 sensors ──┤                                       │            │   SAADC
              └──► 74HC4067 (16:1) ───────────────────┴── AIN_b ──┘
                          ▲
                     4 shared select lines

  GPIO: 2 ADC + 4 select + 1 rail enable = 7   (9 used today → frees 2)
```

Two 4067s rather than three 4051s: 32 channels for 18 keys leaves headroom for future thumb-cluster or encoder additions, and shares one 4-bit select bus. Multiplexer R_on (~70–100 Ω) is negligible against the SAADC's acquisition time.

*Alternative considered — reuse the existing column/row topology as the multiplexer.* Gate sensor power per column (5 load switches), route each row to its own AIN (4 pins). Same nets as today, same pin count, no multiplexer ICs. This depends on an unverified property: **whether an unpowered DRV5053's output is high-impedance.** If it loads or clamps the shared row line, the scheme collapses. It is cheap to test and materially simpler if it holds, so it is scheduled as an early bench check — but the multiplexer design is the baseline because it depends on no such assumption.

### D4 — Scan rate is the power dial

The sensors dominate the power budget. The datasheet gives `I_CC` = **2.7 mA typ** at 25 °C (3.0 typ / 3.6 max at 125 °C), so 18 sensors ≈ **48.6 mA** if left on. Duty-cycling the whole rail — power on, settle, burst-sample all 18, power off — makes the scan rate the controlling variable.

Sensor settling is not the bottleneck: the datasheet specifies `t_on` = **35 µs typ, 50 µs max** (marketed as "fast power-on") plus an output delay `t_d` of 13 µs typ / 25 µs max, so the rail is valid within ~75 µs worst case. The burst is dominated by ADC time, and therefore by the oversampling factor chosen in D11:

| | burst | 1000 Hz | 500 Hz | 250 Hz | 100 Hz | 10 Hz (idle) |
| --- | --- | --- | --- | --- | --- | --- |
| **1× (no oversampling)** | ~165 µs | 8.0 mA | 4.0 mA | 2.0 mA | 0.8 mA | 0.08 mA |
| **4× oversampling** | ~430 µs | 21 mA | 10.4 mA | 5.2 mA | 2.1 mA | 0.21 mA |

The driver uses an **adaptive rate**: full rate while typing, decaying through intermediate rates after inactivity, down to a slow idle poll. A realistic 8 h typing / 16 h idle day gives **~19 days per charge on a 700 mAh cell** at 1× and **~8 days** at 4×, which is the cost of the resolution decision in D11.

Deep sleep is a genuine inversion of the usual situation: waking on a keypress requires sensors to be powered, so "sleep" here means the slowest poll rate rather than a powered-down rail. The first keypress after a long idle may be delayed by up to one idle scan period; this is accepted.

### D5 — Minimal decoupling, no bulk capacitance on the switched rail

Duty-cycling a rail means re-charging every bypass capacitor on it, every scan. This is not a rounding error:

```
  I_avg = C_total × 3.3 V × f_scan

  18 × 100 nF = 1.8 µF   →  5.9 µC/burst × 1 kHz  =  5.9 mA   ← as much as the sensors
  18 ×  10 nF = 180 nF   →  0.59 µC/burst × 1 kHz =  0.6 mA   ← acceptable
```

Decision: **10 nF per sensor and no bulk capacitor on the switched rail.**

This is corroborated by the datasheet, which specifies 0.01 µF as the **minimum** `V_CC` bypass capacitor and requires no other external components. The power-optimal value and the manufacturer's minimum coincide, so the trade this decision was worried about does not arise: there is no reason to fit more, and fitting less is not allowed. Confirm on the coupon that `t_on` and noise hold at the minimum (task 1.11).

Separately, the datasheet caps capacitance *on the OUT pin* at 10 nF for amplifier stability, and permits an RC filter only as `R ≥ 200 Ω` with `C ≤ 0.1 µF`. Any output filtering adds settling time after power-on and therefore costs power under duty-cycling; D11 uses ADC oversampling instead.

### D6 — Two-point calibration plus physical linearization

The field follows a near-inverse-cube law, so **two calibration points per key recover the whole travel curve** — no per-key lookup table, and no requirement for the user to sweep a switch slowly.

```
  Stored per key:   B_rest, B_bottom
  Known constant:   T = 2.8 mm  (datasheet)

      k    = (B_bottom / B_rest)^(1/3)
      r_b  = T / (k − 1)              effective distance at bottom-out
      r_r  = k · r_b                  effective distance at rest

  Live sample B  →  r = r_b · (B_bottom / B)^(1/3)
                 →  travel_mm = r_r − r
```

One cube root per sample is trivial on a Cortex-M4F with an FPU; a 32-entry interpolated LUT is the fallback if profiling demands it.

**The two calibration points also recover the sensor's own parameters, which is why two are sufficient.** The driver measures voltages, not fields, and per D1 both `V_Q` (0.9–1.15 V) and sensitivity `S` (−5 to −17.5 mV/mT) vary widely between parts. Taking the switch's nominal flux endpoints as known:

```
      V_rest   = V_Q + S · B_rest
      V_bottom = V_Q + S · B_bottom

   →  S   = (V_bottom − V_rest) / (B_bottom − B_rest)
      V_Q = V_rest − S · B_rest
```

Two measured voltages therefore yield both `S` and `V_Q` for that specific part, after which any sampled voltage converts to a field and then to travel. Residual error comes only from the magnet's ±15 % flux tolerance, which is largely a common-mode scale error on that key.

This matters because raw counts are a bad unit for the user-facing parameter: per D1 the signal is roughly **7× more sensitive near bottom-out than near rest**, so "50 % of ADC range" is nowhere near 50 % of travel. Linearizing lets the actuation point be specified in millimetres and mean it.

*Alternative considered — work directly in normalized ADC units (0 = rest, 1000 = bottom).* Simpler and physics-free, but the resulting parameter is not proportional to travel and behaves differently at different actuation depths. Rejected: it pushes the non-linearity onto the user.

The point-dipole law is an approximation for a Ø2.50 mm magnet at 3–6 mm. The planned refinement is to measure the true curve **once** on a coupon, fit a shared correction, and let per-key calibration supply only gain and offset — the standard approach in commercial Hall-effect keyboards.

### D7 — Calibration acquisition, drift tracking, and a display-less UX

```
  Per key, persisted in ZMK settings (NVS):
      rest_ref    u16     continuously auto-tracked
      bottom_ref  u16     learned from observed maxima, or an explicit pass
  36 keys × 4 B = 144 bytes
```

- **`rest_ref` tracking is mandatory, not an optimisation.** NdFeB drifts −0.11 %/°C and the sensor has its own tempco; across a normal room-temperature swing this is enough to shift a shallow actuation point and, untracked, to cause false triggers. When a key has been still for several seconds its `rest_ref` is pulled slowly toward the current reading.
- **`bottom_ref`** is learned from observed maxima during normal use, over a compile-time default so the board is usable on first boot before any calibration exists.
- **Explicit calibration pass** remains available via a keymap behaviour for users who want deterministic values immediately.
- **Feedback on a keyboard with no OLED and no LEDs:** the keyboard types its own status. Entering calibration mode emits a marker, and each key emits a character as it is captured. Costs nothing and works on any host.

### D8 — Validate on a single-key coupon before committing a PCB revision

One PG1353C + one DRV5053OA + a nice!nano, measuring voltage against travel. The DRV5053 datasheet has since answered several of the questions this coupon was scoped to settle — `t_on`, decoupling, board thickness, and the noise budget are now resolved from published figures — so its remaining purpose is narrower but still blocking: confirm the real field-vs-travel curve against the dipole fit, confirm output polarity (D1), confirm the noise figure and the oversampling factor in practice (D11), and answer the unpowered-output question in D3, which the datasheet does not address. Still sequenced first, before any PCB spend.

### D9 — Promote Clavis to a local ZMK shield

The kscan node changes completely and cannot be expressed while consuming upstream `corne_left`/`corne_right`. A local `boards/shields/clavis/` is required regardless of sensing technology; `config/corne.zmk.yml` is the unused stub this replaces.

The analog driver ships as an **out-of-tree Zephyr module** added via `config/west.yml`:

```
  zmk-clavis-analog/
  ├── dts/bindings/   clavis,kscan-analog-hall.yaml
  ├── drivers/kscan/  kscan_analog_hall.c
  │     • DT: io-channels, mux select-gpios, enable-gpio, key→channel map
  │     • adaptive-rate sampling loop
  │     • per-key calibration + linearization
  │     • settings handler "clavis/cal/*"
  └── behaviors:      hall_cal, hall_act
```

Critically, it implements **ZMK's stock kscan API and reports plain press/release**. Layers, home-row mods, combos, and the split transport are untouched, and analog values never cross the split link. `firmware/zmk-timeless/` is unaffected.

### D10 — Board stack-up: 4 layers, **1.6 mm**

18 analog nets per half plus multiplexers will not route comfortably on 2 layers with freerouting, and the analog path wants a ground pour and guarding away from the BLE antenna region. Move to 4 layers.

Thickness was previously deferred, and 1.2 mm looked attractive on nominal signal. The sensitivity tolerance in D1 decides it the other way:

| | 1.2 mm | 1.6 mm |
| --- | --- | --- |
| Flux range | 12 → 85 mT | 10 → 60 mT |
| Signal at typical S | 254 mV/mm | 196 mV/mm |
| Saturation onset, worst-case S = −17.5 | **2.13 mm — inside the actuation band** | 2.51 mm — outside it |
| Stiffness | more flex, felt directly through **PCB-mount** switches | stiffer |

A worst-case-sensitivity part on a 1.2 mm board saturates at 2.13 mm of travel, inside the 0.2–2.5 mm actuation band the sensing spec requires. On 1.6 mm the same part saturates at 2.51 mm, just outside it. **Decision: 1.6 mm**, trading nominal resolution for tolerance margin and board stiffness.

### D11 — Meet the resolution requirement with ADC oversampling, not output filtering

The sensing spec requires 0.05 mm resolution across the 0.2–2.5 mm actuation band, and the datasheet's noise figure does not meet that unaided at the shallow end. Input-referred noise is **0.49 mT p-p typ** (0.79 max) against a field gradient that varies roughly 7:1 across the travel:

| travel | field gradient | noise as travel (1×) |
| --- | --- | --- |
| 0.2 mm | ~0.59 mT / 0.1 mm | **~0.083 mm — fails** |
| 1.3 mm | ~1.3 mT / 0.1 mm | ~0.038 mm |
| 2.5 mm | ~4.0 mT / 0.1 mm | ~0.012 mm |

Noise is broadband — the device's bandwidth is 20 kHz — so averaging is effective. **4× hardware oversampling on the SAADC** cuts it by about half, bringing the shallow end to ~0.042 mm and meeting the requirement across the whole band.

*Alternative rejected — an output RC filter.* The datasheet permits `R ≥ 200 Ω` with `C ≤ 0.1 µF`, but any output filter adds settling time after each power-on, and under duty-cycling that time is paid on every scan. Oversampling spends the same budget in a form that is adjustable in firmware rather than fixed in copper.

Oversampling is not free: per D4 it roughly halves battery life. The driver therefore applies it **adaptively** — 1× for the fast scan that detects movement, 4× only for keys near their configured threshold, where the resolution actually matters. This keeps the common case at the 1× power figure. The achievable factor is confirmed on the coupon (task 1.9).

## Risks / Trade-offs

- ~~**`t_on` is larger than assumed**~~ → **Resolved by the datasheet.** `t_on` = 35 µs typ / 50 µs max, plus 25 µs max output delay. Sensor settling is not a constraint; the burst is dominated by ADC time (D4, D11).
- **The magnet is not on the key centreline, and the exact offset is read from a drawing** → a mispositioned sensor wastes a full PCB revision. *Mitigation:* extract exact XY from the drawing or a DXF/STEP, and verify against a physical switch before layout; add the sensor position to the coupon so it is confirmed empirically.
- **The published flux is a two-point specification with ±15 % tolerance; the real curve may deviate from the dipole fit** → linearized millimetres become inaccurate. *Mitigation:* the coupon measures the true curve and yields a shared correction; per-key calibration then only supplies gain and offset.
- ~~**D5 conflicts with `t_on` and analog noise**~~ → **Resolved by the datasheet.** 10 nF is simultaneously the power-optimal value and the manufacturer's stated minimum, and no other external component is required. Noise is handled by oversampling (D11) rather than capacitance.
- **Sensitivity spread of −5 to −17.5 mV/mT is 3.5:1** → a worst-case-low part yields only 89 mV/mm, less than half the typical signal, reducing effective resolution on that key. *Mitigation:* per-key calibration (D6) absorbs the spread for correctness; the resolution margin from D11's oversampling covers the reduced signal. Keys landing at the extreme are detectable via the invalid-calibration check in D7.
- **Oversampling roughly halves battery life** → the resolution requirement and the power budget are in direct tension. *Mitigation:* D11's adaptive oversampling confines the cost to keys near their threshold, keeping the common case at the 1× figure.
- **Battery life drops from months to ~8–19 days** → a real regression for a wireless keyboard. *Mitigation:* accepted deliberately; adaptive scan rate, adaptive oversampling, a larger cell, and the Choc revision remaining buildable for users who prefer runtime over configurability.
- **Temperature drift causing false triggers** → the most likely field failure mode. *Mitigation:* continuous `rest_ref` tracking (D7) is treated as a requirement, not a refinement.
- **Custom ZMK driver has no upstream equivalent to lean on** → maintenance burden as ZMK's kscan API evolves. *Mitigation:* target the stock kscan API and keep all analog logic inside the module, so ZMK upgrades touch one boundary.
- **Mechanical scope is larger than the electronics.** Case, plate, and keycaps all change, and the CNC `.tap` files in `3d/` are hand-maintained. *Mitigation:* plates regenerate from ergogen; the case work is sequenced after the coupon confirms the electrical design.

## Migration Plan

The magnetic board is a **new PCB revision alongside the existing Choc revisions**, not a replacement, following the established `config-*.yaml` pattern. The Choc build and its ZMK targets stay buildable throughout, so rollback is simply continuing to build the existing revision. No user migration exists — the two variants are different physical keyboards.

Sequencing: coupon measurement (D8) → resolve D3 → ergogen footprints and PCB revision → firmware module and shield → case and plate.

## Open Questions

- ~~DRV5053 quiescent voltage and output swing limits~~ — **resolved** from `pcb/docs/drv5053.pdf`: `V_Q` 0.9/1.02/1.15 V, `V_OUT` saturation 0.2 V min to 1.8 V max, recommended output range 0–2 V. Folded into D1.
- ~~Final board thickness~~ — **resolved to 1.6 mm** in D10, decided by worst-case sensitivity rather than nominal signal.
- Whether an unpowered DRV5053's output is high-impedance (D3). The datasheet states only that the device is inactive below 2.5 V `V_CC` and does not specify the output state. This decides between the multiplexer baseline and the simpler gated-column alternative, so it is measured on the coupon; the baseline does not depend on the answer.
- Whether the Ø2.50 feature in the switch's PCB drill diagram is a clearance hole or a position marker (D2). Read here as a marker; confirm against a physical sample. Affects the footprint but not the architecture.
- Which keycap profile suits a 14.5 mm MX-stem switch on a splayed 36-key layout. Aesthetic, and resolvable after the board works.
