## Purpose

Defines the PCB-level contract for a magnetic-switch Clavis revision: how the switch and sensor are placed relative to one another, the signal and flux budget the board must satisfy, and the constraints the analog path imposes on the design.

## ADDED Requirements

### Requirement: Magnetic switch footprint

The board SHALL provide a footprint for the specified magnetic switch that matches its published mechanical drawing: a 14 × 14 mm plate cutout, a centre boss hole, and two mounting-leg holes on the specified pitch.

The footprint SHALL provide no electrical pads or through-holes for switch pins, because the switch has no electrical contacts.

The footprint SHALL be generated from the board's declarative layout source, so that the plate outlines are derived from the same definition as the switch positions.

#### Scenario: Switch seats mechanically

- **WHEN** a switch is fitted to a fabricated board
- **THEN** its centre boss and both mounting legs engage their holes and the switch is retained without a plate

#### Scenario: Plate matches the switch

- **WHEN** the top plate is generated from the layout source
- **THEN** its cutouts accept the switch and align with the switch positions on the board

### Requirement: Sensor placed on the face opposite the switch

The magnetic field sensor for each key SHALL be mounted on the board face opposite the switch, sensing through the board, in the configuration for which the switch manufacturer specifies flux.

Each sensor SHALL be aligned to the projected position of that switch's magnet, which is offset from the key centreline. Alignment tolerance SHALL be tight enough that placement variation does not materially change the measured signal range.

#### Scenario: Sensor aligns with the magnet

- **WHEN** a switch is fitted and the board is inspected
- **THEN** each sensor lies beneath its key's magnet at the offset given in the switch drawing, not on the key centreline

### Requirement: Flux and signal budget

The combination of switch, board thickness, and sensor SHALL produce a signal that changes monotonically with key travel across the whole actuation band, with sufficient range to meet the sensing capability's resolution requirement.

The design SHALL retain adequate margin when the magnet's flux is at either extreme of its specified tolerance, and across the specified operating temperature range, so that a key at rest is never mistaken for a key that is pressed.

Signal saturation SHALL be confined to the region near bottom-out, outside the configurable actuation band.

#### Scenario: Signal spans the travel

- **WHEN** a key is moved from rest to bottom-out on a fabricated board
- **THEN** the sensor signal changes monotonically and its usable range spans the actuation band

#### Scenario: Worst-case magnet tolerance

- **WHEN** the magnet's flux is at either limit of its specified tolerance
- **THEN** a key at rest is distinguishable from a key at the shallowest configurable actuation point

### Requirement: Switchable sensor power

The board SHALL allow the microcontroller to switch power to the sensors as a group, so that sensing power consumption is controlled by how often the sensors are energised.

While sensors are unpowered, the board SHALL draw negligible sensing current.

The capacitance on the switched sensor rail SHALL be small enough that repeatedly charging it does not dominate the sensing power budget at the intended sampling rates.

#### Scenario: Sensors can be de-energised

- **WHEN** firmware de-asserts the sensor power control
- **THEN** sensor supply current falls to a negligible level

#### Scenario: Rail capacitance does not dominate power

- **WHEN** the sensor rail is switched at the keyboard's highest sampling rate
- **THEN** the average current spent recharging the rail's capacitance is a minor fraction of the sensing power budget

### Requirement: Reversible board preserved

A single board design SHALL serve as both the left and right half, as the existing Clavis revisions do.

Switches SHALL mount on the finger-facing face and sensors uniformly on the opposite face for both halves, so that assembling a left or a right half requires no change in which components are populated and no change in magnet orientation relative to the sensor.

#### Scenario: One board yields either half

- **WHEN** two identical boards are assembled, one as each half
- **THEN** both use the same component placement, and both measure the same signal range for an equivalent key press

### Requirement: Microcontroller compatibility

The board SHALL remain compatible with the existing controller module footprint and pinout.

Signals read by the analog-to-digital converter SHALL be assigned to pins that the controller exposes as analog-capable, and the total pin count required SHALL NOT exceed what the module makes available.

#### Scenario: Design fits the existing module

- **WHEN** the board revision is routed
- **THEN** every analog input is on an analog-capable pin of the controller module and no required signal is left unassigned

### Requirement: Analog signal integrity

The analog signal path SHALL be routed to preserve the resolution required by the sensing capability, with a ground reference for the analog nets and without routing analog signals through the region reserved for the wireless module's antenna.

Measurement noise SHALL remain within the limit set by the sensing capability while the wireless radio is transmitting.

#### Scenario: Noise budget met during radio activity

- **WHEN** a key is held stationary while the keyboard is connected and transmitting
- **THEN** the reported travel for that key varies by no more than the sensing capability's resolution limit

### Requirement: Existing revisions remain buildable

The magnetic-switch board SHALL be introduced as an additional revision alongside the existing mechanical-switch revisions, following the project's established revision pattern.

The existing revisions SHALL remain generatable and fabricable from the repository, unchanged in behaviour.

#### Scenario: Mechanical revision still builds

- **WHEN** an existing mechanical-switch revision is generated after the magnetic revision is added
- **THEN** it produces the same outputs as before, with no dependency on the magnetic revision
