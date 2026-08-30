## 1. Bench Validation (single-key coupon)

Confirms the datasheet-derived decisions on real hardware and resolves the questions the datasheets do not answer — D3 (unpowered output impedance), the true field-vs-travel curve, output polarity, and the D11 oversampling factor. No layout work begins until this group is complete.

- [ ] 1.1 Source parts for the coupon (PG1353C switch, DRV5053**OA** — not VA, see design D1 — 74HC4067, nice!nano or SuperMini, 1.6 mm FR4 offcut) and verify each part matches the datasheet markings before use
- [ ] 1.2 Extract the exact magnet XY offset, centre boss diameter, and leg pitch from the switch drawing or a DXF/STEP, and verify the extracted dimensions against a physical switch with calipers
- [ ] 1.3 Determine whether the Ø2.50 feature in the switch's PCB drill diagram is a clearance hole or a magnet position marker, and verify by fitting a physical switch against a test board — record the answer in design.md Open Questions
- [ ] 1.4 Build the coupon: one switch over one sensor on the opposite board face at the offset from 1.2, and verify the sensor reads a stable voltage with the key at rest
- [ ] 1.5 Measure voltage against travel over the full 2.8 mm on 1.6 mm stock, and verify the curve is monotonic, its endpoints fall within the datasheet's published flux tolerance, and the output polarity falls as the key is pressed as predicted in design D1 — if it rises instead, record the ADC gain change to 1/3
- [ ] 1.6 Fit the measured curve against the inverse-cube model from design D6 and verify the residual error stays within the 0.05 mm resolution requirement across the 0.2–2.5 mm actuation band; record the shared correction curve if the fit needs one
- [ ] 1.7 Verify the datasheet's `t_on` (35 µs typ / 50 µs max) and output delay on the bench, and confirm the rail-settle budget in design D4 holds — revise the scan-rate table only if measurement disagrees with the datasheet
- [ ] 1.8 Measure sensor supply current and verify it against the datasheet's 2.7 mA typical used in the D4 power budget
- [ ] 1.9 Measure output noise at 1×, 4×, and 8× SAADC oversampling, and verify that the factor chosen in design D11 meets the 0.05 mm resolution requirement at 0.2 mm travel — the shallowest and hardest point in the band
- [ ] 1.10 Test whether an unpowered DRV5053's output is high-impedance by measuring its loading on a shared pulled-up net, and verify the result decides design D3 between the multiplexer baseline and the gated-column alternative — this is the only D8 question the datasheet does not answer
- [ ] 1.11 Verify the 10 nF datasheet-minimum bypass capacitor is sufficient by confirming `t_on` and noise are unchanged against a larger value, and that the rail recharge current matches the D5 calculation
- [ ] 1.12 Measure the coupon sensor's actual sensitivity and quiescent voltage, and verify the two-point calibration in design D6 recovers both to within the accuracy needed for 0.05 mm travel reporting

## 2. Ergogen Footprints and PCB Revision

- [ ] 2.1 Write `pcb/footprints/magnetic_pg1353c.js` with the 14×14 mm plate cutout, centre boss hole, and two leg holes at the verified pitch and no electrical pads, and verify the generated KiCad footprint matches the switch drawing dimensions
- [ ] 2.2 Write the Hall sensor footprint placed at the magnet offset on the opposite board face, and verify in KiCad that sensor and magnet positions coincide when the two footprints are placed on the same key
- [ ] 2.3 Create the new revision config (1.6 mm, 4-layer per design D10) as a sibling of `pcb/config.yaml`, reusing the existing key layout, and verify `make kicad` generates board and plate files without touching the existing revisions
- [ ] 2.4 Replace the choc/diode footprints with the magnetic switch and sensor footprints in the new revision and verify the netlist contains no diode or switch-pin nets
- [ ] 2.5 Add two 16:1 analog multiplexers per half with a shared 4-bit select bus, and verify the generated netlist assigns all 18 sensor outputs to multiplexer channels with no channel collisions
- [ ] 2.6 Add the P-FET sensor rail load switch and per-sensor decoupling at the value chosen in 1.11, and verify the rail net reaches every sensor and no bulk capacitor is present on it
- [ ] 2.7 Assign controller pins (2 analog inputs, 4 select lines, 1 rail enable) and verify every analog input lands on an analog-capable pin of the module and none conflicts with battery sense
- [ ] 2.8 Reconfigure the board for 4 layers with a ground pour and verify freerouting completes with all analog nets routed and none crossing the wireless module's antenna keepout
- [ ] 2.9 Verify the reversible-board requirement by generating both halves from the single design and confirming identical component placement and sensor-to-magnet geometry
- [ ] 2.10 Generate gerbers for the new revision and verify DRC passes and the existing revisions still generate unchanged output

## 3. PCB Fabrication and Bring-Up

- [ ] 3.1 Fabricate and assemble one half of the new revision, and verify continuity of the sensor rail, the multiplexer select bus, and both analog inputs
- [ ] 3.2 Verify each of the 18 sensors reads a plausible at-rest voltage matching the coupon measurement from 1.5
- [ ] 3.3 Verify every key produces the expected voltage swing when pressed, and that no key is mechanically or magnetically coupled to a neighbour
- [ ] 3.4 Measure noise on a stationary key while the radio is transmitting and verify it stays within the 0.05 mm resolution limit required by the sensing spec

## 4. ZMK Shield Promotion

- [ ] 4.1 Create `firmware/zmk/boards/shields/clavis/` with shield definition, `Kconfig.shield`, and left/right overlays, replacing the unused `config/corne.zmk.yml` stub, and verify the shield is discovered by the ZMK build
- [ ] 4.2 Point `build.yaml` at the new shield targets while retaining the existing Corne targets for the mechanical build, and verify both build successfully
- [ ] 4.3 Verify the existing keymap, Miryoku includes, and `firmware/zmk-timeless/` build unchanged against the new shield

## 5. Analog Sensing Driver

Implements `keyboard/analog-key-sensing`.

- [ ] 5.1 Create the out-of-tree Zephyr module skeleton, register it in `config/west.yml`, and verify a ZMK build picks up the module and its bindings
- [ ] 5.2 Define the devicetree binding (analog channels, multiplexer select GPIOs, rail enable GPIO, key-to-channel map) and verify a build fails cleanly with a helpful message when a required property is missing
- [ ] 5.3 Implement multiplexer channel selection and ADC acquisition of all 18 channels in one burst, and verify on hardware that each key maps to the correct channel by pressing keys one at a time
- [ ] 5.3a Implement adaptive SAADC oversampling per design D11 (1× for movement detection, higher only for keys near their threshold) and verify the 0.05 mm resolution requirement is met at 0.2 mm travel without applying the higher factor to every key
- [ ] 5.4 Implement rail power gating around each sampling burst with the settling delay measured in 1.7, and verify with a scope that the rail is de-energised between bursts
- [ ] 5.5 Implement the linearization from design D6 — two-point calibration solving for the sensor's own quiescent voltage and sensitivity, then field to travel in millimetres, plus the correction curve from 1.6 — and verify reported travel matches a measured physical displacement within 0.05 mm
- [ ] 5.6 Verify reported travel is monotonic across a slow full-travel press and release, per the sensing spec's monotonicity scenarios
- [ ] 5.7 Implement saturation clamping so a key pressed into the saturated region stays pressed with non-decreasing travel, and verify by bottoming out a key hard
- [ ] 5.8 Implement the ZMK kscan API surface reporting press/release only, and verify the existing keymap types correctly with layers, home-row mods, and combos intact
- [ ] 5.9 Verify no continuous position data crosses the split link by confirming the peripheral half sends only key events

## 6. Calibration

Implements `keyboard/key-calibration`.

- [ ] 6.1 Define the persisted calibration record with a format version and implement load/store via ZMK settings, and verify calibration survives a power cycle
- [ ] 6.2 Verify an unrecognised stored format is discarded in favour of defaults and the keyboard remains usable
- [ ] 6.3 Implement default calibration derived from the datasheet flux figures and verify a freshly flashed, uncalibrated keyboard types correctly on every key
- [ ] 6.4 Implement continuous rest-reference tracking gated on the key being stationary and at rest, and verify a key held depressed for a long period does not corrupt its rest reference
- [ ] 6.5 Verify rest tracking prevents false triggers across the operating temperature range by heating and cooling an assembled half with no key touched
- [ ] 6.6 Implement bottom-out reference learning bounded to a plausible range, and verify an implausible reading is not adopted
- [ ] 6.7 Implement invalid-calibration detection for a missing magnet or dead sensor, and verify such a key is not reported as continuously pressed while other keys keep working
- [ ] 6.8 Implement the `hall_cal` keymap behaviour with enter, capture, exit, and abort, and verify aborting leaves prior calibration unchanged
- [ ] 6.9 Implement calibration feedback as keyboard output to the host, and verify entry, per-key capture, and exit are all observable in a text editor
- [ ] 6.10 Implement calibration reset and verify it restores defaults immediately without reflashing

## 7. Actuation Configuration

Implements `keyboard/actuation-config`.

- [ ] 7.1 Implement actuation and release points in millimetres with the press/release state machine, and verify a key actuates at the configured depth and does not actuate above it
- [ ] 7.2 Implement minimum hysteresis enforcement and verify a key held motionless at the actuation point produces at most one state change per second
- [ ] 7.3 Implement bounds validation with top and bottom deadzones, and verify out-of-range values are clamped and a key at rest is never reported pressed
- [ ] 7.4 Implement devicetree defaults and verify they apply on first boot with no stored configuration
- [ ] 7.5 Implement the `hall_act` keymap behaviour for runtime adjustment, and verify changes take effect on the next press and stop at the bounds
- [ ] 7.6 Implement global configuration with optional per-key override and verify an overridden key ignores a global change
- [ ] 7.7 Implement persistence of actuation configuration and verify it survives a power cycle
- [ ] 7.8 Verify actuation configuration and calibration reset independently, per the spec's independence scenario

## 8. Power and Scan Rate

- [ ] 8.1 Implement the adaptive scan-rate state machine (active, decaying, idle) and verify with a scope that the burst interval changes with typing activity
- [ ] 8.2 Verify the rate returns to full on the first key movement and that the keypress triggering the return is still reported
- [ ] 8.3 Measure average current at each scan rate on assembled hardware and verify it against the design D4 power table; update the table with measured figures
- [ ] 8.4 Verify a keypress during idle sampling is never dropped, only delayed, by repeated cold-start presses after long idle periods
- [ ] 8.5 Set debounce to zero in the shield configuration and verify a single slow press across the threshold produces exactly one press event
- [ ] 8.6 Revisit sleep and idle timeouts in the shield `.conf` against the measured power profile and verify battery life over a multi-day typing run
- [ ] 8.7 Update the BOM battery selection to a larger cell sized from the 8.3 measurements and verify it fits the case

## 9. Mechanical

- [ ] 9.1 Regenerate `top_plate` and `bottom_plate` from the new revision and verify the cutouts accept the switch and retain it
- [ ] 9.2 Update the Blender case sources for the ~14.5 mm switch height and verify the assembled case closes with switches and keycaps fitted
- [ ] 9.3 Update the CNC `.tap` files for the new pocket depths and switch openings, and verify against a test cut or a toolpath simulation
- [ ] 9.4 Select and fit MX-stem keycaps for the splayed 36-key layout and verify no collisions between adjacent keycaps at full travel

## 10. Integration and Documentation

- [ ] 10.1 Assemble both halves and verify split operation, pairing, and the full Miryoku layout behave as they do on the mechanical build
- [ ] 10.2 Run a full typing session and verify no false triggers, no missed keys, and no chatter across all 36 keys
- [ ] 10.3 Update `pcb/README.md` with the new revision's BOM and build steps and verify the documented commands reproduce the gerbers from a clean checkout
- [ ] 10.4 Document the calibration and actuation-adjustment procedure in `firmware/zmk/README.md` and verify a reader can calibrate a board from the instructions alone
- [ ] 10.5 Update the root `README.md` to describe both switch variants and verify all cross-links resolve
