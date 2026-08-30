## Purpose

Defines how Clavis measures each key's physical position as a continuous value rather than a binary contact, and the accuracy, latency, power, and compatibility behaviour that measurement must exhibit.

## ADDED Requirements

### Requirement: Continuous key position measurement

The keyboard SHALL measure and report each key's travel as a continuous position value expressed in millimetres, where 0 mm corresponds to the key's rest position and the switch's nominal full travel corresponds to a fully bottomed-out key.

Reported travel SHALL increase monotonically as the key is depressed and decrease monotonically as it is released.

#### Scenario: Key at rest

- **WHEN** a calibrated key is untouched
- **THEN** its reported travel is 0.0 mm within the measurement tolerance

#### Scenario: Key fully depressed

- **WHEN** a calibrated key is pressed to its hard stop
- **THEN** its reported travel equals the switch's nominal full travel within the measurement tolerance

#### Scenario: Partial press is monotonic

- **WHEN** a key is depressed slowly and continuously from rest to bottom-out
- **THEN** the sequence of reported travel values is non-decreasing, with no reversals outside the measurement tolerance

### Requirement: Measurement resolution across the actuation band

The keyboard SHALL resolve changes in key travel of 0.05 mm or finer at every position within the actuation band of 0.2 mm to 2.5 mm of travel.

Measurement noise SHALL NOT exceed the resolution figure, so that a stationary key does not produce travel readings that vary by more than 0.05 mm.

#### Scenario: Resolving a small displacement

- **WHEN** a key is displaced by 0.1 mm anywhere within the actuation band
- **THEN** the change is reflected in the reported travel and is distinguishable from measurement noise

#### Scenario: Stationary key produces a stable reading

- **WHEN** a key is held motionless at a fixed travel position for one second
- **THEN** the reported travel varies by no more than 0.05 mm over that interval

### Requirement: Travel reported in linearized physical units

Reported travel SHALL be linear in physical displacement. An equal increment of reported travel SHALL correspond to an equal increment of physical key displacement anywhere within the actuation band, so that a configured actuation point expressed in millimetres actuates at the same physical depth regardless of where in the travel it is placed.

This requirement exists because the underlying sensor signal is substantially more sensitive near bottom-out than near rest; the raw signal is therefore not an acceptable unit for reported travel.

#### Scenario: Equal millimetre increments are equal physical displacements

- **WHEN** a key is moved from 0.5 mm to 0.6 mm of reported travel, and separately from 2.0 mm to 2.1 mm of reported travel
- **THEN** both correspond to the same physical displacement within the measurement tolerance

#### Scenario: Actuation depth is consistent across keys

- **WHEN** the same actuation point in millimetres is configured for two different calibrated keys
- **THEN** both keys actuate at the same physical depth within the measurement tolerance

### Requirement: Adaptive scan rate

The keyboard SHALL sample key positions at a rate that adapts to recent key activity: a high rate during active typing, one or more reduced rates after a period of inactivity, and a low idle rate when no key has moved for an extended period.

The keyboard SHALL return to the high rate on the first detected key movement.

The scan rate SHALL be the mechanism by which sensing power consumption is controlled.

#### Scenario: Rate rises on activity

- **WHEN** a key moves while the keyboard is sampling at a reduced rate
- **THEN** the keyboard returns to its high sampling rate before the next key event is reported

#### Scenario: Rate falls when idle

- **WHEN** no key has moved for the configured inactivity interval
- **THEN** the keyboard reduces its sampling rate and its average sensing current falls correspondingly

### Requirement: First keypress after idle is not lost

A keypress that occurs while the keyboard is sampling at its idle rate SHALL be registered and reported. It MAY be delayed by up to one idle scan period, but SHALL NOT be dropped.

#### Scenario: Key pressed during idle sampling

- **WHEN** a key is pressed and released after the keyboard has been idle for an extended period
- **THEN** the corresponding press and release events are reported to the host

### Requirement: Freedom from contact bounce

Because the switches have no electrical contacts, a single crossing of the actuation threshold SHALL produce exactly one press event, and a single crossing of the release threshold SHALL produce exactly one release event, without requiring a debounce interval.

#### Scenario: Single press produces a single event

- **WHEN** a key is pressed once past the actuation point
- **THEN** exactly one press event is reported, with no debounce delay applied

#### Scenario: Slow press across the threshold

- **WHEN** a key is depressed very slowly across the actuation point
- **THEN** exactly one press event is reported and no oscillation between press and release occurs

### Requirement: Compatibility with the existing keyboard stack

The sensing subsystem SHALL report key state to the rest of the keyboard as ordinary press and release events, indistinguishable from those produced by a mechanical matrix.

Continuous position values SHALL remain local to the half that measures them and SHALL NOT be transmitted across the split link.

Existing layout behaviour — layers, home-row mods, combos, tap-hold behaviours, and split operation — SHALL continue to function without modification.

#### Scenario: Layout behaviours are unaffected

- **WHEN** the keyboard is built with analog sensing and the existing layout
- **THEN** layers, home-row mods, and combos behave as they did with the mechanical matrix

#### Scenario: Split link carries only key events

- **WHEN** a key is pressed on the peripheral half
- **THEN** only a press event crosses the split link; no continuous position data is transmitted

### Requirement: Tolerance of signal saturation near bottom-out

If the sensor signal saturates before the key reaches its hard stop, the keyboard SHALL continue to report the key as pressed and SHALL report travel clamped to the saturation depth, rather than producing an erroneous or non-monotonic reading.

#### Scenario: Key pressed into the saturation region

- **WHEN** a key is pressed past the depth at which the sensor signal saturates
- **THEN** the key remains reported as pressed and the reported travel does not decrease
