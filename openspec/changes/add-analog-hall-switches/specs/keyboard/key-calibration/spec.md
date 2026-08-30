## Purpose

Defines how each key's raw sensor signal is mapped to physical travel: what calibration data is held, how it is acquired and kept current against drift, and how the keyboard behaves before or when calibration is absent or invalid.

## ADDED Requirements

### Requirement: Per-key calibration data

The keyboard SHALL maintain calibration data independently for every key, sufficient to convert that key's raw sensor reading into physical travel.

Calibration data SHALL account for per-key variation in magnet strength, sensor sensitivity and offset, and mechanical placement, so that two keys given the same actuation point actuate at the same physical depth.

#### Scenario: Keys with differing raw signals behave identically

- **WHEN** two calibrated keys with measurably different raw signal ranges are configured with the same actuation point
- **THEN** both actuate at the same physical depth within the measurement tolerance

### Requirement: Calibration persists across power loss

Calibration data SHALL be stored in non-volatile memory and SHALL be restored on startup.

Calibration SHALL survive a reboot, a battery disconnection, and a firmware restart. It MAY be discarded by a firmware update that changes the calibration data format, in which case the keyboard SHALL fall back to defaults rather than using incompatible stored data.

#### Scenario: Behaviour is unchanged after reboot

- **WHEN** a calibrated keyboard is powered off and back on
- **THEN** every key actuates at the same physical depth as before the power cycle

#### Scenario: Incompatible stored calibration is rejected

- **WHEN** the keyboard starts and finds stored calibration data in an unrecognised format
- **THEN** it discards that data, falls back to default calibration, and remains usable

### Requirement: Usable without prior calibration

The keyboard SHALL ship with a default calibration derived from the switch's published specification and SHALL be fully usable for typing before any calibration has been performed.

#### Scenario: Freshly flashed keyboard types correctly

- **WHEN** firmware is flashed to a keyboard with no stored calibration
- **THEN** every key registers presses and releases at approximately the configured actuation point

### Requirement: User-invocable calibration procedure

The keyboard SHALL provide a calibration mode that the user can enter and exit from the keymap without external tools.

While in calibration mode the keyboard SHALL capture each key's rest and bottom-out references as that key is pressed fully and released, and SHALL persist them on exit.

Calibration mode SHALL be abortable, leaving previously stored calibration unchanged.

#### Scenario: Completing a calibration pass

- **WHEN** the user enters calibration mode and presses every key fully once
- **THEN** each key's rest and bottom-out references are captured and persisted, and the keyboard returns to normal operation

#### Scenario: Aborting calibration

- **WHEN** the user enters calibration mode, calibrates some keys, and then aborts
- **THEN** the previously stored calibration is retained unchanged for all keys

### Requirement: Calibration feedback without a display

The keyboard has no display and no indicator LEDs. Calibration mode SHALL therefore communicate its state to the user through keyboard output to the host, indicating at minimum that calibration mode has been entered, that a given key has been captured, and that calibration mode has ended.

#### Scenario: User observes calibration progress

- **WHEN** the user enters calibration mode with a text editor focused on the host
- **THEN** the keyboard emits output indicating entry into calibration mode, and further output as each key is captured

### Requirement: Continuous rest-reference tracking

The keyboard SHALL continuously track each key's rest reference during normal operation to compensate for drift in magnet strength and sensor output caused by temperature change.

Rest-reference tracking SHALL only occur while a key is stationary and at rest. A key that is held depressed SHALL NOT have its rest reference adapted toward the pressed value.

Tracking SHALL be gradual, such that a genuine key movement is never absorbed as drift.

#### Scenario: Ambient temperature change does not cause false triggers

- **WHEN** the keyboard's ambient temperature changes across its normal operating range with no key touched
- **THEN** no key reports a press, and each key's reported travel remains at 0.0 mm within tolerance

#### Scenario: Held key does not corrupt its rest reference

- **WHEN** a key is held depressed continuously for an extended period and then released
- **THEN** the key's rest reference is unchanged and the key reports 0.0 mm travel on release

### Requirement: Bottom-out reference learning

The keyboard SHALL refine each key's bottom-out reference from positions observed during normal use, so that calibration improves without an explicit calibration pass.

Learned bottom-out references SHALL be constrained to a plausible range derived from the switch specification, so that a spurious reading cannot corrupt a key's calibration.

#### Scenario: Calibration improves through use

- **WHEN** an uncalibrated key is bottomed out during normal typing
- **THEN** its bottom-out reference is refined toward the observed value and persisted

#### Scenario: Implausible observation is not adopted

- **WHEN** a reading outside the plausible range for the switch is observed on a key
- **THEN** that key's bottom-out reference is left unchanged

### Requirement: Detection of invalid calibration

The keyboard SHALL detect a key whose measured signal range is too small to be a working switch and sensor — for example a missing magnet, an unpopulated or failed sensor, or a broken connection.

Such a key SHALL NOT be reported as continuously pressed. The keyboard SHALL retain its previous or default calibration for that key and SHALL continue operating normally on all other keys.

#### Scenario: Key with a failed sensor

- **WHEN** a key's measured signal range is below the minimum plausible for a working switch
- **THEN** that key is not reported as pressed, its calibration is not overwritten, and all other keys continue to function

### Requirement: Calibration reset

The user SHALL be able to discard all stored calibration and return every key to the default calibration, without reflashing firmware.

#### Scenario: Restoring defaults

- **WHEN** the user invokes a calibration reset
- **THEN** all stored per-key calibration is discarded, the default calibration takes effect immediately, and the keyboard remains usable
