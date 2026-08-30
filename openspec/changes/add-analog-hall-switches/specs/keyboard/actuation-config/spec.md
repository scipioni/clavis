## Purpose

Defines the user-facing contract for when a key counts as pressed: how the actuation and release points are expressed and bounded, how they are configured and persisted, and what guarantees the keyboard makes about them.

## ADDED Requirements

### Requirement: Actuation point expressed in millimetres

The actuation point SHALL be configured as a physical depth in millimetres of key travel, not as a raw sensor value or an abstract percentage.

A key SHALL be reported as pressed when its travel reaches or exceeds the configured actuation point.

#### Scenario: Key actuates at the configured depth

- **WHEN** the actuation point is set to 0.8 mm and a key is depressed past 0.8 mm of travel
- **THEN** a press event is reported for that key

#### Scenario: Key does not actuate above the configured depth

- **WHEN** the actuation point is set to 1.5 mm and a key is depressed to only 1.0 mm of travel and held
- **THEN** no press event is reported for that key

### Requirement: Release point and guaranteed hysteresis

The release point SHALL be configurable as a physical depth in millimetres, and SHALL always be shallower than the actuation point.

The keyboard SHALL enforce a minimum separation between the actuation and release points. A configuration that would place the release point at or above the actuation point, or closer to it than the minimum separation, SHALL be rejected or clamped to a valid configuration rather than accepted.

A key held stationary at any depth SHALL NOT oscillate between pressed and released states.

#### Scenario: Key held at the actuation point does not chatter

- **WHEN** a key is held motionless at exactly its configured actuation point for one second
- **THEN** at most one state change is reported during that interval

#### Scenario: Invalid release point is rejected

- **WHEN** the user configures a release point deeper than or equal to the actuation point
- **THEN** the configuration is rejected or clamped, and the keyboard retains a valid actuation and release pair

### Requirement: Configuration bounds

Actuation and release points SHALL be constrained to the usable travel band of the switch. Values outside that band SHALL be rejected or clamped rather than accepted.

The usable band SHALL exclude a deadzone at the top of travel, so that a key at rest cannot be reported as pressed, and a deadzone at the bottom, so that a key cannot require bottoming out against the hard stop to actuate.

#### Scenario: Value beyond full travel is not accepted

- **WHEN** the user configures an actuation point deeper than the switch's usable travel band
- **THEN** the value is rejected or clamped to the deepest valid actuation point

#### Scenario: Key at rest is never pressed

- **WHEN** any valid actuation point is configured and no key is touched
- **THEN** no key is reported as pressed

### Requirement: Default configuration

The keyboard SHALL provide default actuation and release points that are usable for typing without any user configuration, and that fall within the usable travel band for the switch as specified.

#### Scenario: Defaults apply on first boot

- **WHEN** firmware is flashed with no stored actuation configuration
- **THEN** the default actuation and release points are in effect and the keyboard types correctly

### Requirement: Runtime adjustment from the keymap

The user SHALL be able to adjust the actuation point at runtime from the keymap, without reflashing firmware and without external tools.

Adjustments SHALL take effect immediately for subsequent key presses.

#### Scenario: Adjusting actuation while typing

- **WHEN** the user invokes the keymap action that changes the actuation point
- **THEN** subsequent key presses actuate at the new depth

#### Scenario: Adjustment respects bounds

- **WHEN** the user repeatedly invokes the keymap action to move the actuation point beyond a bound
- **THEN** the actuation point stops at the bound and remains valid

### Requirement: Configuration scope

The keyboard SHALL support a global actuation and release configuration applying to all keys, and MAY support per-key overrides. Where a per-key override exists it SHALL take precedence over the global configuration for that key.

#### Scenario: Global configuration applies to all keys

- **WHEN** the global actuation point is changed and no per-key overrides are set
- **THEN** every key actuates at the new depth

#### Scenario: Per-key override takes precedence

- **WHEN** a per-key actuation override is set for one key and the global actuation point is changed
- **THEN** the overridden key continues to actuate at its own configured depth while other keys follow the global value

### Requirement: Configuration persists across power loss

Actuation configuration SHALL be stored in non-volatile memory and restored on startup, surviving reboot and battery disconnection.

#### Scenario: Settings survive a power cycle

- **WHEN** the user adjusts the actuation point and then power-cycles the keyboard
- **THEN** the adjusted actuation point is still in effect after startup

### Requirement: Configuration is independent of calibration

Actuation configuration and per-key calibration SHALL be stored and reset independently. Resetting calibration SHALL NOT discard the user's actuation configuration, and changing the actuation configuration SHALL NOT invalidate calibration.

#### Scenario: Calibration reset preserves actuation settings

- **WHEN** the user resets calibration after having adjusted the actuation point
- **THEN** the adjusted actuation point remains in effect
